'use strict';
// Visual Studio style formatter: spacing, brace placement, blank lines and indentation.
const P = require('./parser');

// ---------------------------------------------------------------- helpers
function lineStates(text) {
  // block-comment state at the start of each line
  const res = [false];
  let i = 0, inBlock = false, inLine = false;
  const n = text.length;
  while (i < n) {
    const c = text[i], d = text[i + 1];
    if (c === '\n') { inLine = false; res.push(inBlock); i++; continue; }
    if (inBlock) { if (c === '*' && d === '/') { inBlock = false; i += 2; continue; } i++; continue; }
    if (inLine) { i++; continue; }
    if (c === '/' && d === '/') { inLine = true; i += 2; continue; }
    if (c === '/' && d === '*') { inBlock = true; i += 2; continue; }
    i++;
  }
  return res;
}
function splitComment(line) {
  const ci = line.indexOf('//');
  return ci >= 0 ? { code: line.slice(0, ci), comment: line.slice(ci) } : { code: line, comment: '' };
}

// ---------------------------------------------------------------- spacing
const OPS = ['<<=', '>>=', '++', '--', '==', '!=', '<=', '>=', '&&', '||', '^^', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '<<', '>>',
  '+', '-', '*', '/', '%', '=', '<', '>', '!', '~', '&', '|', '^', '?', ':', '(', ')', '[', ']', '{', '}', ',', ';', '.'];
const BIN = new Set(['<<=', '>>=', '==', '!=', '<=', '>=', '&&', '||', '^^', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '<<', '>>',
  '*', '/', '%', '=', '<', '>', '&', '|', '^', '?', ':']);
const UNARY_CTX = new Set(['binop', 'unop', '(', '[', ',', ';', '{', 'label']);
const CONTROL = new Set(['if', 'for', 'while', 'switch', 'return']);
const NUM = /^(?:0[xX][0-9a-fA-F]+[uU]?|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?(?:lf|LF|[fFuU])?)/;
const WORD = /^[A-Za-z_]\w*/;

function tokenize(s) {
  const toks = [];
  let i = 0;
  while (i < s.length) {
    const c = s[i];
    if (c === ' ' || c === '\t') { i++; continue; }
    const rest = s.slice(i);
    let m;
    if ((/\d/.test(c) || (c === '.' && /\d/.test(s[i + 1] || ''))) && (m = NUM.exec(rest))) { toks.push({ t: m[0], k: 'word' }); i += m[0].length; continue; }
    if ((m = WORD.exec(rest))) { toks.push({ t: m[0], k: 'word' }); i += m[0].length; continue; }
    const op = OPS.find(o => rest.startsWith(o));
    if (!op) return null;
    toks.push({ t: op, k: op });
    i += op.length;
  }
  return toks;
}
function classify(toks) {
  const isLabelLine = toks.length && (toks[0].t === 'case' || toks[0].t === 'default');
  let ternary = 0, depth = 0, labelDone = false;
  for (let i = 0; i < toks.length; i++) {
    const tk = toks[i], prev = toks[i - 1];
    if (tk.t === '(' || tk.t === '[') depth++;
    else if (tk.t === ')' || tk.t === ']') depth--;
    if (tk.t === '+' || tk.t === '-') {
      const unary = !prev || UNARY_CTX.has(prev.k) || (prev.k === 'word' && (prev.t === 'return' || prev.t === 'case'));
      tk.k = unary ? 'unop' : 'binop';
    } else if (tk.t === '!' || tk.t === '~') tk.k = 'unop';
    else if (tk.t === '++' || tk.t === '--') tk.k = prev && (prev.k === 'word' || prev.t === ')' || prev.t === ']') ? 'postop' : 'unop';
    else if (tk.t === '?') { ternary++; tk.k = 'binop'; }
    else if (tk.t === ':') {
      if (ternary > 0) { ternary--; tk.k = 'binop'; }
      else if (isLabelLine && !labelDone && depth === 0) { tk.k = 'label'; labelDone = true; }
      else tk.k = 'binop';
    } else if (BIN.has(tk.t)) tk.k = 'binop';
  }
}
function needSpace(p, c) {
  if (c.k === ',' || c.k === ';' || c.k === ')' || c.k === ']' || c.k === '.' || c.k === 'postop' || c.k === 'label') return false;
  if (p.k === '(' || p.k === '[' || p.k === '.' || p.k === 'unop') return false;
  if (c.k === '[') return false;
  if (c.k === '(') {
    if (p.k === 'word') return CONTROL.has(p.t);
    if (p.k === ')' || p.k === ']') return false;
  }
  return true;
}
function spaceLine(line) {
  const { code, comment } = splitComment(line);
  if (!code.trim() || code.includes('/*') || code.includes('*/')) return line;
  const toks = tokenize(code);
  if (!toks || !toks.length) return line;
  classify(toks);
  let out = toks[0].t;
  for (let k = 1; k < toks.length; k++) out += (needSpace(toks[k - 1], toks[k]) ? ' ' : '') + toks[k].t;
  if (comment) out += ' '.repeat(Math.max(1, code.length - code.trimEnd().length)) + comment;
  return out;
}

// ---------------------------------------------------------------- braces & blank lines (whole document only)
function restructure(lines, opts) {
  const inComment = lineStates(lines.join('\n'));
  const out = [];
  const isCode = (t, i) => !inComment[i] && t && t[0] !== '#' && !t.startsWith('//');
  for (let i = 0; i < lines.length; i++) {
    const raw = lines[i];
    const t = raw.trim();
    if (!isCode(t, i) || opts.braceStyle === 'keep') { out.push(raw); continue; }
    const { code, comment } = splitComment(t);
    if (opts.braceStyle === 'newLine') {
      let c = code.trim();
      const parts = [];
      if (c.length > 1 && c[0] === '}' && !comment) {
        const rest = c.slice(1).trim();
        if (rest && !/^[;,)]/.test(rest) && !/^while\b/.test(rest)) { parts.push('}'); c = rest; }
      }
      if (c.length > 1 && c.endsWith('{') && !/[=,(]\s*\{$/.test(c)) {
        parts.push(c.slice(0, -1).trimEnd());
        parts.push('{' + (comment ? ' ' + comment : ''));
      } else {
        parts.push(c + (comment ? ' ' + comment : ''));
      }
      if (parts.length === 1) out.push(raw); else out.push(...parts);
    } else if (opts.braceStyle === 'sameLine') {
      let j = out.length - 1;
      while (j >= 0 && !out[j].trim()) j--;
      const prev = j >= 0 ? out[j].trim() : '';
      const prevOk = prev && prev[0] !== '#' && !prev.includes('//') && !prev.includes('*/') && !/[;{},]$/.test(prev);
      if (code.trim() === '{' && prevOk && j === out.length - 1) out[j] = out[j].replace(/\s+$/, '') + ' {' + (comment ? ' ' + comment : '');
      else if (/^else\b/.test(t) && prev === '}' && j === out.length - 1) out[j] = out[j].replace(/\s+$/, '') + ' ' + t;
      else out.push(raw);
    } else out.push(raw);
  }
  if (opts.maxBlankLines >= 0) {
    const inC = lineStates(out.join('\n'));
    const res = [];
    let blanks = 0;
    for (let i = 0; i < out.length; i++) {
      if (!out[i].trim() && !inC[i]) { blanks++; if (blanks > opts.maxBlankLines) continue; } else blanks = 0;
      res.push(out[i]);
    }
    return res;
  }
  return out;
}

// ---------------------------------------------------------------- indentation (+ per-line spacing)
function formatLines(text, opts, fromLine, toLine) {
  const unit = opts.insertSpaces ? ' '.repeat(opts.tabSize || 4) : '\t';
  const lines = text.split('\n');
  const cleanLines = P.stripComments(text).split('\n');
  const inComment = lineStates(text);
  if (fromLine == null) { fromLine = 0; toLine = lines.length - 1; }

  const stack = [];
  let paren = 0, pending = 0, pendingSwitch = false;
  const edits = [];

  for (let i = 0; i < lines.length; i++) {
    const raw = lines[i].replace(/\r$/, '');
    const trimmed = raw.trim();
    const ct = (cleanLines[i] || '').replace(/\r$/, '').trim();
    let out = raw;

    if (inComment[i]) {
      out = raw.replace(/\s+$/, '');
    } else if (trimmed === '') {
      out = '';
    } else if (trimmed[0] === '#') {
      out = trimmed;
    } else {
      let closers = 0;
      while (closers < ct.length && ct[closers] === '}') closers++;
      const depth = Math.max(0, stack.length - closers);
      let level = depth;
      const isLabel = /^(case\b[^:]*|default\s*):/.test(ct);
      const owner = stack[depth - 1];
      if (owner && owner.type === 'switch' && owner.caseSeen && !isLabel) level++;
      if (paren > 0 && !/^[)\]]/.test(ct)) level++;
      if (pending > 0 && ct[0] !== '{') level += pending;
      out = unit.repeat(level) + (opts.spacing ? spaceLine(trimmed) : trimmed);

      if (/\bswitch\s*\(/.test(ct)) pendingSwitch = true;
      for (const c of ct) {
        if (c === '{') { stack.push({ type: pendingSwitch ? 'switch' : 'block', caseSeen: false }); pendingSwitch = false; }
        else if (c === '}') stack.pop();
        else if (c === '(' || c === '[') paren++;
        else if (c === ')' || c === ']') paren = Math.max(0, paren - 1);
      }
      if (isLabel && stack.length && stack[stack.length - 1].type === 'switch') stack[stack.length - 1].caseSeen = true;

      const header = paren === 0 && (/^(\}\s*)?(else\s+)?(if|for|while)\s*\(.*\)$/.test(ct) || /^(\}\s*)?else$/.test(ct));
      if (header) pending++;
      else if (ct.endsWith('{') || ct[0] === '{') pending = 0;
      else if (paren === 0 && (ct.endsWith(';') || ct.endsWith('}'))) pending = 0;
    }

    if (i >= fromLine && i <= toLine && out !== raw) edits.push({ line: i, text: out });
  }
  return edits;
}

const DEFAULTS = { insertSpaces: true, tabSize: 4, spacing: true, braceStyle: 'newLine', maxBlankLines: 2 };

function formatDocument(text, options) {
  const opts = Object.assign({}, DEFAULTS, options);
  const eol = text.includes('\r\n') ? '\r\n' : '\n';
  let lines = restructure(text.split(/\r?\n/), opts);
  const t1 = lines.join('\n');
  lines = t1.split('\n');
  for (const e of formatLines(t1, opts)) lines[e.line] = e.text;
  return lines.join(eol);
}
function formatRange(text, options, fromLine, toLine) {
  return formatLines(text, Object.assign({}, DEFAULTS, options), fromLine, toLine);
}

module.exports = { formatDocument, formatRange, spaceLine };

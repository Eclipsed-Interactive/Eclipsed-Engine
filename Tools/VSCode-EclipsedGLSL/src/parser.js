'use strict';
// Lightweight, error-tolerant GLSL symbol parser (no vscode dependency).
const B = require('./builtins');

const QUALS = 'const|in|out|inout|uniform|buffer|shared|attribute|varying|centroid|flat|smooth|noperspective|patch|sample|invariant|precise|highp|mediump|lowp|coherent|volatile|restrict|readonly|writeonly';
const STORAGE = new Set(['in', 'out', 'inout', 'uniform', 'buffer', 'shared', 'const', 'attribute', 'varying']);
const RE_BLOCK = new RegExp('(layout\\s*\\(([^)]*)\\)\\s*)?((?:\\b(?:' + QUALS + ')\\s+)*)\\b(uniform|buffer|in|out)\\s+([A-Za-z_]\\w*)\\s*\\{', 'g');
const RE_VAR = new RegExp('(layout\\s*\\(([^)]*)\\)\\s*)?((?:\\b(?:' + QUALS + ')\\s+)*)\\b([A-Za-z_]\\w*)((?:\\s*\\[[^\\]]*\\])*)\\s+([A-Za-z_]\\w*)((?:\\s*\\[[^\\]]*\\])*)\\s*(?=[=;,)])', 'g');
const RE_PARAM = new RegExp('^(\\s*)((?:\\b(?:' + QUALS + ')\\s+)*)([A-Za-z_]\\w*)((?:\\s*\\[[^\\]]*\\])*)\\s*([A-Za-z_]\\w*)?((?:\\s*\\[[^\\]]*\\])*)');
const RE_FIELD = new RegExp('^(\\s*(?:layout\\s*\\([^)]*\\)\\s*)?(?:\\b(?:' + QUALS + ')\\s+)*)([A-Za-z_]\\w*)((?:\\s*\\[[^\\]]*\\])*)\\s+');

function stripComments(text) {
  const chars = text.split('');
  const n = text.length;
  let i = 0;
  while (i < n) {
    const c = text[i], d = text[i + 1];
    if (c === '/' && d === '/') {
      while (i < n && text[i] !== '\n' && text[i] !== '\r') { chars[i] = ' '; i++; }
    } else if (c === '/' && d === '*') {
      chars[i] = ' '; chars[i + 1] = ' '; i += 2;
      while (i < n && !(text[i] === '*' && text[i + 1] === '/')) { if (text[i] !== '\n' && text[i] !== '\r') chars[i] = ' '; i++; }
      if (i < n) { chars[i] = ' '; chars[i + 1] = ' '; i += 2; }
    } else if (c === '"') {
      i++;
      while (i < n && text[i] !== '"' && text[i] !== '\n') i++;
      i++;
    } else i++;
  }
  return chars.join('');
}

function splitTopLevel(s, base) {
  const out = [];
  let depth = 0, st = 0;
  for (let i = 0; i < s.length; i++) {
    const c = s[i];
    if (c === '(' || c === '[' || c === '{') depth++;
    else if (c === ')' || c === ']' || c === '}') depth--;
    else if (c === ',' && depth === 0) { out.push({ text: s.slice(st, i), offset: base + st }); st = i + 1; }
  }
  out.push({ text: s.slice(st), offset: base + st });
  return out;
}

function countDims(arr) { return ((arr || '').match(/\[/g) || []).length; }
function layoutInt(layout, key) {
  const m = new RegExp('\\b' + key + '\\s*=\\s*(\\d+)').exec(layout || '');
  return m ? +m[1] : null;
}

function matchBrackets(t) {
  const stack = [], pairs = [], byOpen = new Map(), errors = [];
  const OPEN = '({[', CLOSE = ')}]';
  for (let i = 0; i < t.length; i++) {
    const c = t[i];
    if (OPEN.includes(c)) stack.push(i);
    else if (CLOSE.includes(c)) {
      const want = OPEN[CLOSE.indexOf(c)];
      if (stack.length && t[stack[stack.length - 1]] === want) {
        const o = stack.pop();
        if (c === '}') { const p = { open: o, close: i }; pairs.push(p); byOpen.set(o, p); }
      } else {
        const at = stack.lastIndexOf(stack.slice().reverse().find(o => t[o] === want));
        if (at >= 0 && stack.length - at <= 2) {
          // a nearby unclosed bracket swallowed this one: report it, then match
          while (stack.length - 1 > at) { const o = stack.pop(); errors.push({ offset: o, message: "Unclosed '" + t[o] + "'" }); }
          const o = stack.pop();
          if (c === '}') { const p = { open: o, close: i }; pairs.push(p); byOpen.set(o, p); }
        } else {
          errors.push({ offset: i, message: "Unmatched '" + c + "'" });
        }
      }
    }
  }
  for (const o of stack) {
    errors.push({ offset: o, message: "Unclosed '" + t[o] + "'" });
    if (t[o] === '{') { const p = { open: o, close: t.length }; pairs.push(p); byOpen.set(o, p); }
  }
  pairs.sort((a, b) => a.open - b.open);
  return { pairs, byOpen, errors };
}

function innermost(pairs, off) {
  let best = null;
  for (const p of pairs) {
    if (p.open >= off) break;
    if (off <= p.close) best = p;
  }
  return best;
}

function parseFields(clean, start, end) {
  const fields = [];
  let segStart = start;
  for (let i = start; i <= end; i++) {
    if (i === end || clean[i] === ';') {
      const seg = clean.slice(segStart, i);
      const m = RE_FIELD.exec(seg);
      if (m && !B.KEYWORD_SET.has(m[2]) || (m && B.TYPE_SET.has(m[2]))) {
        const rest = seg.slice(m[0].length);
        for (const piece of splitTopLevel(rest, segStart + m[0].length)) {
          const dm = /^\s*([A-Za-z_]\w*)((?:\s*\[[^\]]*\])*)/.exec(piece.text);
          if (!dm) continue;
          fields.push({ name: dm[1], type: m[2], array: (m[3] + dm[2]).replace(/\s+/g, ''), offset: piece.offset + dm[0].indexOf(dm[1]) });
        }
      }
      segStart = i + 1;
    }
  }
  return fields;
}

function parse(text) {
  const noComments = stripComments(text);
  const macros = [];
  let version = null;
  const chars = noComments.split('');
  const pre = /^[ \t]*#[^\n\r]*/gm;
  let m;
  while ((m = pre.exec(noComments))) {
    const line = m[0];
    const dm = /^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)(\([^)]*\))?[ \t]*(.*)$/.exec(line);
    if (dm) macros.push({ name: dm[1], params: dm[2] || '', value: dm[3].trim(), offset: m.index + line.indexOf(dm[1], line.indexOf('define') + 6) });
    const vm = /^[ \t]*#[ \t]*version[ \t]+(\d+)(?:[ \t]+(\w+))?/.exec(line);
    if (vm) version = { number: +vm[1], profile: vm[2] || '', offset: m.index };
    for (let i = m.index; i < m.index + line.length; i++) chars[i] = ' ';
  }
  const clean = chars.join('');
  const { pairs, byOpen, errors } = matchBrackets(clean);

  // structs
  const structs = new Map();
  const fieldBodies = new Set();
  const reStruct = /\bstruct\s+([A-Za-z_]\w*)\s*\{/g;
  while ((m = reStruct.exec(clean))) {
    const open = m.index + m[0].length - 1;
    const p = byOpen.get(open);
    if (!p) continue;
    fieldBodies.add(open);
    structs.set(m[1], { name: m[1], kind: 'struct', fields: parseFields(clean, open + 1, p.close),
      offset: m.index + m[0].indexOf(m[1], 6), start: m.index, end: Math.min(p.close + 1, clean.length) });
  }

  // interface blocks
  const blocks = [];
  RE_BLOCK.lastIndex = 0;
  while ((m = RE_BLOCK.exec(clean))) {
    const open = m.index + m[0].length - 1;
    const p = byOpen.get(open);
    if (!p || innermost(pairs, m.index)) continue;
    fieldBodies.add(open);
    const name = m[5];
    const after = clean.slice(p.close + 1, p.close + 256);
    const im = /^\s*([A-Za-z_]\w*)?\s*((?:\[[^\]]*\]\s*)*);/.exec(after);
    const instance = im && im[1] ? im[1] : null;
    const layout = (m[2] || '').trim();
    blocks.push({ name, kind: 'block', storage: m[4], layout, binding: layoutInt(layout, 'binding'),
      fields: parseFields(clean, open + 1, p.close), offset: m.index + m[0].lastIndexOf(name), start: m.index,
      end: Math.min(clean.length, p.close + 1 + (im ? im[0].length : 0)), instance,
      instanceOffset: instance ? p.close + 1 + im[0].indexOf(instance) : -1, instanceArray: im ? im[2].replace(/\s+/g, '') : '' });
  }
  const types = new Map(structs);
  for (const b of blocks) if (!types.has(b.name)) types.set(b.name, b);
  const isType = n => B.TYPE_SET.has(n) || structs.has(n);

  // functions
  const functions = [];
  const paramRanges = [];
  const reFunc = /\b([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*\(([^()]*)\)\s*(\{|;)/g;
  while ((m = reFunc.exec(clean))) {
    const ret = m[1], name = m[2];
    if (!isType(ret) || B.KEYWORD_SET.has(name) || isType(name)) { reFunc.lastIndex = m.index + m[1].length; continue; }
    if (innermost(pairs, m.index)) continue;
    const parenOpen = m.index + m[0].indexOf('(');
    const parenClose = parenOpen + 1 + m[3].length;
    paramRanges.push([parenOpen, parenClose]);
    const body = m[4] === '{' ? (byOpen.get(m.index + m[0].length - 1) || null) : null;
    const params = [];
    const ptxt = m[3].trim();
    if (ptxt && ptxt !== 'void') {
      for (const piece of splitTopLevel(m[3], parenOpen + 1)) {
        const pm = RE_PARAM.exec(piece.text);
        if (!pm) continue;
        const pname = pm[5] || '';
        params.push({ type: pm[3], name: pname, qualifiers: pm[2].trim(), array: (pm[4] + pm[6]).replace(/\s+/g, ''),
          offset: pname ? piece.offset + pm[0].lastIndexOf(pname) : piece.offset, label: piece.text.trim().replace(/\s+/g, ' ') });
      }
    }
    functions.push({ name, returnType: ret, params, signature: ret + ' ' + name + '(' + params.map(p => p.label).join(', ') + ')',
      offset: m.index + m[0].indexOf(name, m[1].length), start: m.index, end: body ? Math.min(body.close + 1, clean.length) : m.index + m[0].length,
      body, paramRange: [parenOpen, parenClose], isPrototype: m[4] === ';' });
  }

  // variables
  const variables = [];
  RE_VAR.lastIndex = 0;
  while ((m = RE_VAR.exec(clean))) {
    const type = m[4], name = m[6];
    if (!isType(type) || B.KEYWORD_SET.has(name) || isType(name)) { RE_VAR.lastIndex = m.index + 1; continue; }
    const typeIdx = m[0].indexOf(type, (m[1] || '').length + m[3].length);
    const nameOff = m.index + m[0].indexOf(name, typeIdx + type.length);
    const encl = innermost(pairs, nameOff);
    if (encl && fieldBodies.has(encl.open)) continue;
    if (paramRanges.some(r => nameOff > r[0] && nameOff < r[1])) continue;
    const quals = m[3].trim().split(/\s+/).filter(Boolean);
    const storage = quals.find(q => STORAGE.has(q)) || '';
    const base = { name, type, array: (m[5] + m[7]).replace(/\s+/g, ''), qualifiers: quals, storage, layout: (m[2] || '').trim(),
      offset: nameOff, scope: encl ? { open: encl.open, close: encl.close } : null };
    variables.push(base);
    const end = m.index + m[0].length;
    if (clean[end] === ',' || clean[end] === '=') {
      let depth = 0, j = end;
      for (; j < clean.length; j++) {
        const c = clean[j];
        if (c === '(' || c === '[' || c === '{') depth++;
        else if (c === ')' || c === ']' || c === '}') { if (depth === 0) break; depth--; }
        else if (c === ';' && depth === 0) break;
      }
      const pieces = splitTopLevel(clean.slice(end, j), end);
      for (let k = 1; k < pieces.length; k++) {
        const dm = /^\s*([A-Za-z_]\w*)((?:\s*\[[^\]]*\])*)/.exec(pieces[k].text);
        if (dm) variables.push(Object.assign({}, base, { name: dm[1], array: (m[5] + dm[2]).replace(/\s+/g, ''), offset: pieces[k].offset + dm[0].indexOf(dm[1]) }));
      }
    }
  }
  for (const f of functions) {
    if (!f.body) continue;
    for (const p of f.params) {
      if (!p.name) continue;
      variables.push({ name: p.name, type: p.type, array: p.array, qualifiers: p.qualifiers.split(/\s+/).filter(Boolean), storage: '', layout: '',
        offset: p.offset, scope: { open: f.body.open, close: f.body.close }, isParam: true, func: f });
    }
  }
  for (const b of blocks) {
    if (b.instance) {
      variables.push({ name: b.instance, type: b.name, array: b.instanceArray, qualifiers: [b.storage], storage: b.storage, layout: b.layout,
        offset: b.instanceOffset, scope: null, block: b });
    } else {
      for (const f of b.fields) variables.push({ name: f.name, type: f.type, array: f.array, qualifiers: [b.storage], storage: b.storage,
        layout: b.layout, offset: f.offset, scope: null, block: b, blockMember: true });
    }
  }
  variables.sort((a, b) => a.offset - b.offset);
  return { text, clean, version, macros, pairs, errors, structs, blocks, types, functions, variables };
}

function visibleVars(model, off) {
  return model.variables.filter(v => !v.scope || (v.scope.open < off && off <= v.scope.close && (v.isParam || v.offset < off)));
}

function lookupVar(model, name, off) {
  let best = null;
  for (const v of model.variables) {
    if (v.name !== name) continue;
    if (v.scope && !(v.scope.open < off && off <= v.scope.close && (v.isParam || v.offset < off))) continue;
    if (!best) { best = v; continue; }
    const bs = best.scope ? best.scope.open : -1, vs = v.scope ? v.scope.open : -1;
    if (vs > bs || (vs === bs && v.offset > best.offset && v.offset < off)) best = v;
  }
  return best;
}

// Walks backwards from the '.' at index dot and returns the access chain, e.g. a[0].b.c -> [{a,index:1},{b},{c}]
function extractChain(t, dot) {
  const parts = [];
  let i = dot;
  for (;;) {
    let j = i - 1;
    while (j >= 0 && /\s/.test(t[j])) j--;
    let index = 0, call = false;
    while (j >= 0 && (t[j] === ']' || t[j] === ')')) {
      const close = t[j], open = close === ']' ? '[' : '(';
      let depth = 0;
      for (; j >= 0; j--) {
        if (t[j] === close) depth++;
        else if (t[j] === open) { depth--; if (depth === 0) break; }
      }
      if (j < 0) return null;
      if (close === ']') index++; else call = true;
      j--;
      while (j >= 0 && /\s/.test(t[j])) j--;
    }
    const e = j + 1;
    while (j >= 0 && /\w/.test(t[j])) j--;
    const name = t.slice(j + 1, e);
    if (!name || /^\d/.test(name)) return null;
    parts.unshift({ name, index, call });
    let k = j;
    while (k >= 0 && (t[k] === ' ' || t[k] === '\t')) k--;
    if (k >= 0 && t[k] === '.') { i = k; continue; }
    break;
  }
  return parts;
}

function applyIndex(cur, n) {
  for (let i = 0; i < n && cur; i++) {
    if (cur.dims > 0) cur = { type: cur.type, dims: cur.dims - 1 };
    else {
      const vi = B.vecInfo(cur.type);
      if (vi) { cur = { type: vi.scalar, dims: 0 }; continue; }
      const mi = B.matInfo(cur.type);
      cur = mi ? { type: mi.column, dims: 0 } : null;
    }
  }
  return cur;
}

function memberType(model, cur, name, call) {
  if (!cur) return null;
  if (cur.dims > 0) return name === 'length' ? { type: 'int', dims: 0, method: true } : null;
  if (name === 'length' && call && (B.vecInfo(cur.type) || B.matInfo(cur.type))) return { type: 'int', dims: 0, method: true };
  const st = model.types.get(cur.type) || B.BUILTIN_STRUCTS[cur.type];
  if (st) {
    const f = st.fields.find(f => f.name === name);
    return f ? { type: f.type, dims: countDims(f.array), field: f, owner: st } : null;
  }
  const vi = B.vecInfo(cur.type);
  if (vi && B.isSwizzle(name, vi.size)) return { type: B.vecType(vi.scalar, name.length), dims: 0, swizzle: true };
  return null;
}

function resolveChain(model, parts, off) {
  let cur = null;
  const p0 = parts[0];
  if (p0.call) {
    const f = model.functions.find(f => f.name === p0.name);
    if (f) cur = { type: f.returnType, dims: 0 };
    else if (B.TYPE_SET.has(p0.name) || model.structs.has(p0.name)) cur = { type: p0.name, dims: 0 };
    else if (B.FUNC_RETURNS[p0.name]) cur = { type: B.FUNC_RETURNS[p0.name], dims: 0 };
  } else {
    const v = lookupVar(model, p0.name, off);
    if (v) cur = { type: v.type, dims: countDims(v.array) };
    else if (B.VARS[p0.name]) cur = { type: B.VARS[p0.name].type, dims: countDims(B.VARS[p0.name].array) };
  }
  cur = applyIndex(cur, p0.index);
  for (let k = 1; k < parts.length && cur; k++) {
    cur = memberType(model, cur, parts[k].name, parts[k].call);
    cur = applyIndex(cur, parts[k].index);
  }
  return cur;
}

module.exports = { parse, stripComments, visibleVars, lookupVar, extractChain, resolveChain, memberType, innermost, countDims, layoutInt };

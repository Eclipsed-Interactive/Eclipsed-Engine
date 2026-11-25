'use strict';
const vscode = require('vscode');
const path = require('path');
const fs = require('fs');
const cp = require('child_process');
const B = require('./builtins');
const P = require('./parser');
const F = require('./formatter');

const LANG = 'eclipsed-glsl';
const STAGE_INFO = {
  vertex: { letter: 'v', label: 'Vertex', glslang: 'vert', exts: ['.vglsl', '.vert'] },
  tesscontrol: { letter: 't', label: 'Tess Control', glslang: 'tesc', exts: ['.tglsl', '.hglsl', '.tesc'] },
  tesseval: { letter: 'e', label: 'Tess Evaluation', glslang: 'tese', exts: ['.eglsl', '.dglsl', '.tese'] },
  geometry: { letter: 'g', label: 'Geometry', glslang: 'geom', exts: ['.gglsl', '.geom'] },
  fragment: { letter: 'f', label: 'Pixel', glslang: 'frag', exts: ['.pglsl', '.fglsl', '.frag'] },
  compute: { letter: 'c', label: 'Compute', glslang: 'comp', exts: ['.cglsl', '.comp'] },
};
const STAGES = Object.keys(STAGE_INFO);
const PIPELINE = ['vertex', 'tesscontrol', 'tesseval', 'geometry', 'fragment'];

const cfg = () => vscode.workspace.getConfiguration('eclipsedGlsl');

// ---------------------------------------------------------------- stage helpers
function stageFromPath(p) {
  const ext = path.extname(p).toLowerCase();
  for (const s of STAGES) if (STAGE_INFO[s].exts.includes(ext)) return s;
  return null;
}
function inferStage(text) {
  if (/\blocal_size_[xyz]\b/.test(text)) return 'compute';
  if (/\b(EmitVertex|EndPrimitive)\s*\(/.test(text)) return 'geometry';
  if (/\bgl_TessCoord\b/.test(text)) return 'tesseval';
  if (/\bgl_TessLevel(Outer|Inner)\s*\[/.test(text)) return 'tesscontrol';
  if (/\bgl_Position\s*=/.test(text)) return 'vertex';
  return 'fragment';
}
function stageOf(doc) {
  const s = stageFromPath(doc.uri.fsPath);
  if (s) return s;
  const def = cfg().get('defaultStage', 'auto');
  return def !== 'auto' && STAGE_INFO[def] ? def : inferStage(doc.getText());
}
function stemOf(p) { return p.slice(0, p.length - path.extname(p).length); }
function siblingFile(fsPath, stage) {
  const stem = stemOf(fsPath);
  for (const ext of STAGE_INFO[stage].exts) {
    const f = stem + ext;
    if (f.toLowerCase() !== fsPath.toLowerCase() && fs.existsSync(f)) return f;
  }
  return null;
}
function adjacentStage(fsPath, stage, dir) {
  const i = PIPELINE.indexOf(stage);
  if (i < 0) return null;
  for (let k = i + dir; k >= 0 && k < PIPELINE.length; k += dir) {
    const f = siblingFile(fsPath, PIPELINE[k]);
    if (f) return { stage: PIPELINE[k], file: f };
  }
  return null;
}
function allSiblings(fsPath, stage) {
  if (stage === 'compute') return [];
  return PIPELINE.filter(s => s !== stage).map(s => ({ stage: s, file: siblingFile(fsPath, s) })).filter(x => x.file);
}

// ---------------------------------------------------------------- models
const docCache = new Map();
function getModel(doc) {
  const k = doc.uri.toString();
  const c = docCache.get(k);
  if (c && c.version === doc.version) return c.model;
  const model = P.parse(doc.getText());
  docCache.set(k, { version: doc.version, model });
  return model;
}
function lineStarts(text) {
  const r = [0];
  for (let i = 0; i < text.length; i++) if (text[i] === '\n') r.push(i + 1);
  return r;
}
function offToPos(ls, off) {
  let lo = 0, hi = ls.length - 1;
  while (lo < hi) { const mid = (lo + hi + 1) >> 1; if (ls[mid] <= off) lo = mid; else hi = mid - 1; }
  return new vscode.Position(lo, off - ls[lo]);
}
const fileCache = new Map();
function loadFile(fsPath) {
  const open = vscode.workspace.textDocuments.find(d => d.uri.scheme === 'file' && d.uri.fsPath.toLowerCase() === fsPath.toLowerCase());
  if (open) return { uri: open.uri, model: getModel(open), positionAt: o => open.positionAt(o), name: path.basename(fsPath) };
  let st;
  try { st = fs.statSync(fsPath); } catch (e) { return null; }
  const c = fileCache.get(fsPath);
  if (c && c.mtime === st.mtimeMs) return c.entry;
  const text = fs.readFileSync(fsPath, 'utf8');
  const ls = lineStarts(text);
  const entry = { uri: vscode.Uri.file(fsPath), model: P.parse(text), positionAt: o => offToPos(ls, o), name: path.basename(fsPath) };
  fileCache.set(fsPath, { mtime: st.mtimeMs, entry });
  return entry;
}
function rangeAt(positionAt, off, len) { return new vscode.Range(positionAt(off), positionAt(off + len)); }
function docRange(doc, off, len) { return rangeAt(o => doc.positionAt(o), off, len); }
function globalIO(model, storage) { return model.variables.filter(v => !v.scope && !v.block && v.storage === storage); }

// ---------------------------------------------------------------- symbol resolution
function resolveAt(model, start, end, word) {
  const t = model.clean;
  let k = start - 1;
  while (k >= 0 && /\s/.test(t[k])) k--;
  const isCall = /^\s*\(/.test(t.slice(end, end + 64));
  if (k >= 0 && t[k] === '.') {
    const parts = P.extractChain(t, k);
    if (!parts) return null;
    const base = P.resolveChain(model, parts, start);
    const mt = P.memberType(model, base, word, isCall);
    if (!mt) return null;
    if (mt.field) return { kind: 'field', field: mt.field, owner: mt.owner, type: mt.type };
    if (mt.swizzle) return { kind: 'swizzle', type: mt.type, base };
    if (mt.method) return { kind: 'method', type: 'int' };
    return null;
  }
  const fns = model.functions.filter(f => f.name === word);
  const v = P.lookupVar(model, word, end);
  if (v && !(isCall && fns.length)) return { kind: 'var', v };
  if (model.types.has(word)) return { kind: 'type', t: model.types.get(word) };
  if (fns.length) return { kind: 'func', fns, name: word };
  const mac = model.macros.find(x => x.name === word);
  if (mac) return { kind: 'macro', m: mac };
  if (B.FUNCS[word]) return { kind: 'bfunc', name: word, def: B.FUNCS[word] };
  if (B.VARS[word]) return { kind: 'bvar', name: word, def: B.VARS[word] };
  if (B.KEYWORD_DOCS[word] || B.KEYWORD_SET.has(word) || B.LAYOUT_QUALIFIERS[word]) return { kind: 'keyword', name: word };
  return null;
}
function sameTarget(a, b) {
  if (!a || !b || a.kind !== b.kind) return false;
  switch (a.kind) {
    case 'var': return a.v === b.v;
    case 'field': return a.field === b.field;
    case 'func': return a.name === b.name;
    case 'type': return a.t === b.t;
    case 'macro': return a.m === b.m;
    default: return false;
  }
}
function targetName(t) {
  switch (t.kind) {
    case 'var': return t.v.name; case 'field': return t.field.name; case 'func': return t.name;
    case 'type': return t.t.name; case 'macro': return t.m.name; default: return null;
  }
}
// all offsets in a model that refer to target
function occurrences(model, target) {
  const name = targetName(target);
  if (!name) return [];
  const res = new Set();
  const re = /\b[A-Za-z_]\w*\b/g;
  let m;
  while ((m = re.exec(model.clean))) {
    if (m[0] !== name) continue;
    if (sameTarget(resolveAt(model, m.index, m.index + name.length, name), target)) res.add(m.index);
  }
  if (target.kind === 'field' && target.field.offset != null) res.add(target.field.offset);
  if (target.kind === 'macro') res.add(target.m.offset);
  if (target.kind === 'type') res.add(target.t.offset);
  if (target.kind === 'func') for (const f of target.fns) res.add(f.offset);
  return [...res].sort((a, b) => a - b);
}
function wordAt(doc, pos) {
  const range = doc.getWordRangeAtPosition(pos, /[A-Za-z_]\w*/);
  if (!range) return null;
  const model = getModel(doc);
  const start = doc.offsetAt(range.start), end = doc.offsetAt(range.end);
  if (model.clean[start] !== model.text[start]) return null; // inside a comment / preprocessor line
  return { range, start, end, word: doc.getText(range), model };
}
// sibling-stage counterpart of a global in/out variable
function linkedVars(doc, v) {
  if (v.scope || v.block || doc.uri.scheme !== 'file') return [];
  const stage = stageOf(doc);
  const out = [];
  if (v.storage === 'in' && stage !== 'vertex') {
    const prev = adjacentStage(doc.uri.fsPath, stage, -1);
    const e = prev && loadFile(prev.file);
    if (e) for (const o of globalIO(e.model, 'out')) if (o.name === v.name) out.push({ entry: e, v: o });
  }
  if (v.storage === 'out' && stage !== 'fragment') {
    const next = adjacentStage(doc.uri.fsPath, stage, +1);
    const e = next && loadFile(next.file);
    if (e) for (const o of globalIO(e.model, 'in')) if (o.name === v.name) out.push({ entry: e, v: o });
  }
  return out;
}

// ---------------------------------------------------------------- markdown helpers
function declText(v) {
  let s = '';
  if (v.layout) s += 'layout(' + v.layout + ') ';
  if (v.qualifiers.length && !v.blockMember) s += v.qualifiers.join(' ') + ' ';
  return s + v.type + ' ' + v.name + v.array;
}
function sigsMarkdown(name, def) {
  const md = new vscode.MarkdownString();
  md.appendCodeblock(def.sigs.join('\n'), 'glsl');
  md.appendMarkdown(def.doc);
  if (def.stages) md.appendMarkdown('\n\n*Stages: ' + def.stages.split('').map(l => STAGES.find(s => STAGE_INFO[s].letter === l)).map(s => STAGE_INFO[s].label).join(', ') + '*');
  return md;
}
function varMarkdown(doc, model, v) {
  const md = new vscode.MarkdownString();
  md.appendCodeblock(declText(v), 'glsl');
  if (v.blockMember) md.appendMarkdown('Member of ' + v.block.storage + ' block **' + v.block.name + '**' + (v.block.binding != null ? ' (binding = ' + v.block.binding + ')' : '') + '\n\n');
  if (v.isParam) md.appendMarkdown('Parameter of **' + v.func.name + '**\n\n');
  const st = model.types.get(v.type);
  if (st && st.kind === 'struct') md.appendCodeblock(model.text.slice(st.start, st.end), 'glsl');
  for (const l of linkedVars(doc, v)) {
    md.appendMarkdown((v.storage === 'in' ? 'Written by ' : 'Read by ') + '**' + l.entry.name + '**: ');
    md.appendCodeblock(declText(l.v), 'glsl');
  }
  return md;
}

// ---------------------------------------------------------------- completion
const staticCache = new Map();
function staticItems(letter) {
  if (staticCache.has(letter)) return staticCache.get(letter);
  const items = [];
  for (const k of B.KEYWORDS) {
    const it = new vscode.CompletionItem(k, vscode.CompletionItemKind.Keyword);
    if (B.KEYWORD_DOCS[k]) it.documentation = B.KEYWORD_DOCS[k];
    it.sortText = '4' + k;
    items.push(it);
  }
  for (const t of B.TYPES) {
    const it = new vscode.CompletionItem(t, vscode.CompletionItemKind.TypeParameter);
    it.detail = 'built-in type';
    it.sortText = '3' + t;
    items.push(it);
  }
  for (const name of Object.keys(B.FUNCS)) {
    const def = B.FUNCS[name];
    if (!B.stageOk(def, letter)) continue;
    const it = new vscode.CompletionItem(name, vscode.CompletionItemKind.Function);
    it.detail = def.sigs[0] + (def.sigs.length > 1 ? '  (+' + (def.sigs.length - 1) + ' overloads)' : '');
    it.documentation = sigsMarkdown(name, def);
    it.insertText = new vscode.SnippetString(name + '($0)');
    it.command = { command: 'editor.action.triggerParameterHints', title: 'Signature help' };
    it.sortText = '2' + name;
    items.push(it);
  }
  for (const name of Object.keys(B.VARS)) {
    const def = B.VARS[name];
    if (!B.stageOk(def, letter)) continue;
    const it = new vscode.CompletionItem(name, def.constant ? vscode.CompletionItemKind.Constant : vscode.CompletionItemKind.Variable);
    it.detail = def.type + def.array;
    it.documentation = def.doc;
    it.sortText = '2' + name;
    items.push(it);
  }
  staticCache.set(letter, items);
  return items;
}
function simpleItems(list, kind, docs) {
  return list.map(l => {
    const it = new vscode.CompletionItem(l, kind);
    if (docs && docs[l]) it.documentation = docs[l];
    return it;
  });
}
function memberItems(model, cur) {
  const items = [];
  if (!cur) return items;
  if (cur.dims > 0) {
    const it = new vscode.CompletionItem('length', vscode.CompletionItemKind.Method);
    it.insertText = 'length()';
    it.detail = 'int length()';
    return [it];
  }
  const st = model.types.get(cur.type) || B.BUILTIN_STRUCTS[cur.type];
  if (st) {
    st.fields.forEach((f, i) => {
      const it = new vscode.CompletionItem(f.name, vscode.CompletionItemKind.Field);
      it.detail = f.type + ' ' + f.name + f.array;
      it.documentation = 'Member of ' + st.name;
      it.sortText = String(i).padStart(3, '0');
      items.push(it);
    });
    return items;
  }
  const vi = B.vecInfo(cur.type);
  if (vi) {
    const sets = ['xyzw', 'rgba', 'stpq'];
    const seen = new Set();
    const add = (s, sort) => {
      if (seen.has(s) || !B.isSwizzle(s, vi.size)) return;
      seen.add(s);
      const it = new vscode.CompletionItem(s, vscode.CompletionItemKind.Property);
      it.detail = B.vecType(vi.scalar, s.length);
      it.sortText = sort + s;
      items.push(it);
    };
    for (let si = 0; si < sets.length; si++) {
      const set = sets[si].slice(0, vi.size);
      for (const ch of set) add(ch, si + 'a');
      for (let n = 2; n <= vi.size; n++) add(set.slice(0, n), si + 'b');
    }
    add('yx', '3'); add('zw', '3'); add('xz', '3'); add('xxx', '3');
  }
  return items;
}

function provideCompletion(doc, pos) {
  const model = getModel(doc);
  const off = doc.offsetAt(pos);
  const line = doc.lineAt(pos.line).text.slice(0, pos.character);
  const letter = STAGE_INFO[stageOf(doc)].letter;

  // preprocessor
  if (/^\s*#\s*\w*$/.test(line)) return simpleItems(B.DIRECTIVES, vscode.CompletionItemKind.Keyword);
  if (/^\s*#\s*version\s+[\w ]*$/.test(line)) return B.VERSIONS.map((v, i) => { const it = new vscode.CompletionItem(v, vscode.CompletionItemKind.EnumMember); it.sortText = String(i).padStart(2, '0'); return it; });
  if (/^\s*#\s*extension\s+\w*$/.test(line)) return simpleItems(B.EXTENSIONS, vscode.CompletionItemKind.Module);
  if (/^\s*#\s*extension\s+\w+\s*:\s*\w*$/.test(line)) return simpleItems(['require', 'enable', 'warn', 'disable'], vscode.CompletionItemKind.Keyword);
  if (/^\s*#/.test(line)) return [];
  // inside a comment
  if (off > 0 && model.text[off - 1] !== model.clean[off - 1] && !/\s/.test(model.text[off - 1])) return [];
  if (line.includes('//') && model.clean.slice(off - line.length, off).indexOf('/') !== line.indexOf('//')) return [];

  // member access
  const mm = /\.\s*(\w*)$/.exec(line);
  if (mm) {
    if (/(^|[^\w.])\d+\.\s*\w*$/.test(line)) return []; // float literal
    const dot = off - mm[0].length;
    const parts = P.extractChain(model.clean, dot);
    if (!parts) return [];
    return memberItems(model, P.resolveChain(model, parts, dot));
  }

  // layout(...)
  if (/\blayout\s*\([^)]*$/.test(line)) {
    return Object.keys(B.LAYOUT_QUALIFIERS).map(q => {
      const it = new vscode.CompletionItem(q, vscode.CompletionItemKind.Property);
      it.documentation = B.LAYOUT_QUALIFIERS[q];
      if (B.LAYOUT_WITH_VALUE.has(q)) it.insertText = new vscode.SnippetString(q + ' = ').appendPlaceholder('0');
      return it;
    });
  }

  const items = [];
  // "in " in a later stage: offer the previous stage's outputs as declarations
  const inDecl = /^\s*(?:(?:flat|smooth|noperspective|centroid)\s+)*in\s+\w*$/.exec(line);
  if (inDecl && letter !== 'v' && doc.uri.scheme === 'file') {
    const prev = adjacentStage(doc.uri.fsPath, stageOf(doc), -1);
    const e = prev && loadFile(prev.file);
    if (e) {
      const declared = new Set(globalIO(model, 'in').map(v => v.name));
      const arrayed = letter === 'g' || letter === 't' || letter === 'e';
      for (const o of globalIO(e.model, 'out')) {
        if (declared.has(o.name)) continue;
        const decl = o.type + ' ' + o.name + (arrayed ? '[]' : o.array) + ';';
        const it = new vscode.CompletionItem(decl, vscode.CompletionItemKind.Interface);
        it.detail = 'output of ' + e.name;
        it.filterText = o.type + ' ' + o.name;
        it.sortText = '0' + o.name;
        items.push(it);
      }
    }
  }

  for (const st of model.structs.values()) {
    const it = new vscode.CompletionItem(st.name, vscode.CompletionItemKind.Struct);
    it.detail = 'struct ' + st.name;
    it.documentation = new vscode.MarkdownString().appendCodeblock(model.text.slice(st.start, st.end), 'glsl');
    it.sortText = '1' + st.name;
    items.push(it);
  }
  const seenFn = new Set();
  for (const f of model.functions) {
    if (seenFn.has(f.name) || f.name === 'main') continue;
    seenFn.add(f.name);
    const it = new vscode.CompletionItem(f.name, vscode.CompletionItemKind.Function);
    it.detail = f.signature;
    it.insertText = new vscode.SnippetString(f.name + '(' + (f.params.length ? '$0' : '') + ')');
    if (f.params.length) it.command = { command: 'editor.action.triggerParameterHints', title: 'Signature help' };
    it.sortText = '1' + f.name;
    items.push(it);
  }
  const seenVar = new Set();
  const vis = P.visibleVars(model, off).sort((a, b) => (b.scope ? b.scope.open : -1) - (a.scope ? a.scope.open : -1));
  for (const v of vis) {
    if (seenVar.has(v.name)) continue;
    seenVar.add(v.name);
    const kind = v.blockMember ? vscode.CompletionItemKind.Field : (v.storage === 'uniform' || v.storage === 'const') ? vscode.CompletionItemKind.Constant : vscode.CompletionItemKind.Variable;
    const it = new vscode.CompletionItem(v.name, kind);
    it.detail = declText(v);
    if (v.blockMember) it.documentation = v.block.storage + ' block ' + v.block.name;
    it.sortText = (v.scope ? '0' : '1') + v.name;
    items.push(it);
  }
  for (const mac of model.macros) {
    const it = new vscode.CompletionItem(mac.name, vscode.CompletionItemKind.Constant);
    it.detail = '#define ' + mac.name + mac.params + ' ' + mac.value;
    it.sortText = '1' + mac.name;
    items.push(it);
  }
  return items.concat(staticItems(letter));
}

// ---------------------------------------------------------------- signature help
function provideSignatureHelp(doc, pos) {
  const model = getModel(doc);
  const t = model.clean;
  const off = doc.offsetAt(pos);
  let depth = 0, commas = 0, i = off - 1;
  for (; i >= 0; i--) {
    const c = t[i];
    if (c === ')' || c === ']') depth++;
    else if (c === '(' || c === '[') { if (depth === 0) break; depth--; }
    else if (c === ',' && depth === 0) commas++;
    else if (c === ';' || c === '{' || c === '}') return null;
  }
  if (i < 0 || t[i] !== '(') return null;
  let j = i - 1;
  while (j >= 0 && /\s/.test(t[j])) j--;
  const e = j + 1;
  while (j >= 0 && /\w/.test(t[j])) j--;
  const name = t.slice(j + 1, e);
  if (!name) return null;

  const sigs = [];
  const build = (label, paramLabels, docText) => {
    const si = new vscode.SignatureInformation(label, docText);
    let from = label.indexOf('(') + 1;
    for (const pl of paramLabels) {
      const s = label.indexOf(pl, from);
      if (s < 0) continue;
      si.parameters.push(new vscode.ParameterInformation([s, s + pl.length]));
      from = s + pl.length;
    }
    return si;
  };
  for (const f of model.functions) if (f.name === name) sigs.push({ n: f.params.length, si: build(f.signature, f.params.map(p => p.label)) });
  if (!sigs.length && B.FUNCS[name]) {
    for (const s of B.FUNCS[name].sigs) {
      const inner = s.slice(s.indexOf('(') + 1, s.lastIndexOf(')'));
      const params = inner.trim() ? inner.split(',').map(x => x.trim()) : [];
      sigs.push({ n: params.length, si: build(s, params, B.FUNCS[name].doc) });
    }
  }
  if (!sigs.length) return null;
  const help = new vscode.SignatureHelp();
  help.signatures = sigs.map(s => s.si);
  const active = sigs.findIndex(s => s.n > commas);
  help.activeSignature = active >= 0 ? active : 0;
  help.activeParameter = commas;
  return help;
}

// ---------------------------------------------------------------- hover
function provideHover(doc, pos) {
  const w = wordAt(doc, pos);
  if (!w) return null;
  const { model, word, start, end, range } = w;
  const r = resolveAt(model, start, end, word);
  if (!r) return null;
  let md = new vscode.MarkdownString();
  switch (r.kind) {
    case 'field':
      md.appendCodeblock(r.field.type + ' ' + r.field.name + r.field.array, 'glsl');
      md.appendMarkdown('Member of ' + (r.owner.kind === 'block' ? r.owner.storage + ' block' : 'struct') + ' **' + r.owner.name + '**');
      break;
    case 'swizzle': md.appendCodeblock(r.type + '  // swizzle of ' + r.base.type, 'glsl'); break;
    case 'method': md.appendCodeblock('int length()', 'glsl'); break;
    case 'var': md = varMarkdown(doc, model, r.v); break;
    case 'type': md.appendCodeblock(model.text.slice(r.t.start, r.t.end).split(/\r?\n/).slice(0, 40).join('\n'), 'glsl'); break;
    case 'func': md.appendCodeblock(r.fns.map(f => f.signature).join('\n'), 'glsl'); break;
    case 'macro': md.appendCodeblock('#define ' + r.m.name + r.m.params + ' ' + r.m.value, 'glsl'); break;
    case 'bfunc': md = sigsMarkdown(r.name, r.def); break;
    case 'bvar':
      md.appendCodeblock(r.def.type + ' ' + r.name + r.def.array, 'glsl');
      md.appendMarkdown(r.def.doc);
      break;
    case 'keyword': {
      const d = B.KEYWORD_DOCS[word] || B.LAYOUT_QUALIFIERS[word];
      if (!d) return null;
      md.appendMarkdown('**' + word + '** — ' + d);
      break;
    }
  }
  return new vscode.Hover(md, range);
}

// ---------------------------------------------------------------- definition / references / rename / highlights
function locationsFor(doc, r) {
  const here = (off, len) => new vscode.Location(doc.uri, docRange(doc, off, len));
  switch (r.kind) {
    case 'var': {
      const locs = [here(r.v.offset, r.v.name.length)];
      for (const l of linkedVars(doc, r.v)) locs.push(new vscode.Location(l.entry.uri, rangeAt(l.entry.positionAt, l.v.offset, l.v.name.length)));
      return locs;
    }
    case 'field': return r.field.offset != null ? [here(r.field.offset, r.field.name.length)] : [];
    case 'type': return [here(r.t.offset, r.t.name.length)];
    case 'func': {
      const defs = r.fns.filter(f => !f.isPrototype);
      return (defs.length ? defs : r.fns).map(f => here(f.offset, f.name.length));
    }
    case 'macro': return [here(r.m.offset, r.m.name.length)];
    default: return [];
  }
}
function provideDefinition(doc, pos) {
  const w = wordAt(doc, pos);
  if (!w) return null;
  const r = resolveAt(w.model, w.start, w.end, w.word);
  return r ? locationsFor(doc, r) : null;
}
function refsFor(doc, w, includeLinked) {
  const r = resolveAt(w.model, w.start, w.end, w.word);
  if (!r || !targetName(r)) return null;
  const res = occurrences(w.model, r).map(o => ({ uri: doc.uri, range: docRange(doc, o, w.word.length) }));
  if (includeLinked && r.kind === 'var') {
    for (const l of linkedVars(doc, r.v)) {
      for (const o of occurrences(l.entry.model, { kind: 'var', v: l.v })) res.push({ uri: l.entry.uri, range: rangeAt(l.entry.positionAt, o, w.word.length) });
    }
  }
  return { r, res };
}
function provideReferences(doc, pos) {
  const w = wordAt(doc, pos);
  if (!w) return null;
  const x = refsFor(doc, w, true);
  return x ? x.res.map(o => new vscode.Location(o.uri, o.range)) : null;
}
function provideHighlights(doc, pos) {
  const w = wordAt(doc, pos);
  if (!w) return null;
  const x = refsFor(doc, w, false);
  return x ? x.res.map(o => new vscode.DocumentHighlight(o.range)) : null;
}
function prepareRename(doc, pos) {
  const w = wordAt(doc, pos);
  if (!w) throw new Error('Nothing to rename here.');
  const r = resolveAt(w.model, w.start, w.end, w.word);
  if (!r || !targetName(r)) throw new Error('Built-in symbols cannot be renamed.');
  return w.range;
}
function provideRename(doc, pos, newName) {
  if (!/^[A-Za-z_]\w*$/.test(newName)) throw new Error('Not a valid GLSL identifier.');
  const w = wordAt(doc, pos);
  if (!w) return null;
  const x = refsFor(doc, w, true);
  if (!x) return null;
  const edit = new vscode.WorkspaceEdit();
  for (const o of x.res) edit.replace(o.uri, o.range, newName);
  return edit;
}

// ---------------------------------------------------------------- outline, folding, semantic tokens
function provideSymbols(doc) {
  const model = getModel(doc);
  const S = vscode.SymbolKind;
  const sym = (name, detail, kind, start, end, nameOff) => new vscode.DocumentSymbol(name, detail, kind,
    docRange(doc, start, Math.max(1, end - start)), docRange(doc, nameOff, name.length));
  const out = [];
  for (const mac of model.macros) out.push(sym(mac.name, mac.value, S.Constant, mac.offset, mac.offset + mac.name.length, mac.offset));
  for (const st of model.structs.values()) {
    const s = sym(st.name, 'struct', S.Struct, st.start, st.end, st.offset);
    s.children = st.fields.map(f => sym(f.name, f.type + f.array, S.Field, f.offset, f.offset + f.name.length, f.offset));
    out.push(s);
  }
  for (const b of model.blocks) {
    const s = sym(b.name, b.storage + (b.binding != null ? ' (binding ' + b.binding + ')' : ''), S.Interface, b.start, b.end, b.offset);
    s.children = b.fields.map(f => sym(f.name, f.type + f.array, S.Field, f.offset, f.offset + f.name.length, f.offset));
    out.push(s);
  }
  for (const v of model.variables) {
    if (v.scope || v.blockMember || (v.block && v.block.instance === v.name)) continue;
    out.push(sym(v.name, (v.storage ? v.storage + ' ' : '') + v.type + v.array, v.storage === 'uniform' || v.storage === 'const' ? S.Constant : S.Variable, v.offset, v.offset + v.name.length, v.offset));
  }
  for (const f of model.functions) out.push(sym(f.name, f.signature, S.Function, f.start, f.end, f.offset));
  out.sort((a, b) => a.range.start.compareTo(b.range.start));
  return out;
}
function provideFolding(doc) {
  const model = getModel(doc);
  const ranges = [];
  for (const p of model.pairs) {
    const a = doc.positionAt(p.open).line, b = doc.positionAt(Math.min(p.close, model.text.length)).line;
    if (b > a) ranges.push(new vscode.FoldingRange(a, b - 1));
  }
  const stack = [];
  for (let i = 0; i < doc.lineCount; i++) {
    const t = doc.lineAt(i).text;
    if (/^\s*\/\/\s*#?region\b/.test(t)) stack.push(i);
    else if (/^\s*\/\/\s*#?endregion\b/.test(t) && stack.length) ranges.push(new vscode.FoldingRange(stack.pop(), i, vscode.FoldingRangeKind.Region));
  }
  const cm = /\/\*[\s\S]*?\*\//g;
  let m;
  while ((m = cm.exec(model.text))) {
    const a = doc.positionAt(m.index).line, b = doc.positionAt(m.index + m[0].length).line;
    if (b > a) ranges.push(new vscode.FoldingRange(a, b, vscode.FoldingRangeKind.Comment));
  }
  return ranges;
}
const TOKEN_TYPES = ['struct', 'interface', 'function', 'macro', 'parameter', 'property', 'variable'];
const TOKEN_MODS = ['declaration', 'readonly'];
const legend = new vscode.SemanticTokensLegend(TOKEN_TYPES, TOKEN_MODS);
function provideSemanticTokens(doc) {
  const model = getModel(doc);
  const toks = [];
  const re = /\b[A-Za-z_]\w*\b/g;
  const fnNames = new Set(model.functions.map(f => f.name));
  const macNames = new Set(model.macros.map(x => x.name));
  let m;
  while ((m = re.exec(model.clean))) {
    const w = m[0];
    if (B.KEYWORD_SET.has(w) || w.startsWith('gl_')) continue;
    let type = -1, mods = 0;
    const st = model.types.get(w);
    if (st) type = st.kind === 'block' ? 1 : 0;
    else if (macNames.has(w)) type = 3;
    else {
      let k = m.index - 1;
      while (k >= 0 && /\s/.test(model.clean[k])) k--;
      if (k >= 0 && model.clean[k] === '.') {
        const r = resolveAt(model, m.index, m.index + w.length, w);
        if (r && r.kind === 'field') type = 5;
      } else if (fnNames.has(w) && /^\s*\(/.test(model.clean.slice(m.index + w.length, m.index + w.length + 32))) type = 2;
      else {
        const v = P.lookupVar(model, w, m.index + w.length);
        if (v) {
          type = v.isParam ? 4 : v.blockMember ? 5 : 6;
          if (v.storage === 'uniform' || v.storage === 'const') mods |= 2;
        }
      }
    }
    if (type >= 0) toks.push([m.index, w.length, type, mods]);
  }
  for (const mac of model.macros) toks.push([mac.offset, mac.name.length, 3, 1]);
  toks.sort((a, b) => a[0] - b[0]);
  const builder = new vscode.SemanticTokensBuilder(legend);
  for (const t of toks) { const p = doc.positionAt(t[0]); builder.push(p.line, p.character, t[1], t[2], t[3]); }
  return builder.build();
}

// ---------------------------------------------------------------- diagnostics
let diagCollection, glslangCollection, statusItem;
let validatorState = 'unknown'; // ok | missing | disabled
let validatorMissingNotified = false;

function nameRange(doc, v) { return docRange(doc, v.offset, v.name.length); }
function mk(range, msg, sev, tags) {
  const d = new vscode.Diagnostic(range, msg, sev);
  d.source = 'eclipsed-glsl';
  if (tags) d.tags = tags;
  return d;
}
function analyze(doc) {
  if (doc.languageId !== LANG) return;
  const model = getModel(doc);
  const stage = stageOf(doc);
  const W = vscode.DiagnosticSeverity.Warning, I = vscode.DiagnosticSeverity.Information, H = vscode.DiagnosticSeverity.Hint;
  const diags = [];
  if (validatorState !== 'ok') {
    for (const e of model.errors) diags.push(mk(docRange(doc, e.offset, 1), e.message, vscode.DiagnosticSeverity.Error));
    if (!model.version && model.text.trim()) diags.push(mk(docRange(doc, 0, 0), 'Missing #version directive.', W));
  }
  if (cfg().get('linkage.enabled', true) && doc.uri.scheme === 'file') {
    const fsPath = doc.uri.fsPath;
    const arrayedIn = stage === 'geometry' || stage === 'tesscontrol' || stage === 'tesseval';
    if (stage !== 'vertex' && stage !== 'compute') {
      const prev = adjacentStage(fsPath, stage, -1);
      const e = prev && loadFile(prev.file);
      if (e) {
        const outs = globalIO(e.model, 'out');
        for (const v of globalIO(model, 'in')) {
          const o = outs.find(o => o.name === v.name);
          if (!o) { diags.push(mk(nameRange(doc, v), "'" + v.name + "' is not written by the previous stage (" + e.name + ').', W)); continue; }
          const a = v.type + (arrayedIn ? '' : v.array), b = o.type + (arrayedIn ? '' : o.array);
          if (a !== b) diags.push(mk(nameRange(doc, v), "Type mismatch: '" + v.name + "' is " + a + ' here but ' + b + ' in ' + e.name + '.', W));
          if (stage === 'fragment' && /^(u?int|[iu]vec\d)$/.test(v.type) && !v.qualifiers.includes('flat'))
            diags.push(mk(nameRange(doc, v), "Integer input '" + v.name + "' must be declared 'flat' in a pixel shader.", W));
        }
      }
    }
    if (stage !== 'fragment' && stage !== 'compute') {
      const next = adjacentStage(fsPath, stage, +1);
      const e = next && loadFile(next.file);
      if (e) {
        const ins = new Set(globalIO(e.model, 'in').map(v => v.name));
        for (const v of globalIO(model, 'out')) if (!ins.has(v.name))
          diags.push(mk(nameRange(doc, v), "'" + v.name + "' is never read by the next stage (" + e.name + ').', H, [vscode.DiagnosticTag.Unnecessary]));
      }
    }
    // uniform / buffer blocks shared across the stages of one program
    for (const sib of allSiblings(fsPath, stage)) {
      const e = loadFile(sib.file);
      if (!e) continue;
      for (const b of model.blocks) {
        if (b.storage !== 'uniform' && b.storage !== 'buffer') continue;
        const r = docRange(doc, b.offset, b.name.length);
        const same = e.model.blocks.find(o => o.name === b.name && o.storage === b.storage);
        if (same) {
          const sig = x => x.fields.map(f => f.type + ' ' + f.name + f.array).join('; ');
          if (sig(b) !== sig(same)) diags.push(mk(r, "Block '" + b.name + "' has different members in " + e.name + ': ' + sig(same), W));
          if (b.binding !== same.binding) diags.push(mk(r, "Block '" + b.name + "' uses binding " + b.binding + ' here but ' + same.binding + ' in ' + e.name + '.', W));
        } else if (b.binding != null) {
          const clash = e.model.blocks.find(o => o.storage === b.storage && o.binding === b.binding && o.name !== b.name);
          if (clash) diags.push(mk(r, 'Binding ' + b.binding + " is also used by '" + clash.name + "' in " + e.name + '.', I));
        }
      }
    }
  }
  diagCollection.set(doc.uri, diags);
}

function validatorPath() {
  const c = cfg().get('validator.path', '');
  if (c) return c;
  const exe = 'glslangValidator' + (process.platform === 'win32' ? '.exe' : '');
  const sdk = process.env.VULKAN_SDK;
  if (sdk) for (const dir of ['Bin', 'bin']) { const p = path.join(sdk, dir, exe); if (fs.existsSync(p)) return p; }
  return 'glslangValidator';
}
function setValidatorState(s) {
  if (validatorState === s) return;
  validatorState = s;
  updateStatus();
  for (const d of vscode.workspace.textDocuments) analyze(d);
}
function parseGlslang(out, doc) {
  const diags = [];
  for (const raw of out.split(/\r?\n/)) {
    const m = /^(ERROR|WARNING):\s*(?:[^:\s]*:)?(\d+):\s*(.*)$/.exec(raw.trim());
    if (!m) continue;
    const ln = Math.max(0, Math.min(doc.lineCount - 1, +m[2] - 1));
    const lt = doc.lineAt(ln);
    let range = new vscode.Range(ln, lt.firstNonWhitespaceCharacterIndex, ln, lt.text.length);
    const q = /'([^']+)'/.exec(m[3]);
    if (q && q[1].trim()) {
      const idx = lt.text.indexOf(q[1].trim());
      if (idx >= 0) range = new vscode.Range(ln, idx, ln, idx + q[1].trim().length);
    }
    const d = new vscode.Diagnostic(range, m[3], m[1] === 'ERROR' ? vscode.DiagnosticSeverity.Error : vscode.DiagnosticSeverity.Warning);
    d.source = 'glslang';
    diags.push(d);
  }
  return diags;
}
const running = new Map();
function runValidator(doc) {
  if (doc.languageId !== LANG) return;
  if (!cfg().get('validator.enabled', true)) { glslangCollection.delete(doc.uri); setValidatorState('disabled'); return; }
  if (validatorState === 'missing') return;
  const key = doc.uri.toString();
  const old = running.get(key);
  if (old) { try { old.kill(); } catch (e) { /* ignore */ } }
  const version = doc.version;
  let child;
  try {
    child = cp.spawn(validatorPath(), ['--stdin', '-S', STAGE_INFO[stageOf(doc)].glslang], { windowsHide: true });
  } catch (e) { setValidatorState('missing'); return; }
  running.set(key, child);
  let out = '';
  child.stdout.on('data', d => { out += d; });
  child.stderr.on('data', d => { out += d; });
  child.stdin.on('error', () => {});
  const timer = setTimeout(() => { try { child.kill(); } catch (e) { /* ignore */ } }, 10000);
  child.on('error', e => {
    clearTimeout(timer);
    if (running.get(key) === child) running.delete(key);
    if (e.code === 'ENOENT') {
      setValidatorState('missing');
      if (!validatorMissingNotified && cfg().get('validator.path', '')) {
        validatorMissingNotified = true;
        vscode.window.showWarningMessage('Eclipsed GLSL: glslangValidator not found at "' + cfg().get('validator.path') + '".');
      }
    }
  });
  child.on('close', () => {
    clearTimeout(timer);
    if (running.get(key) !== child) return;
    running.delete(key);
    setValidatorState('ok');
    if (doc.isClosed || doc.version !== version) return;
    glslangCollection.set(doc.uri, parseGlslang(out, doc));
  });
  child.stdin.end(doc.getText());
}

// ---------------------------------------------------------------- status bar & commands
function updateStatus() {
  const ed = vscode.window.activeTextEditor;
  if (!ed || ed.document.languageId !== LANG) { statusItem.hide(); return; }
  const stage = stageOf(ed.document);
  let text = '$(symbol-color) ' + STAGE_INFO[stage].label + ' Shader';
  let tip = 'Eclipsed GLSL: ' + STAGE_INFO[stage].label + ' stage. Click to switch to a paired stage.';
  if (validatorState === 'missing') {
    text += ' $(warning)';
    tip += '\n\nglslangValidator was not found, so only basic syntax checks run. Install the Vulkan SDK or set "eclipsedGlsl.validator.path".';
  } else if (validatorState === 'ok') tip += '\n\nValidated with glslang.';
  statusItem.text = text;
  statusItem.tooltip = tip;
  statusItem.show();
}
async function openPaired() {
  const ed = vscode.window.activeTextEditor;
  if (!ed || ed.document.uri.scheme !== 'file') return;
  const stage = stageOf(ed.document);
  const sibs = allSiblings(ed.document.uri.fsPath, stage);
  if (!sibs.length) { vscode.window.showInformationMessage('No other shader stages found next to ' + path.basename(ed.document.uri.fsPath) + '.'); return; }
  let pick = sibs[0];
  if (sibs.length > 1) {
    const choice = await vscode.window.showQuickPick(sibs.map(s => ({ label: path.basename(s.file), description: STAGE_INFO[s.stage].label, s })), { placeHolder: 'Open shader stage' });
    if (!choice) return;
    pick = choice.s;
  }
  await vscode.window.showTextDocument(vscode.Uri.file(pick.file));
}
async function newShaderPair(uri) {
  let dir = uri && uri.fsPath;
  if (dir && fs.existsSync(dir) && !fs.statSync(dir).isDirectory()) dir = path.dirname(dir);
  if (!dir) {
    const folder = await vscode.window.showOpenDialog({ canSelectFolders: true, canSelectFiles: false, openLabel: 'Create here' });
    if (!folder) return;
    dir = folder[0].fsPath;
  }
  const name = await vscode.window.showInputBox({ prompt: 'Shader name (creates Name.vglsl and Name.pglsl)', validateInput: v => /^[\w-]+$/.test(v) ? null : 'Letters, digits, _ and - only' });
  if (!name) return;
  const vs = '#version 460 core\n\nlayout(location=0) in vec2 VertexPosition;\nlayout(location=1) in vec2 TexCoord;\n\nout vec2 outTexCoord;\n\nvoid main()\n{\n   outTexCoord = TexCoord;\n   gl_Position = vec4(VertexPosition, 0.0, 1.0);\n}\n';
  const ps = '#version 460 core\n\nin vec2 outTexCoord;\n\nout vec4 frag_colour;\n\nvoid main()\n{\n   frag_colour = vec4(outTexCoord, 0.0, 1.0);\n}\n';
  for (const ext of ['.vglsl', '.pglsl']) {
    if (fs.existsSync(path.join(dir, name + ext))) { vscode.window.showErrorMessage(name + ext + ' already exists.'); return; }
  }
  fs.writeFileSync(path.join(dir, name + '.vglsl'), vs);
  fs.writeFileSync(path.join(dir, name + '.pglsl'), ps);
  await vscode.window.showTextDocument(vscode.Uri.file(path.join(dir, name + '.vglsl')));
}

// ---------------------------------------------------------------- formatting
function fmtOptions(o) {
  const c = cfg();
  return { insertSpaces: o.insertSpaces, tabSize: o.tabSize, spacing: c.get('format.spacing', true),
    braceStyle: c.get('format.braceStyle', 'newLine'), maxBlankLines: c.get('format.maxBlankLines', 2) };
}
function formatEdits(doc, options, fromLine, toLine) {
  const text = doc.getText();
  if (fromLine == null) {
    const out = F.formatDocument(text, fmtOptions(options));
    return out === text ? [] : [vscode.TextEdit.replace(new vscode.Range(doc.positionAt(0), doc.positionAt(text.length)), out)];
  }
  return F.formatRange(text, fmtOptions(options), fromLine, toLine).map(e => vscode.TextEdit.replace(doc.lineAt(e.line).range, e.text));
}

// ---------------------------------------------------------------- activation
function activate(context) {
  const sel = { language: LANG };
  diagCollection = vscode.languages.createDiagnosticCollection('eclipsed-glsl');
  glslangCollection = vscode.languages.createDiagnosticCollection('glslang');
  statusItem = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Right, 100);
  statusItem.command = 'eclipsedGlsl.openPairedShader';

  const timers = new Map();
  const schedule = (doc, delay) => {
    if (doc.languageId !== LANG) return;
    const k = doc.uri.toString();
    clearTimeout(timers.get(k));
    timers.set(k, setTimeout(() => {
      timers.delete(k);
      analyze(doc);
      runValidator(doc);
    }, delay));
  };
  const refreshSiblings = doc => {
    for (const d of vscode.workspace.textDocuments) if (d !== doc && d.languageId === LANG) analyze(d);
  };

  context.subscriptions.push(
    diagCollection, glslangCollection, statusItem,
    vscode.languages.registerCompletionItemProvider(sel, { provideCompletionItems: provideCompletion }, '.', '#'),
    vscode.languages.registerSignatureHelpProvider(sel, { provideSignatureHelp }, '(', ','),
    vscode.languages.registerHoverProvider(sel, { provideHover }),
    vscode.languages.registerDefinitionProvider(sel, { provideDefinition }),
    vscode.languages.registerReferenceProvider(sel, { provideReferences }),
    vscode.languages.registerDocumentHighlightProvider(sel, { provideDocumentHighlights: provideHighlights }),
    vscode.languages.registerRenameProvider(sel, { prepareRename, provideRenameEdits: provideRename }),
    vscode.languages.registerDocumentSymbolProvider(sel, { provideDocumentSymbols: provideSymbols }),
    vscode.languages.registerFoldingRangeProvider(sel, { provideFoldingRanges: provideFolding }),
    vscode.languages.registerDocumentSemanticTokensProvider(sel, { provideDocumentSemanticTokens: provideSemanticTokens }, legend),
    vscode.languages.registerDocumentFormattingEditProvider(sel, { provideDocumentFormattingEdits: (doc, o) => formatEdits(doc, o) }),
    vscode.languages.registerDocumentRangeFormattingEditProvider(sel, { provideDocumentRangeFormattingEdits: (doc, r, o) => formatEdits(doc, o, r.start.line, r.end.line) }),
    vscode.languages.registerOnTypeFormattingEditProvider(sel, { provideOnTypeFormattingEdits: (doc, pos, ch, o) => formatEdits(doc, o, pos.line, pos.line) }, '}'),
    vscode.commands.registerCommand('eclipsedGlsl.openPairedShader', openPaired),
    vscode.commands.registerCommand('eclipsedGlsl.newShaderPair', newShaderPair),
    vscode.commands.registerCommand('eclipsedGlsl.revalidate', () => {
      validatorState = 'unknown';
      for (const d of vscode.workspace.textDocuments) schedule(d, 0);
      updateStatus();
    }),
    vscode.workspace.onDidOpenTextDocument(d => schedule(d, 0)),
    vscode.workspace.onDidChangeTextDocument(e => {
      if (e.document.languageId !== LANG) return;
      if (cfg().get('validator.runOnChange', true)) schedule(e.document, 400);
      else { const k = e.document.uri.toString(); clearTimeout(timers.get(k)); timers.set(k, setTimeout(() => analyze(e.document), 300)); }
    }),
    vscode.workspace.onDidSaveTextDocument(d => { schedule(d, 0); if (d.languageId === LANG) refreshSiblings(d); }),
    vscode.workspace.onDidCloseTextDocument(d => {
      diagCollection.delete(d.uri); glslangCollection.delete(d.uri); docCache.delete(d.uri.toString());
    }),
    vscode.window.onDidChangeActiveTextEditor(updateStatus),
    vscode.workspace.onDidChangeConfiguration(e => {
      if (e.affectsConfiguration('eclipsedGlsl')) {
        validatorState = 'unknown';
        glslangCollection.clear();
        for (const d of vscode.workspace.textDocuments) schedule(d, 0);
      }
    }),
  );
  for (const d of vscode.workspace.textDocuments) schedule(d, 0);
  updateStatus();
}
function deactivate() {
  for (const c of running.values()) { try { c.kill(); } catch (e) { /* ignore */ } }
}
module.exports = { activate, deactivate };

'use strict';
// GLSL 4.60 built-in language data. Stage letters: v=vertex t=tess control e=tess eval g=geometry f=fragment/pixel c=compute

const TYPES = ['void', 'bool', 'int', 'uint', 'float', 'double', 'atomic_uint'];
for (const p of ['', 'b', 'i', 'u', 'd']) for (const n of [2, 3, 4]) TYPES.push(p + 'vec' + n);
for (const p of ['', 'd']) {
  for (const n of [2, 3, 4]) TYPES.push(p + 'mat' + n);
  for (const c of [2, 3, 4]) for (const r of [2, 3, 4]) TYPES.push(p + 'mat' + c + 'x' + r);
}
const DIMS = ['1D', '2D', '3D', 'Cube', '2DRect', '1DArray', '2DArray', 'CubeArray', 'Buffer', '2DMS', '2DMSArray'];
for (const p of ['', 'i', 'u']) for (const d of DIMS) { TYPES.push(p + 'sampler' + d); TYPES.push(p + 'image' + d); }
TYPES.push('sampler1DShadow', 'sampler2DShadow', 'samplerCubeShadow', 'sampler2DRectShadow',
  'sampler1DArrayShadow', 'sampler2DArrayShadow', 'samplerCubeArrayShadow');
const TYPE_SET = new Set(TYPES);

const QUALIFIERS = ['const', 'in', 'out', 'inout', 'uniform', 'buffer', 'shared', 'attribute', 'varying',
  'centroid', 'flat', 'smooth', 'noperspective', 'patch', 'sample', 'invariant', 'precise',
  'highp', 'mediump', 'lowp', 'precision', 'coherent', 'volatile', 'restrict', 'readonly', 'writeonly', 'layout', 'subroutine'];
const CONTROL = ['if', 'else', 'for', 'while', 'do', 'switch', 'case', 'default', 'break', 'continue', 'return', 'discard'];
const OTHER = ['struct', 'true', 'false'];
const KEYWORDS = QUALIFIERS.concat(CONTROL, OTHER);
const KEYWORD_SET = new Set(KEYWORDS.concat(TYPES));

const KEYWORD_DOCS = {
  in: 'Input from the previous pipeline stage (or vertex attribute in a vertex shader).',
  out: 'Output to the next pipeline stage (or a render target in a pixel shader).',
  inout: 'Function parameter copied in and copied back out.',
  uniform: 'Value that is constant across a draw call, set from the application.',
  buffer: 'Shader storage buffer object (SSBO) block. Read/write, may hold an unsized array as last member.',
  shared: 'Variable shared by all invocations of a compute work group.',
  const: 'Compile-time constant or read-only function parameter.',
  flat: 'No interpolation: the value from the provoking vertex is used. Required for integer inputs in a pixel shader.',
  smooth: 'Perspective-correct interpolation (the default).',
  noperspective: 'Linear interpolation in screen space.',
  centroid: 'Interpolate at a sample inside the primitive (multisampling).',
  layout: 'Layout qualifiers: location, binding, std140/std430, local_size_x, primitive types, ...',
  std140: 'Standard uniform block layout. vec3 is aligned like vec4, array elements are padded to 16 bytes.',
  std430: 'Tighter buffer layout (SSBOs only for blocks). Arrays of scalars/vec2 are not padded to 16 bytes.',
  discard: 'Pixel shader only: throws away the current fragment.',
  readonly: 'Memory qualifier: buffer/image can only be read.',
  writeonly: 'Memory qualifier: buffer/image can only be written.',
  coherent: 'Memory qualifier: writes are visible to other invocations.',
  precise: 'Prevents optimizations that change the precision of the result.',
  invariant: 'Output is computed identically across shaders with the same expression.',
};

const LAYOUT_QUALIFIERS = {
  location: 'Input/output location index.', binding: 'Binding point of a block, sampler or image.',
  std140: KEYWORD_DOCS.std140, std430: KEYWORD_DOCS.std430, packed: 'Implementation-defined block layout.', shared: 'Shared block layout.',
  offset: 'Byte offset of a block member / atomic counter.', align: 'Alignment of a block member.', component: 'Component of a location.',
  index: 'Dual-source blending index.', set: 'Descriptor set (Vulkan).', push_constant: 'Push constant block (Vulkan).',
  row_major: 'Matrices stored row-major.', column_major: 'Matrices stored column-major (default).',
  local_size_x: 'Compute work group size X.', local_size_y: 'Compute work group size Y.', local_size_z: 'Compute work group size Z.',
  points: 'Geometry input/output primitive: points.', lines: 'Geometry input primitive: lines.', lines_adjacency: 'Geometry input: lines with adjacency.',
  triangles: 'Geometry input primitive: triangles.', triangles_adjacency: 'Geometry input: triangles with adjacency.',
  line_strip: 'Geometry output primitive: line strip.', triangle_strip: 'Geometry output primitive: triangle strip.',
  max_vertices: 'Max vertices a geometry shader invocation emits.', invocations: 'Geometry shader instancing count.', stream: 'Output stream index.',
  vertices: 'Tessellation control: output patch size.', quads: 'Tessellation domain: quads.', isolines: 'Tessellation domain: isolines.',
  equal_spacing: 'Tessellation spacing.', fractional_even_spacing: 'Tessellation spacing.', fractional_odd_spacing: 'Tessellation spacing.',
  cw: 'Clockwise winding.', ccw: 'Counter-clockwise winding.', point_mode: 'Tessellator outputs points.',
  origin_upper_left: 'gl_FragCoord origin at upper left.', pixel_center_integer: 'gl_FragCoord pixel centers on integers.',
  early_fragment_tests: 'Force depth/stencil tests before the pixel shader runs.',
  depth_any: 'gl_FragDepth may be changed arbitrarily.', depth_greater: 'gl_FragDepth only increases.', depth_less: 'gl_FragDepth only decreases.', depth_unchanged: 'gl_FragDepth unchanged.',
  rgba32f: 'Image format.', rgba16f: 'Image format.', rg32f: 'Image format.', rg16f: 'Image format.', r11f_g11f_b10f: 'Image format.', r32f: 'Image format.', r16f: 'Image format.',
  rgba8: 'Image format.', rgba16: 'Image format.', rgb10_a2: 'Image format.', rg8: 'Image format.', r8: 'Image format.', rgba8_snorm: 'Image format.',
  rgba32i: 'Image format.', rgba16i: 'Image format.', rgba8i: 'Image format.', r32i: 'Image format.',
  rgba32ui: 'Image format.', rgba16ui: 'Image format.', rgba8ui: 'Image format.', r32ui: 'Image format.',
};
const LAYOUT_WITH_VALUE = new Set(['location', 'binding', 'offset', 'align', 'component', 'index', 'set', 'local_size_x', 'local_size_y', 'local_size_z', 'max_vertices', 'invocations', 'stream', 'vertices']);

// ---------------- functions ----------------
const FUNCS = {};
function fn(names, doc, sigs, stages) {
  for (const name of names.split(' ')) FUNCS[name] = { doc, sigs: sigs.map(s => s.replace(/\$/g, name)), stages };
}
const G1 = ['genType $(genType x)'];
fn('radians', 'Converts degrees to radians.', ['genType $(genType degrees)']);
fn('degrees', 'Converts radians to degrees.', ['genType $(genType radians)']);
fn('sin', 'Sine (radians).', ['genType $(genType angle)']);
fn('cos', 'Cosine (radians).', ['genType $(genType angle)']);
fn('tan', 'Tangent (radians).', ['genType $(genType angle)']);
fn('asin', 'Arc sine. Result in [-pi/2, pi/2].', G1);
fn('acos', 'Arc cosine. Result in [0, pi].', G1);
fn('atan', 'Arc tangent. Two-argument form returns the angle of (x, y) in [-pi, pi].', ['genType $(genType y, genType x)', 'genType $(genType y_over_x)']);
fn('sinh cosh tanh asinh acosh atanh', 'Hyperbolic function.', G1);
fn('pow', 'x raised to the power y.', ['genType $(genType x, genType y)']);
fn('exp', 'Natural exponentiation e^x.', G1);
fn('log', 'Natural logarithm.', G1);
fn('exp2', '2 raised to the power x.', G1);
fn('log2', 'Base 2 logarithm.', G1);
fn('sqrt', 'Square root.', ['genType $(genType x)', 'genDType $(genDType x)']);
fn('inversesqrt', '1 / sqrt(x).', ['genType $(genType x)', 'genDType $(genDType x)']);
fn('abs', 'Absolute value.', ['genType $(genType x)', 'genIType $(genIType x)', 'genDType $(genDType x)']);
fn('sign', 'Returns -1, 0 or 1 depending on the sign of x.', ['genType $(genType x)', 'genIType $(genIType x)']);
fn('floor', 'Largest integer <= x.', G1);
fn('trunc', 'Nearest integer whose absolute value is <= |x|.', G1);
fn('round', 'Nearest integer.', G1);
fn('roundEven', 'Nearest integer, 0.5 rounds to nearest even.', G1);
fn('ceil', 'Smallest integer >= x.', G1);
fn('fract', 'x - floor(x).', G1);
fn('mod', 'x - y * floor(x / y).', ['genType $(genType x, float y)', 'genType $(genType x, genType y)']);
fn('modf', 'Splits x into integer (out i) and fractional (return) parts.', ['genType $(genType x, out genType i)']);
fn('min', 'Minimum of x and y.', ['genType $(genType x, genType y)', 'genType $(genType x, float y)', 'genIType $(genIType x, genIType y)', 'genUType $(genUType x, genUType y)']);
fn('max', 'Maximum of x and y.', ['genType $(genType x, genType y)', 'genType $(genType x, float y)', 'genIType $(genIType x, genIType y)', 'genUType $(genUType x, genUType y)']);
fn('clamp', 'min(max(x, minVal), maxVal).', ['genType $(genType x, genType minVal, genType maxVal)', 'genType $(genType x, float minVal, float maxVal)', 'genIType $(genIType x, int minVal, int maxVal)', 'genUType $(genUType x, uint minVal, uint maxVal)']);
fn('mix', 'Linear blend: x * (1 - a) + y * a. With a bool vector, selects components.', ['genType $(genType x, genType y, genType a)', 'genType $(genType x, genType y, float a)', 'genType $(genType x, genType y, genBType a)']);
fn('step', '0.0 if x < edge, else 1.0.', ['genType $(genType edge, genType x)', 'genType $(float edge, genType x)']);
fn('smoothstep', 'Hermite interpolation between 0 and 1 when edge0 < x < edge1.', ['genType $(genType edge0, genType edge1, genType x)', 'genType $(float edge0, float edge1, genType x)']);
fn('isnan', 'True if x is NaN.', ['genBType $(genType x)']);
fn('isinf', 'True if x is infinite.', ['genBType $(genType x)']);
fn('floatBitsToInt', 'Raw bits of a float as int.', ['genIType $(genType value)']);
fn('floatBitsToUint', 'Raw bits of a float as uint.', ['genUType $(genType value)']);
fn('intBitsToFloat', 'Reinterprets int bits as float.', ['genType $(genIType value)']);
fn('uintBitsToFloat', 'Reinterprets uint bits as float.', ['genType $(genUType value)']);
fn('fma', 'Fused multiply-add a * b + c.', ['genType $(genType a, genType b, genType c)']);
fn('frexp', 'Splits x into significand and exponent.', ['genType $(genType x, out genIType exp)']);
fn('ldexp', 'x * 2^exp.', ['genType $(genType x, genIType exp)']);
fn('packUnorm2x16 packSnorm2x16', 'Packs two normalized floats into a uint.', ['uint $(vec2 v)']);
fn('packUnorm4x8 packSnorm4x8', 'Packs four normalized floats into a uint.', ['uint $(vec4 v)']);
fn('unpackUnorm2x16 unpackSnorm2x16', 'Unpacks a uint into two normalized floats.', ['vec2 $(uint p)']);
fn('unpackUnorm4x8 unpackSnorm4x8', 'Unpacks a uint into four normalized floats.', ['vec4 $(uint p)']);
fn('packHalf2x16', 'Packs two floats as 16-bit halves into a uint.', ['uint $(vec2 v)']);
fn('unpackHalf2x16', 'Unpacks two 16-bit halves from a uint.', ['vec2 $(uint v)']);
fn('packDouble2x32', 'Packs two uints into a double.', ['double $(uvec2 v)']);
fn('unpackDouble2x32', 'Unpacks a double into two uints.', ['uvec2 $(double v)']);
fn('length', 'Length of vector x.', ['float $(genType x)']);
fn('distance', 'Distance between p0 and p1.', ['float $(genType p0, genType p1)']);
fn('dot', 'Dot product.', ['float $(genType x, genType y)']);
fn('cross', 'Cross product.', ['vec3 $(vec3 x, vec3 y)']);
fn('normalize', 'Vector in the same direction with length 1.', G1);
fn('faceforward', 'Returns N if dot(Nref, I) < 0, else -N.', ['genType $(genType N, genType I, genType Nref)']);
fn('reflect', 'Reflection direction: I - 2 * dot(N, I) * N. N should be normalized.', ['genType $(genType I, genType N)']);
fn('refract', 'Refraction vector for ratio of indices eta.', ['genType $(genType I, genType N, float eta)']);
fn('matrixCompMult', 'Component-wise matrix multiply.', ['mat $(mat x, mat y)']);
fn('outerProduct', 'Outer product c * r.', ['mat $(vec c, vec r)']);
fn('transpose', 'Transposed matrix.', ['mat $(mat m)']);
fn('determinant', 'Determinant of m.', ['float $(mat m)']);
fn('inverse', 'Inverse of m.', ['mat $(mat m)']);
fn('lessThan lessThanEqual greaterThan greaterThanEqual equal notEqual', 'Component-wise comparison.', ['bvec $(vec x, vec y)', 'bvec $(ivec x, ivec y)', 'bvec $(uvec x, uvec y)']);
fn('any', 'True if any component is true.', ['bool $(bvec x)']);
fn('all', 'True if all components are true.', ['bool $(bvec x)']);
fn('not', 'Component-wise logical complement.', ['bvec $(bvec x)']);
fn('uaddCarry', 'Add with carry.', ['genUType $(genUType x, genUType y, out genUType carry)']);
fn('usubBorrow', 'Subtract with borrow.', ['genUType $(genUType x, genUType y, out genUType borrow)']);
fn('umulExtended', '32x32 -> 64 bit unsigned multiply.', ['void $(genUType x, genUType y, out genUType msb, out genUType lsb)']);
fn('imulExtended', '32x32 -> 64 bit signed multiply.', ['void $(genIType x, genIType y, out genIType msb, out genIType lsb)']);
fn('bitfieldExtract', 'Extracts bits [offset, offset + bits).', ['genIType $(genIType value, int offset, int bits)', 'genUType $(genUType value, int offset, int bits)']);
fn('bitfieldInsert', 'Inserts bits of insert into base.', ['genIType $(genIType base, genIType insert, int offset, int bits)', 'genUType $(genUType base, genUType insert, int offset, int bits)']);
fn('bitfieldReverse', 'Reverses the bits.', ['genIType $(genIType value)', 'genUType $(genUType value)']);
fn('bitCount', 'Number of set bits.', ['genIType $(genIType value)', 'genIType $(genUType value)']);
fn('findLSB', 'Index of least significant set bit, -1 if none.', ['genIType $(genIType value)', 'genIType $(genUType value)']);
fn('findMSB', 'Index of most significant set bit, -1 if none.', ['genIType $(genIType value)', 'genIType $(genUType value)']);
// textures
fn('textureSize', 'Dimensions of a texture level.', ['ivec2 $(gsampler2D sampler, int lod)', 'ivec3 $(gsampler3D sampler, int lod)', 'int $(gsamplerBuffer sampler)']);
fn('textureQueryLod', 'LOD that would be used for the given coordinate.', ['vec2 $(gsampler sampler, vec P)'], 'f');
fn('textureQueryLevels', 'Number of mipmap levels.', ['int $(gsampler sampler)']);
fn('textureSamples', 'Number of samples of a multisample texture.', ['int $(gsampler2DMS sampler)']);
fn('texture', 'Samples a texture. The bias form is only available in pixel shaders.', ['gvec4 $(gsampler2D sampler, vec2 P)', 'gvec4 $(gsampler2D sampler, vec2 P, float bias)', 'gvec4 $(gsampler3D sampler, vec3 P)', 'gvec4 $(gsamplerCube sampler, vec3 P)', 'gvec4 $(gsampler2DArray sampler, vec3 P)', 'float $(sampler2DShadow sampler, vec3 P)']);
fn('textureProj', 'Projective texture lookup (P divided by its last component).', ['gvec4 $(gsampler2D sampler, vec3 P)', 'gvec4 $(gsampler2D sampler, vec4 P)']);
fn('textureLod', 'Texture lookup with explicit level of detail.', ['gvec4 $(gsampler2D sampler, vec2 P, float lod)', 'gvec4 $(gsampler3D sampler, vec3 P, float lod)', 'gvec4 $(gsamplerCube sampler, vec3 P, float lod)']);
fn('textureOffset', 'Texture lookup with a texel offset.', ['gvec4 $(gsampler2D sampler, vec2 P, ivec2 offset)', 'gvec4 $(gsampler2D sampler, vec2 P, ivec2 offset, float bias)']);
fn('texelFetch', 'Fetches a single texel by integer coordinate (no filtering).', ['gvec4 $(gsampler2D sampler, ivec2 P, int lod)', 'gvec4 $(gsampler3D sampler, ivec3 P, int lod)', 'gvec4 $(gsamplerBuffer sampler, int P)', 'gvec4 $(gsampler2DMS sampler, ivec2 P, int sample)']);
fn('texelFetchOffset', 'texelFetch with an offset.', ['gvec4 $(gsampler2D sampler, ivec2 P, int lod, ivec2 offset)']);
fn('textureProjOffset', 'Projective lookup with offset.', ['gvec4 $(gsampler2D sampler, vec3 P, ivec2 offset)']);
fn('textureLodOffset', 'Explicit LOD lookup with offset.', ['gvec4 $(gsampler2D sampler, vec2 P, float lod, ivec2 offset)']);
fn('textureProjLod', 'Projective lookup with explicit LOD.', ['gvec4 $(gsampler2D sampler, vec3 P, float lod)']);
fn('textureGrad', 'Lookup with explicit gradients.', ['gvec4 $(gsampler2D sampler, vec2 P, vec2 dPdx, vec2 dPdy)']);
fn('textureGradOffset', 'Lookup with explicit gradients and offset.', ['gvec4 $(gsampler2D sampler, vec2 P, vec2 dPdx, vec2 dPdy, ivec2 offset)']);
fn('textureProjGrad', 'Projective lookup with explicit gradients.', ['gvec4 $(gsampler2D sampler, vec3 P, vec2 dPdx, vec2 dPdy)']);
fn('textureGather', 'Gathers one component from the four texels used for bilinear filtering.', ['gvec4 $(gsampler2D sampler, vec2 P)', 'gvec4 $(gsampler2D sampler, vec2 P, int comp)']);
fn('textureGatherOffset', 'textureGather with an offset.', ['gvec4 $(gsampler2D sampler, vec2 P, ivec2 offset)']);
fn('textureGatherOffsets', 'textureGather with four offsets.', ['gvec4 $(gsampler2D sampler, vec2 P, ivec2 offsets[4])']);
// atomics
fn('atomicCounterIncrement', 'Atomically increments the counter, returns the old value.', ['uint $(atomic_uint c)']);
fn('atomicCounterDecrement', 'Atomically decrements the counter, returns the new value.', ['uint $(atomic_uint c)']);
fn('atomicCounter', 'Current counter value.', ['uint $(atomic_uint c)']);
fn('atomicAdd atomicMin atomicMax atomicAnd atomicOr atomicXor atomicExchange', 'Atomic operation on buffer/shared memory. Returns the original value.', ['uint $(inout uint mem, uint data)', 'int $(inout int mem, int data)']);
fn('atomicCompSwap', 'If mem == compare, writes data. Returns the original value.', ['uint $(inout uint mem, uint compare, uint data)', 'int $(inout int mem, int compare, int data)']);
// images
fn('imageSize', 'Dimensions of an image.', ['ivec2 $(gimage2D image)', 'ivec3 $(gimage3D image)']);
fn('imageSamples', 'Samples of a multisample image.', ['int $(gimage2DMS image)']);
fn('imageLoad', 'Reads a texel from an image.', ['gvec4 $(gimage2D image, ivec2 P)', 'gvec4 $(gimage3D image, ivec3 P)']);
fn('imageStore', 'Writes a texel to an image.', ['void $(gimage2D image, ivec2 P, gvec4 data)', 'void $(gimage3D image, ivec3 P, gvec4 data)']);
fn('imageAtomicAdd imageAtomicMin imageAtomicMax imageAtomicAnd imageAtomicOr imageAtomicXor imageAtomicExchange', 'Atomic operation on an image texel.', ['uint $(uimage2D image, ivec2 P, uint data)', 'int $(iimage2D image, ivec2 P, int data)']);
fn('imageAtomicCompSwap', 'Atomic compare-and-swap on an image texel.', ['uint $(uimage2D image, ivec2 P, uint compare, uint data)']);
// stage specific
fn('EmitVertex', 'Emits the current values of output variables as a vertex.', ['void $()'], 'g');
fn('EndPrimitive', 'Completes the current output primitive.', ['void $()'], 'g');
fn('EmitStreamVertex', 'Emits a vertex to the given stream.', ['void $(int stream)'], 'g');
fn('EndStreamPrimitive', 'Ends the primitive on the given stream.', ['void $(int stream)'], 'g');
fn('dFdx dFdy dFdxFine dFdyFine dFdxCoarse dFdyCoarse', 'Screen-space partial derivative.', G1, 'f');
fn('fwidth fwidthFine fwidthCoarse', 'abs(dFdx(p)) + abs(dFdy(p)).', ['genType $(genType p)'], 'f');
fn('interpolateAtCentroid', 'Samples an input at the pixel centroid.', ['genType $(genType interpolant)'], 'f');
fn('interpolateAtSample', 'Samples an input at the given sample.', ['genType $(genType interpolant, int sample)'], 'f');
fn('interpolateAtOffset', 'Samples an input at an offset from the pixel center.', ['genType $(genType interpolant, vec2 offset)'], 'f');
fn('barrier', 'Synchronizes all invocations in a work group (compute) or patch (tess control).', ['void $()'], 'tc');
fn('memoryBarrier', 'Orders all memory transactions.', ['void $()']);
fn('memoryBarrierAtomicCounter', 'Orders atomic counter accesses.', ['void $()']);
fn('memoryBarrierBuffer', 'Orders buffer variable accesses.', ['void $()']);
fn('memoryBarrierImage', 'Orders image accesses.', ['void $()']);
fn('memoryBarrierShared', 'Orders shared variable accesses.', ['void $()'], 'c');
fn('groupMemoryBarrier', 'Orders memory transactions within the work group.', ['void $()'], 'c');

const FUNC_RETURNS = {
  texture: 'vec4', textureLod: 'vec4', textureOffset: 'vec4', textureProj: 'vec4', textureGrad: 'vec4', texelFetch: 'vec4',
  textureGather: 'vec4', imageLoad: 'vec4', cross: 'vec3', length: 'float', distance: 'float', dot: 'float',
  unpackUnorm4x8: 'vec4', unpackSnorm4x8: 'vec4', unpackHalf2x16: 'vec2', unpackUnorm2x16: 'vec2',
};

// ---------------- variables ----------------
const VARS = {};
function v(name, type, stages, doc, array) { VARS[name] = { type, stages, doc, array: array || '' }; }
v('gl_VertexID', 'int', 'v', 'Index of the current vertex.');
v('gl_InstanceID', 'int', 'v', 'Index of the current instance in an instanced draw.');
v('gl_DrawID', 'int', 'v', 'Index of the draw in a multi-draw command.');
v('gl_BaseVertex', 'int', 'v', 'basevertex parameter of the draw.');
v('gl_BaseInstance', 'int', 'v', 'baseinstance parameter of the draw.');
v('gl_Position', 'vec4', 'vteg', 'Clip-space output position of the vertex.');
v('gl_PointSize', 'float', 'vteg', 'Size of rasterized points (needs GL_PROGRAM_POINT_SIZE).');
v('gl_ClipDistance', 'float', 'vtegf', 'User clip distances.', '[]');
v('gl_CullDistance', 'float', 'vtegf', 'User cull distances.', '[]');
v('gl_in', 'gl_PerVertex', 'teg', 'Per-vertex outputs of the previous stage.', '[]');
v('gl_out', 'gl_PerVertex', 't', 'Per-vertex outputs of the tessellation control shader.', '[]');
v('gl_PrimitiveIDIn', 'int', 'g', 'Primitive index of the input primitive.');
v('gl_InvocationID', 'int', 'tg', 'Invocation index (tess control vertex / geometry instance).');
v('gl_PrimitiveID', 'int', 'tegf', 'Index of the current primitive.');
v('gl_Layer', 'int', 'gf', 'Layer of a layered framebuffer to render to.');
v('gl_ViewportIndex', 'int', 'gf', 'Viewport to render to.');
v('gl_PatchVerticesIn', 'int', 'te', 'Number of vertices in the input patch.');
v('gl_TessLevelOuter', 'float', 'te', 'Outer tessellation levels.', '[4]');
v('gl_TessLevelInner', 'float', 'te', 'Inner tessellation levels.', '[2]');
v('gl_TessCoord', 'vec3', 'e', 'Position of the vertex in the abstract patch.');
v('gl_FragCoord', 'vec4', 'f', 'Window-relative coordinate of the pixel (x, y, z = depth, w = 1/w).');
v('gl_FrontFacing', 'bool', 'f', 'True if the primitive is front facing.');
v('gl_PointCoord', 'vec2', 'f', 'Coordinate inside a point sprite [0, 1].');
v('gl_SampleID', 'int', 'f', 'Index of the sample being processed.');
v('gl_SamplePosition', 'vec2', 'f', 'Position of the sample within the pixel.');
v('gl_SampleMaskIn', 'int', 'f', 'Coverage mask of the pixel.', '[]');
v('gl_SampleMask', 'int', 'f', 'Output coverage mask.', '[]');
v('gl_HelperInvocation', 'bool', 'f', 'True if this is a helper invocation (for derivatives).');
v('gl_FragDepth', 'float', 'f', 'Output depth value (default gl_FragCoord.z).');
v('gl_NumWorkGroups', 'uvec3', 'c', 'Number of work groups passed to glDispatchCompute.');
v('gl_WorkGroupSize', 'uvec3', 'c', 'local_size_x/y/z of this shader.');
v('gl_WorkGroupID', 'uvec3', 'c', 'Index of the current work group.');
v('gl_LocalInvocationID', 'uvec3', 'c', 'Index of the invocation within the work group.');
v('gl_GlobalInvocationID', 'uvec3', 'c', 'gl_WorkGroupID * gl_WorkGroupSize + gl_LocalInvocationID.');
v('gl_LocalInvocationIndex', 'uint', 'c', 'Flattened gl_LocalInvocationID.');
for (const c of ['gl_MaxVertexAttribs', 'gl_MaxTextureImageUnits', 'gl_MaxCombinedTextureImageUnits', 'gl_MaxDrawBuffers',
  'gl_MaxGeometryOutputVertices', 'gl_MaxComputeWorkGroupCount', 'gl_MaxComputeWorkGroupSize', 'gl_MaxClipDistances', 'gl_MaxViewports'])
  VARS[c] = { type: /WorkGroup(Count|Size)/.test(c) ? 'ivec3' : 'int', doc: 'Implementation limit constant.', array: '', constant: true };

const BUILTIN_STRUCTS = {
  gl_PerVertex: { name: 'gl_PerVertex', kind: 'struct', fields: [
    { name: 'gl_Position', type: 'vec4', array: '' }, { name: 'gl_PointSize', type: 'float', array: '' },
    { name: 'gl_ClipDistance', type: 'float', array: '[]' }, { name: 'gl_CullDistance', type: 'float', array: '[]' }] },
};

const DIRECTIVES = ['version', 'extension', 'define', 'undef', 'if', 'ifdef', 'ifndef', 'else', 'elif', 'endif', 'pragma', 'line', 'error', 'include'];
const VERSIONS = ['460 core', '450 core', '440 core', '430 core', '420 core', '410 core', '400 core', '330 core', '300 es', '310 es', '320 es'];
const EXTENSIONS = ['GL_ARB_bindless_texture', 'GL_ARB_shader_draw_parameters', 'GL_ARB_gpu_shader_int64', 'GL_KHR_shader_subgroup_basic',
  'GL_KHR_shader_subgroup_arithmetic', 'GL_EXT_nonuniform_qualifier', 'GL_ARB_shader_viewport_layer_array', 'GL_GOOGLE_include_directive'];

// ---------------- type helpers ----------------
const SCALAR_OF = { '': 'float', b: 'bool', i: 'int', u: 'uint', d: 'double' };
function vecInfo(t) { const m = /^([biud]?)vec([234])$/.exec(t); return m ? { prefix: m[1], size: +m[2], scalar: SCALAR_OF[m[1]] } : null; }
function vecType(scalar, n) {
  if (n === 1) return scalar;
  const p = Object.keys(SCALAR_OF).find(k => SCALAR_OF[k] === scalar);
  return (p || '') + 'vec' + n;
}
function matInfo(t) { const m = /^(d?)mat([234])(?:x([234]))?$/.exec(t); return m ? { cols: +m[2], rows: +(m[3] || m[2]), column: (m[1] ? 'dvec' : 'vec') + (m[3] || m[2]) } : null; }
const SWIZZLE_SETS = ['xyzw', 'rgba', 'stpq'];
function isSwizzle(name, size) {
  if (name.length < 1 || name.length > 4) return false;
  return SWIZZLE_SETS.some(set => [...name].every(ch => { const i = set.indexOf(ch); return i >= 0 && i < size; }));
}
function stageOk(def, letter) { return !def.stages || !letter || def.stages.includes(letter); }

module.exports = { TYPES, TYPE_SET, QUALIFIERS, CONTROL, KEYWORDS, KEYWORD_SET, KEYWORD_DOCS, LAYOUT_QUALIFIERS, LAYOUT_WITH_VALUE,
  FUNCS, FUNC_RETURNS, VARS, BUILTIN_STRUCTS, DIRECTIVES, VERSIONS, EXTENSIONS, vecInfo, vecType, matInfo, isSwizzle, stageOk };

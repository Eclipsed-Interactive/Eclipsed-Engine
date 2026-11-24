#type pixel
#version 460 core

layout(binding=0) uniform sampler2D albedo;

layout(std140, binding=5) uniform MaterialBuffer
{
   vec4 color;
   vec4 spriteRect; // x, y, width, height
};

// Ins
layout(location = 0) in vec2 outTexCoord;

// Outs
layout(location = 0) out vec4 frag_colour;

void main() 
{
   vec2 modifiedUV = vec2(
      spriteRect.x + outTexCoord.x * spriteRect.z,
      spriteRect.y + outTexCoord.y * spriteRect.w
   );

   vec4 textureAlbedo = texture(albedo, modifiedUV);

   frag_colour = textureAlbedo * color;
}
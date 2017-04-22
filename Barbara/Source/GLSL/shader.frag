#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;


layout(push_constant) uniform PushConsts 
{
uint index;
} pushConsts;
layout(set= 1, binding = 1) uniform sampler2D texSampler[6];

void main()
{
    outColor = texture(texSampler[pushConsts.index], fragTexCoord);
}

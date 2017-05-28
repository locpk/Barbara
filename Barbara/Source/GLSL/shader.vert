#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(binding = 0) uniform UniformBuffer
{
    mat4 model;
    mat4 view;
    mat4 proj;
	mat4 normalMat;
	vec3 camPos;
} uniformBuffer;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec2 fragTexCoord;

out gl_PerVertex 
{
    vec4 gl_Position;
}
;
void main()
{
    gl_Position = uniformBuffer.proj * uniformBuffer.view * uniformBuffer.model * vec4(inPosition, 1.0);
    fragColor = inColor;
	fragTexCoord = inTexCoord;
	fragPos = vec3(uniformBuffer.model * vec4(inPosition, 1.0));
	fragNormal = vec3(uniformBuffer.normalMat * vec4(inNormal, 1.0));
}

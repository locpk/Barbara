#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec2 fragTexCoord;



layout(location = 0) out vec4 outColor;


layout(set= 1, binding = 1) uniform sampler2D texSampler[6];


layout(push_constant) uniform PushConsts 
{
vec4	ambientLightColorAndIndex;
vec3	lightPos;
} pushConsts;



void main()
{
	uint textureIndex = uint(pushConsts.ambientLightColorAndIndex.a);
	vec3 lightColor = vec3(1.0f,0.0f,0.0f);
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(pushConsts.lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;


    outColor = texture(texSampler[textureIndex], fragTexCoord) * vec4(pushConsts.ambientLightColorAndIndex.rgb +diffuse,1.0f);
	outColor = clamp(outColor,0.0f,1.0f);
}

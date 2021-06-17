#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in vec3 a_Normal;
layout(location = 6) in mat4 a_Model;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;
out vec3 v_Normal;

out vec3 FragPos;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_Normal = a_Normal; //mat3(transpose(inverse(a_Model))) * a_Normal;
	FragPos = a_Position;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}		

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
			
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;
in vec3 v_Normal;

in vec3 FragPos;

uniform sampler2D u_Textures[32];
uniform vec4 lightColor;
uniform vec3 lightPos;

uniform vec3 u_ViewPos;

void main()
{
	// ambient
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * vec3(lightColor);

	//diffuse
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(lightColor);

	vec4 result = vec4((ambient + diffuse) , 1.0) * v_Color;

	vec4 texColor = result;
	texColor *= texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;

	color = texColor; 
}		
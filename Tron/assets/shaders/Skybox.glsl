#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 TexCoords;

// uniform mat4 u_ViewProjection;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = a_Position;
    vec4 pos = projection * view * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}  


#type fragment
#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_Skybox;

void main()
{    
    FragColor = texture(u_Skybox, TexCoords);
}
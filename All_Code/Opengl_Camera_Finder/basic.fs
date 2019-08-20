#version 330 core

layout(location=0) out vec4 f_color;
in vec4 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture; //importet color from vertices shader
uniform vec4 u_color; //importet color from main

void main()
{
	vec4 texColor=texture(u_texture,v_texCoord);
	//f_color=u_color;
	f_color=v_color;
}
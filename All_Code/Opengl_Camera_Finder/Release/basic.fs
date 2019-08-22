#version 330 core

layout(location=0) out vec4 f_color;
in vec4 v_color;
in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_position;

uniform sampler2D u_texture; //importet color from vertices shader
uniform vec4 u_color; //importet color from main

void main()
{
	vec4 texColor=texture(u_texture,v_texCoord);
	//f_color=u_color;
	//f_color=v_color;


	//lighting
	vec3 view=normalize(-v_position); //kamera immer bei (0,0,0)
	vec3 light = normalize(vec3(1.0f,1.0f,1.0f)); //licht richtung --> für head light position (0,0,0)
	vec3 normal=normalize(v_normal); // normalizserung kann verloren gehen durch interpolation für fragments zwischen vertices
	vec3 color=vec3(1.0f,0.0f,0.0f);
	vec3 refleccolor=vec3(1.0f,1.0f,1.0f);
	vec3 reflection=reflect(-light,normal); // berechnet reflektion (einfallsswikeln=ausfallwinkels)
	vec3 ambient=color*0.2;
	vec3 deffuse=max(dot(normal,light),0.0)*color; //projection der beiden afeinander --> max sorg dafür das es nicht unter 0 geht
	vec3 specular=pow(max(dot(reflection,view),0.0),4.0)*refleccolor; // specular reflektionen
	f_color=vec4(ambient+deffuse+specular,1.0f); // combination der einezlen effekte
}
#version 330 core

layout(location=0) in vec3 a_position;
layout(location=1) in vec2 a_texCoord;
layout(location=2) in vec4 a_color;
layout(location=3) in vec3 a_normal;



out vec4 v_color;
out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_position; //fragment position

uniform mat4 u_modelViewProj;
uniform mat4 u_modelView; // view matrix ohne verzerrungen der perspektive die normale andern 
uniform mat4 u_invModelView; //ignoriert skalierung des bildes --> wenn nicht andern sich dadurch normalenvektoren

void main(){

gl_Position  = u_modelViewProj * vec4(a_position, 1.0f);

v_color = a_color;
//v_color=vec4(normalize(a_position),1.0f);
//v_color=vec4(a_normal,1.0f);
v_texCoord=a_texCoord;

//lighting
v_normal=mat3(u_invModelView)*a_normal; //mat3 weil keine translation weil das sind vektoren keine punkte
v_position=vec3(u_modelView*vec4(a_position,1.0f));
}
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 g_color;
in vec2 g_texCoord;
in vec3 g_normal;
in vec3 g_position;


out vec4 v_color;
out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_position;

void main(){
v_color = g_color;
v_texCoord=g_texCoord;
v_normal=g_normal;
v_position=g_position;
}
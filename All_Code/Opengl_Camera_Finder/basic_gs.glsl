#version 450 core
layout(lines_adjacency) in;
//layout(triangle_strip, max_vertices = 20) out;
layout(line_strip, max_vertices = 20) out;


in vec4 g_color[];
in vec2 g_texCoord[];
in vec3 g_normal[];
in vec3 g_position[]; 

out vec4 v_color;
out vec2 v_texCoord;
out vec3 v_normal; 
out vec3 v_position;


void main(){
    
v_color=g_color[0];
v_texCoord=g_texCoord[0];
v_normal=g_normal[0]; 
v_position=g_position[0];
EmitVertex();
v_color=g_color[1];
v_texCoord=g_texCoord[1];
v_normal=g_normal[1]; 
v_position=g_position[1];
EmitVertex();
v_color=g_color[2];
v_texCoord=g_texCoord[2];
v_normal=g_normal[2]; 
v_position=g_position[2];
EmitVertex();
v_color=g_color[3];
v_texCoord=g_texCoord[3];
v_normal=g_normal[3]; 
v_position=g_position[3];
EmitVertex();
//v_color=g_color[4];
//v_texCoord=g_texCoord[4];
//v_normal=g_normal[4]; 
//v_position=g_position[4];
//EmitVertex();
//v_color=g_color[5];
//v_texCoord=g_texCoord[5];
//v_normal=g_normal[5]; 
//v_position=g_position[5];
//EmitVertex();
//v_color=g_color[6];
//v_texCoord=g_texCoord[6];
//v_normal=g_normal[6]; 
//v_position=g_position[6];
EmitVertex();
EndPrimitive();
}

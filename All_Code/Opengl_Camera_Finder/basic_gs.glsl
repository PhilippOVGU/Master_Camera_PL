#version 450 core
layout(lines) in;
//layout(lines_adjacency) in;
//layout(triangle_strip, max_vertices = 20) out;
layout(triangle_strip, max_vertices = 20) out;


in vec4 g_color[];
in vec2 g_texCoord[];
in vec3 g_normal[];
in vec3 g_position[]; 

out vec4 v_color;
out vec2 v_texCoord;
out vec3 v_normal; 
out vec3 v_position;
uniform float lineWidth = 0.2;

void createVertexCopy(uint source, vec3 translateDirection, vec3 tangentDirection, float translateScale) {
    if (translateScale == 0) return;
    gl_Position = vec4((gl_in[source].gl_Position + vec4(translateDirection,0) * translateScale).xy,gl_in[source].gl_Position.zw);
//    vertex.borderDistance = translateScale;
//    vertex.normal = normalize(cross(translateDirection, tangentDirection));
//    vertex.tangent = tangentDirection;
//    vertex.width = abs(translateScale);
//    copy(source);
    EmitVertex();   // Submits the current vertex and starts a new vertex
}
void main(){
//const float scale = lineWidth/2.0;
//const vec3 line_0_1 = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
//const vec3 line_1_2 = gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz;
//const vec3 line_2_3 = gl_in[2].gl_Position.xyz - gl_in[3].gl_Position.xyz;
//const vec3 tan_1 = normalize((line_0_1 + line_1_2)/2.0);
//const vec3 tan_2 = normalize((line_1_2 + line_2_3)/2.0);
//
//const vec3 orth_1 = normalize(cross(-v_position,tan_1));
//const vec3 orth_2 = normalize(cross(-v_position,tan_2));
//
//
//
//createVertexCopy(1, orth_1, tan_1, +scale * 1);
//createVertexCopy(2, orth_2, tan_2, +scale * 1);
//createVertexCopy(2, orth_2, tan_2, -scale * 1);
//EndPrimitive();
//
//createVertexCopy(1, orth_1, tan_1, +scale * 1);
//createVertexCopy(1, orth_1, tan_1, -scale * 1);
//createVertexCopy(2, orth_2, tan_2, -scale * 1);
//EndPrimitive();

const vec3 linevector=gl_in[1].gl_Position.xyz-gl_in[0].gl_Position.xyz;
//const vec3 viewvector=vec3(1,1,1);
//const vec3 viewvector=vec3(-g_position[0]);
const vec3 viewvector=vec3((-g_position[0]-g_position[1])/2.0);
const vec3 orthogonal=normalize(cross(viewvector,linevector));
gl_Position = vec4((gl_in[0].gl_Position + vec4(orthogonal,0)*lineWidth).xy,gl_in[0].gl_Position.zw);
//gl_Position = gl_in[0].gl_Position;
v_color=g_color[0];
v_texCoord=g_texCoord[0];
//v_normal=g_normal[0]; 
v_normal=normalize(cross(linevector, gl_Position.xyz));
v_position=g_position[0];
EmitVertex();
gl_Position = vec4((gl_in[0].gl_Position - vec4(orthogonal,0)*lineWidth).xy,gl_in[0].gl_Position.zw);
v_color=g_color[0];
v_texCoord=g_texCoord[0];
//v_normal=g_normal[0]; 
v_normal=normalize(cross(linevector, gl_Position.xyz));
v_position=g_position[0];
EmitVertex();
gl_Position = vec4((gl_in[1].gl_Position + vec4(orthogonal,0)*lineWidth).xy,gl_in[1].gl_Position.zw);
v_color=g_color[1];
v_texCoord=g_texCoord[1];
//v_normal=g_normal[1]; 
v_normal=normalize(cross(linevector, gl_Position.xyz));
v_position=g_position[1];
EmitVertex();
gl_Position = vec4((gl_in[1].gl_Position - vec4(orthogonal,0)*lineWidth).xy,gl_in[1].gl_Position.zw);
v_color=g_color[1];
v_texCoord=g_texCoord[1];
//v_normal=g_normal[1]; 
v_normal=normalize(cross(linevector, gl_Position.xyz));
v_position=g_position[1];
EmitVertex();
EndPrimitive();



//EndPrimitive();
}

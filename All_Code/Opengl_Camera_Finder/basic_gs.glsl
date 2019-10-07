#version 450 core
layout(lines_adjacency) in;
//layout(triangle_strip, max_vertices = 20) out;
layout(line_strip, max_vertices = 20) out;

in vec4 v_color[];
in vec2 v_texCoord[];
in vec3 v_normal[]; 
in vec3 v_position[];


out vec4 g_color;
out vec2 g_texCoord;
out vec3 g_normal; 
out vec3 g_position;

void main(){
    
	/**
	 * Nicht zu viele Sachen auf einmal! Ich habe den Geometry Shader mal so angepasst, dass er die existierende
	 * Geometrie durchreicht (also auch Linien anstelle von Dreiecken produziert). Adaptiere das am besten in kleinen
	 * Schritten. Den Umschwung von Linien auf Dreiecke würde ich erst am Ende machen - du kannst zwar Dreiecke berechnen,
	 * diese aber trotzdem weiter als Linien ausgeben.
	 */
	
	g_color = v_color[0];
	g_texCoord = v_texCoord[0];
	g_position = v_position[0];
	g_normal = v_normal[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex(); 
	
	g_color = v_color[1];
	g_texCoord = v_texCoord[1];
	g_position = v_position[1];
	g_normal = v_normal[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex(); 
	
	EndPrimitive();
	
	g_color = v_color[1];
	g_texCoord = v_texCoord[1];
	g_position = v_position[1];
	g_normal = v_normal[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex(); 
	
	g_color = v_color[2];
	g_texCoord = v_texCoord[2];
	g_position = v_position[2];
	g_normal = v_normal[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex(); 
	
	EndPrimitive();
	
	g_color = v_color[2];
	g_texCoord = v_texCoord[2];
	g_position = v_position[2];
	g_normal = v_normal[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex(); 
	
	g_color = v_color[3];
	g_texCoord = v_texCoord[3];
	g_position = v_position[3];
	g_normal = v_normal[3];
	gl_Position = gl_in[3].gl_Position;
	EmitVertex(); 
	
	EndPrimitive();
}

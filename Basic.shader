#VERTEX
#version 330 core
   
layout(location = 1) in vec3 vertexColor;  
layout(location = 0) in vec3 position;   
out vec3 fragmentColor;  
uniform mat4 MODEL;  
uniform mat4 PROJECTION;
void main()  
{  
	gl_Position = PROJECTION * MODEL * vec4(position, 1.0);  
	fragmentColor = vertexColor; 
} 
#VERTEXEND

#FRAGMENT
#version 330 core  
   
in vec3 fragmentColor;  
out vec3 color;   
void main()  
{  
	color = fragmentColor;  
}  
#FRAGMENTEND
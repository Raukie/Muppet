#VERTEX
#version 330 core
   
layout(location = 1) in vec3 vertexColor;  
layout(location = 0) in vec3 position;   
out vec3 fragmentColor;  
uniform mat4 MVP;  
void main()  
{  
	gl_Position = MVP * vec4(position, 1.0);  
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
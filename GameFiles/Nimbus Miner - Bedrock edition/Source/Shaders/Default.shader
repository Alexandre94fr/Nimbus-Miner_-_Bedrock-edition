// -- DOCUMENTATION : 
// -- This file contains the vertex and the fragment shader.

// -- A vertex shader is the shader that will place on the screen our vertices
// -- (called x time | x representing the number of vertex you have)

// -- The shader that will fill out the triangle of colored pixels 
// -- (called x time | x representing the number of pixels you have to draw)

// -- I decided to put them in the same file because it will be easier to modify them.

// -- But that has for consequences for us to be able to extract only the correct shader, and to not take documentation.
// -- In order to detect what is a vertex or fragment shader you have the "// Shader Vertex / Fragment" 
// -- (not in UPPERCASE to not be detected) comment before it.
// -- For documentation detection you have the "// --" before it.

// SHADER VERTEX
#version 330 core
 
layout(location = 0) in vec2 positionAttribute;
 
void main()
{
    gl_Position = vec4(positionAttribute, 0.0, 1.0);
}


// ------------------------------------------------ //


// SHADER FRAGMENT
#version 330 core
 
out vec4 FragmentColor;
 
// -- Accessible variable (by vertex shader, and CPU)
uniform vec4 u_Color;
 
void main()
{ 
    FragmentColor = u_Color; 
}
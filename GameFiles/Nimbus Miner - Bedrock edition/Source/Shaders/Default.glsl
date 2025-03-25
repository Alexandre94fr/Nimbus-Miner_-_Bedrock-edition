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
 
layout(location = 0) in vec4 PositionAttribute;
layout(location = 1) in vec2 TexturePositionAttribute;
 
// -- Transmitting data to the fragment shader
// -- v stands for "varying"
out vec2 v_TexturePosition; 
 
void main()
{
    gl_Position = PositionAttribute;
    v_TexturePosition = TexturePositionAttribute;
}
 
// -- ========================== -- //
// -- ========================== -- //
// -- ========================== -- //
 
// SHADER FRAGMENT
#version 330 core
 
// -- Accessible variable (by vertex shader, and CPU)
uniform vec4 u_Color;
uniform sampler2D u_Texture;
 
// -- Getting values from the Vertex Shader
in vec2 v_TexturePosition;
 
out vec4 FragmentColor;
 
void main()
{
    vec4 textureColor = texture(u_Texture, v_TexturePosition);
    
    FragmentColor = textureColor * u_Color; 
}
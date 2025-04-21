// -- DOCUMENTATION : 
// -- This file contains the vertex and the fragment shader.

// -- A vertex shader is the shader that will place on the screen our vertices
// -- (called x time | x representing the number of vertex you have)

// -- The shader that will fill out the triangle of colored pixels 
// -- (called x time | x representing the number of pixels you have to draw)

// -- I decided to put them in the same file because it will be easier to modify them.

// -- But that has for consequences for us to be able to extract only the correct shader, and to not take documentation.
// -- In order to detect what is a vertex or fragment shader you have the "// SHADER VERTEX / FRAGMENT"

// -- If you want to put documention into your shaders you can put "// --" in the line. 
// -- If you do so all the line will be ignored.

// SHADER VERTEX
#version 330 core
 
layout(location = 0) in vec4 PositionAttribute;
layout(location = 1) in vec4 NormalAttribute;
layout(location = 2) in vec4 ColorAttribute;
layout(location = 3) in vec3 TexturePositionAttribute;
// -- Contains TexturePosition (UV) AND TextureIndex (Layer) [the index of which texture will be drawn]
 
uniform mat4 u_ModelViewProjectionMatrix;
 
// -- Transmitting data to the fragment shader
// -- v stands for "varying"
out vec3 v_TexturePosition; 
out vec4 v_Color;
 
void main()
{
    gl_Position = u_ModelViewProjectionMatrix * PositionAttribute;
    v_Color = ColorAttribute;
    v_TexturePosition = TexturePositionAttribute;
}
 
// -- ========================== -- //
// -- ========================== -- //
// -- ========================== -- //
 
// SHADER FRAGMENT
#version 330 core
 
// -- Accessible variable (by vertex shader, and CPU)
uniform sampler2D u_Texture;
// -- MULTI-TEXTURE VERSION : uniform sampler2DArray u_TextureArray;
 
// -- Getting values from the Vertex Shader
in vec3 v_TexturePosition;
in vec4 v_Color;
 
out vec4 FragmentColor;
 
void main()
{
    vec4 textureColor = texture(u_Texture, v_TexturePosition.xy);
    // -- MULTI-TEXTURE VERSION : vec4 textureColor = texture(u_TextureArray, v_TexturePosition);
    
    FragmentColor = textureColor * v_Color; 
}
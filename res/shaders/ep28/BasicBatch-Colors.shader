#shader vertex
#version 330 core

layout(location=0) in vec4 a_Position;
layout(location=1) in vec4 a_Color;

//  the v_ stands for 'varying'
// out vec2 v_TexCoord;

//  Model View Projection matrix -- though just the projection matrix is sent
uniform mat4 u_MVP;

//  The old thing it was called: a varying.
out vec4 v_Color;

void main()
{
    //  Multiply it with the position to transform
    //  ... the position to fit the new aspect ratio defined by the Orthographic Projection Matrix
    v_Color = a_Color;
    gl_Position = u_MVP * a_Position;
    // v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in vec4 v_Color;

//  This now needs to be setup in C++
// uniform vec4 u_Color;
// uniform sampler2D u_Texture;

void main()
{

    //  Assigning the output color of each fragment/pixel to be what the input value is.
    color = v_Color;
};
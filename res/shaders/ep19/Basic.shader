#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoord;

//  the v_ stands for 'varying'
out vec2 v_TexCoord;

//  Model View Projection matrix -- though just the projection matrix is sent
uniform mat4 u_MVP;

void main()
{
    //  Multiply it with the position to transform
    //  ... the position to fit the new aspect ratio defined by the Orthographic Projection Matrix
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

in vec2 v_TexCoord;

//  This now needs to be setup in C++
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    //  Needs the texture coordinates from the VertexShader to sample that specific texture coordinate
    //  This is because fragement shader runs for every pixel, so the right pixel position from the texture
    //  needs to be known so it can be looked up and drawn on the surface.
    vec4 texColor = texture(u_Texture, v_TexCoord);
    //  Assigning the output color of each fragment/pixel to be what this uniform value is.
    color = texColor;
};
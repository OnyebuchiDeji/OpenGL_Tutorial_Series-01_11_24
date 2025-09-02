#shader vertex
#version 330 core

layout(location=0) in vec4 a_Position;
layout(location=1) in vec4 a_Color;
layout(location=2) in vec2 a_TexCoord;
layout(location=3) in float a_TexIndex;

//  the v_ stands for 'varying'
// out vec2 v_TexCoord;

//  Model View Projection matrix -- though just the projection matrix is sent
uniform mat4 u_MVP;

//  The old thing it was called: a varying.
out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
    //  Multiply it with the position to transform
    //  ... the position to fit the new aspect ratio defined by the Orthographic Projection Matrix
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;

    gl_Position = u_MVP * a_Position;
    // v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location=0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[5];

//  This now needs to be setup in C++
// uniform vec4 u_Color;
// uniform sampler2D u_Texture;

void main()
{

    //This will show which has texture coordinate 0,
    //if (v_TexIndex >= 0.9f && v_TexIndex < 2.0f)
    //    o_Color = vec4(v_TexIndex, 0.0f, 0.0f, 1.0f);
    //else if (v_TexIndex >= 2.0f)
    //    o_Color = vec4(0.0f, v_TexIndex, 0.0f, 1.0f);
    //else
    //    o_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    
    // o_Color *= vec4(v_TexCoord, 0.0, 1.0);

    int index = int(v_TexIndex);
    o_Color = texture(u_Textures[index], v_TexCoord);


};
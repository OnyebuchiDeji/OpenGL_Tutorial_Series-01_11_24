#shader vertex
#version 330 core

layout(location=0) in vec4 position;

void main()
{
    gl_Position = position;
};


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

//  This now needs to be setup in C++
uniform vec4 u_Color;

void main()
{
    //  Assigning the output color of each fragment/pixel to be what this uniform value is.
    color = u_Color;
};
/**
*   EP17: "Textures in OpenGL"
*   
*   What was done:
*       Created Texture class
*       Implemented loading image data using stbi library.
*       Set the GL Texture parameters to create a Texture object on initialization
*       implemented Bind and Unbind methods
*       Then instantiated the Texture object
*       Bound the Texture to OpenGL, getting it ready for OpenGL to use
*       Then specified via a Uniform the slot that OpenGL should read the texture from in the shader.
*       Then speecified texture coordinates to tell the geometry being rendered which area of the
*           texture to read pixel color from when OpenGL is rasterizing the pixels.
*/

//  External
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"




int main(void)
{
    /**
    *   From the GLFW library
    */
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /**
    *   @Exp
    *   By specifying these pieces of code, these are done:
    *       The major version 3 and minor version 3, hence 3.3
    *   Also Core OPENGL Profile is being used.
    *   This profile requires that a VertexArrayObject be made explicitely.
    */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /**
    *   The COMPAT profile is the one specified above by the major and minor versions, 3.3
    *   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    *   But Core ensures that no VertexArrayObject is made and bound implicitely.
    */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Yo! World!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Syncing Frame Rate */
    glfwSwapInterval(1);

    //  Initialize GLEW; now one can use the GL Functions.
    //  All the functions are function pointers.
    if (glewInit() != GLEW_OK)
        std::cout << "Error!\n";

    //  Print out version.
    std::cout << glGetString(GL_VERSION) << "\n";
    {


        /**
        *   The number of vertices are changed for the number for a square
        *   It can be made easier using index buffers.
        *   This are the number of vertices needed when using Index Buffers; there is no need
        *   to repeat vertices, hence the number of vertices reduce.
        *  as only the unique vertices are left
        *
        *   The index buffer is added to the Vertex Buffer
        **/
        /**
        *   Texture Coordinates
        *
        *   Now these tell the gemoetry being rendered which area of the Texture to render from.
        *   The pixel shader goes through and rasterizes eveery pixel of the rectangle 9in this case);
        *   it is responsible for determine the colour of each pixel at each position within the Rectangle's area
        *   By specifying the texture coordinates, one specifies the area that the Dragment shader should read from
        *   to sample the pixel color and use that to determine which color and where it should be rendered on the Rectangle.
        *
        *   It starts from bottom left (s=0, t=0) to top right (s=1, t=1), where s represents horizontal position
        *   and t represents vertical position.
        *   so s=0.5, t=0.5 is halfway.
        */
        //  x, y, s, t
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,   //  0
            0.5f, -0.5f, 1.0f, 0.0f,   //  1
            0.5f, 0.5f, 1.0f, 1.0f,    //  2
            -0.5f, 0.5f, 0.0f, 1.0f   //  3
        };

        //  Specified the indices for each triangle
        //  Note the indices of the triangle arfe drawin in an anticlockwise direction
        //  Then this data is sent ot the GPU for OpenGL to render it
        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle
        };


        /**
        *   Enabling Blending is essential so that rastered textures allow transparency int hte parts
        *   that transparency should be in.
        * 
        *   It defines how OpenGL blends alpha pixels.
        *   It defines that for the source, take the source alpha channel color, and when rendering something on top of
        *   that, take 1 - Src_Alpha;
        *    this will effectively does 1 - Src_Alpha = the Destination colour.
        *   Basically, all pictures' alpha values are set to 1; so there's no blending of pixels; the places
        *   that are meant to be transparent are not.
        *   Doing 1 - Src_Alpha = 0; making those parts that are meant to be transparent but are not, transparent again.
        */

        //  glEnable(GL_BLEND) can be declared with glBelndFunc9) in any order.
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /**
        *   For the OpenGL Core profile, a Vertex Array Object needs to be bound
        *
        *   The below is all that is needed.
        *
        */

        VertexArray vao;


        VertexBuffer vbo(positions, 16 * sizeof(float));
        //  Decided that the Vao should not keep a reference to a Vertex Buffer; it should just bind it to a layout.
        //vao.AddBuffer(vbo);

        VertexBufferLayout layout;
        layout.Push<float>(2u);
        layout.Push<float>(2u);
        vao.AddBuffer(vbo, layout);

        //  The Index Buffer Object
        //  Note, only the count of the indices are passed in. It is already known that they are type
        //  unsigned int
        IndexBuffer ibo(indices, 6);

        //  Create Shader Object
        Shader shader("res/shaders/ep17/Basic.shader");
        shader.Bind();

        //  Setting the value the uniform of that location.
        shader.SetUniform4f("u_Color", 0.65f, 0.08f, 0.58f, 1.0f);
        Texture texture("res/textures/star_rasengan.png");
        texture.Bind();//   No parameter hence 0 by default

        /**
        *   Now, tell the shader which Texture Slot to sample from.
        *   It's done via a Uniform but not an integer slot, instead a Sampler slot.
        *   More specifically, a 2DTextureSampler.
        *   This involves sending an Integer uniform to the shader. This integer is the slot that the
        *   texture has been bound to and that it should be sampled from.
        *   Then in the shader code, use the Sampler/Integer to sample the texture (in the shader)
        *   and do what one wants with it.
        */
        //  This is binding the texture to slot 0
        shader.SetUniform1i("u_Texture", 0);


        /**
        *   @Exp
        *
        *   Unbind Everything so anything: The Program, the Vertex Buffer, and the Index Buffer
        *   So they can be bound only when it's time to draw.
        */
        //GLCall(glUseProgram(0));
        vao.Unbind();
        shader.Unbind();
        vbo.Unbind();
        ibo.Unbind();

        Renderer renderer;

        //  Animating the Color
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            renderer.Clear();

            /**
            *   bind shader -- must bind shader before setting the uniform.
            *   The issue her, which is meant to be taken care of by the Renderer is
            *   the `shader.Bind()` followed by `shader.SetUniform4f`
            *   The issue is this: 1) The shader has to be bound before a Uniform can be set
            *   and 2) normally, it's not the shader that is passed into the Renderer, but rather
            *   a Material.
            *   a Material is a shader and a unique set of data that define how the material to be drawn
            *   by that shader should look like. Hence, the material will include Uniforms to be sent
            *   to that shader and as a result the Renderer is meant to take care of Binding the shader,
            *   setting every Uniform defined in the Materials, and then call the other bindings and 
            *   finally calls the Draw function for the vertex array, rendering everything.
            */
            
            shader.Bind();
            //  setup uniform
            shader.SetUniform4f("u_Color", r, 0.05f, 0.58f, 1.0f);
            //  the Renderer Binds the VAO and IBO and the Shader
            renderer.Draw(vao, ibo, shader);


            if (r > 1.0f)
                increment = -0.01f;
            else if (r < 0.0f)
                increment = 0.01f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        //  No need for this because the shader is destroyed by the destruvtor
        //GLCall(glDeleteProgram(shader_program));
    }

    glfwTerminate();
    return 0;
}
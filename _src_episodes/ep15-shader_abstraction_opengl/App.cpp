/**
*   EP15: "Shader Abstraction in OpenGL"
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
#include "Shader.h"



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
        float positions[] = {
            -0.5f, -0.5f,   //  0
            0.5f, -0.5f,    //  1
            0.5f, 0.5f,     //  2
            -0.5f, 0.5f,    //  3
        };

        //  Specified the indices for each triangle
        //  Note the indices of the triangle arfe drawin in an anticlockwise direction
        //  Then this data is sent ot the GPU for OpenGL to render it
        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle
        };

        /**
        *   For the OpenGL Core profile, a Vertex Array Object needs to be bound
        *
        *   The below is all that is needed.
        *
        */


        VertexArray vao;

        VertexBuffer vbo(positions, 8 * sizeof(float));
        //  Decided that the Vao should not keep a reference to a Vertex Buffer; it should just bind it to a layout.
        //vao.AddBuffer(vbo);

        VertexBufferLayout layout;
        layout.Push<float>(2u);
        vao.AddBuffer(vbo, layout);

        //  The Index Buffer Object
        //  Note, only the count of the indices are passed in. It is already known that they are type
        //  unsigned int
        IndexBuffer ibo(indices, 6);

        //  Parse and Use Shader
        /**
        *   Below, a relative path is specified.
        **/
        Shader shader("res/shaders/ep11-14/Basic.shader");
        shader.Bind();

        /**
        *   Setting up the Uniform.
        *
        *   1.  Retrieve the location of the uniform variable: Once the OpenGL program is created, it assigns an ID to every uniform so that
        *       uniform can be referenced.
        *       The way these Uniforms are looked up is by their names.
        *       So the shader is prompted to get the location of that Uniform variable, which gives an integer back as the response.
        *       But Modern versions of OpenGL allows one to set an index like it was done with:
        *           `
        *               layout(location=0) out vec4 color;
        *           `
        *       So from OpenGL 4.3 one is able to specify an explicit Uniform location.
        *       But this case retrieves it using th ename
        */
        shader.SetUniform4f("u_Color", 0.65f, 0.08f, 0.58f, 1.0f)

        /**
        *   @Exp
        *
        *   Unbind Everything so anything: The Program, the Vertex Buffer, and the Index Buffer
        *   So they can be bound only when it's time to draw.
        */
        vao.Unbind();
        shader.Unbind();
        vbo.Unbind();
        ibo.Unbind();

        //  Animating the Color
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            /**
            *   @Exp
            *   Now, bind them all but no need to bind the buffer itself...
            *   just binding shader_program, vertex array and index buffer.
            *
            *   instead just the Vertex Array Object in its place:
            *       GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
            *   and no need for these:
            *           GLCall(glEnableVertexAttribArray(0));
            *           GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
            */
            //  bind shader
            shader.Bind();
            //  setup uniform
            shader.SetUniform4f("u_Color", r, 0.05f, 0.58f, 1.0f));
            //  Bind just the Vertex Array Object
            //GLCall(glBindVertexArray(vao));
            vao.Bind();
            //  bind index buffer
            ibo.Bind();

            /**
            *   Now, considering the below draw call, on that perhaps it's something to put as part of the Index Buffer class...
            *   If one has a space ship model, the Vertex Buffer will contain every single vertex of that spaceship.
            *   But there will most likely be several index buffers that draw parts of that ship because the wings will likely be
            *   a different material than the cockpit for example...
            *   The wings will probably be metal while the cockpit window glass.
            *   Hence one needs to split drawing these different parts into two different draw calls:
            *       1.  the draw call with the index buffer that just draws the wings
            *       2.  the draw call with the index buffer that just draws the cockpit.
            *   These will be covered when loading 3d models is covered.
            *
            *   So, there is one Vertex Buffer containing the model's vertices, and then several index buffers indexed into that Vertex Buffer.
            *   Hence one could tie the draw call to the index buffer sine theoretically the index buffer specifies which part of the model to draw.
            *
            *   But here, the Renderer will take care of the actual draw call.
            *   It will look at the index buffer, prompt for its count and execute the draw call.
            */

            GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr)); //  <- used with an index buffer.


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

        //  Not needed again because Shadr wrapper class handles this.
        //GLCall(glDeleteProgram(shader_program));
    }

    glfwTerminate();
    return 0;
}
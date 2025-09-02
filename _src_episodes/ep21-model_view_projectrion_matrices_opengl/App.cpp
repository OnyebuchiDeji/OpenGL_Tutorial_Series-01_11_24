/**
*   EP21: "Model View Projection Matrices in OpenGL"
* 
*   A camera matrix is added
*   A model matrix is added.
* 
*   the projection * view * model are multiplied to represent the transformation on the object.
*   These are all that's needed to be known to position objects in a 2D or 3D world.
* 
*   They are used to translate the image horizontally and vertically by 200px
* 
*   The next episodes would look at adding perspective/using a Perspective Projection matrix that maps the 3d points
*   to the 2D normalized coordinates for rendering on the screen.
*   But because of this, a way to update the code is needed using imGUI, to prevent needing to open and close the program continually.
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
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"




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
    window = glfwCreateWindow(960, 540, "Yo! World!", NULL, NULL);
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
        //  x, y, s, t
        //float positions[] = {
        //    -0.5f, -0.5f, 0.0f, 0.0f,   //  0
        //    0.5f, -0.5f, 1.0f, 0.0f,   //  1
        //    0.5f, 0.5f, 1.0f, 1.0f,    //  2
        //    -0.5f, 0.5f, 0.0f, 1.0f   //  3
        //};

        //  Positions after changing window resolution to 960x540
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f,   //  0
            200.0f, 100.0f, 1.0f, 0.0f,   //  1
            200.0f, 200.0f, 1.0f, 1.0f,    //  2
            100.0f, 200.0f, 0.0f, 1.0f   //  3
        };

        //  Specified the indices for each triangle
        //  Note the indices of the triangle arfe drawin in an anticlockwise direction
        //  Then this data is sent ot the GPU for OpenGL to render it
        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle
        };




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
        /**
        *   Orthographic Matrix
        *   The below defines a 4x3 aspect ratio, 2:1.5
        *   The Orthographic Matrix is a way to map all of one's coordinates unto a 2D plane
        *   such that objects that are further away are not actually smaller.
        *   
        *   This is opposed to Persepctive Projection, used in 3D rendering, such that the further an object is from
        *   the camera, the smaller it becomes.
        * 
        *   After creating the Matrice, send it to shader to do the transforms on the Vertices (`positions`). However
        *   this can also be done in the CPU, here, without sending it to the GPU.
        * 
        *   The params:
        *       (left, right, bottom, top, zNear, zFar)
        *   It specifies the boundaries of the Window.
        * 
        *   Any vertex position that is outside these bounds will not be displayed.
        */

        //  When resolution was 640 x 480
        //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        //  Increasing world space but maintaining aspect ratio
        //glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);

        //  When resolution now 960 x 540
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        //glm::vec4 result = proj * vp;
        /**
        *  the whole point is that the `view` is translated to the left...
        *  meaning the camera was moved to the right, causing the coordinates (points)
        *  in the world to move to the left.
        */
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        /**
        *   Now the model matrix represents transforms on the model. In this case, the model is moved 200px to...
        *   the right and upwards.
        *   This is the matrix you use to control each individual model loaded into the world.
        */
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        //  Note it's multiplied proj*view*model because of the memory layout of the array in OpenGL
        // is column-major. So it's right to left.
        glm::mat4 mvp = proj * view * model;


        //  Create Shader Object
        Shader shader("res/shaders/ep20/Basic.shader");
        shader.Bind();

        //  Setting the value the uniform of that location.
        shader.SetUniform4f("u_Color", 0.65f, 0.08f, 0.58f, 1.0f);
        //  Setting the Matric uniform
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/star_rasengan.png");
        texture.Bind();//   No parameter hence 0 by default

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
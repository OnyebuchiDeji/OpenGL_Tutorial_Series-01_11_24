/**
*   EP20: "Projection Matrix in OpenGL"
* 
*   elaboration:
*       Changed the normalized rendering coordinates using a Projection matrix.
*       that is, changed the normalized world coordinates from left -1, right +1, top +1, bottom -1
*       to different coordinates, e.g.
*                1. left -2. right 2, top 1.5, bottom -1.5  -- maintained 4:3 aspect ratio  
*                2. left -4, right 4, top 3, bottom -3      -- maintained 4:3 aspect ratio
*
* 
*       In the original, only points between +1 and -1 could be displayed
*       In the second, points between -2 and 2 for both axis
*       In the third, between -4 and 4
*       
*       Later, after changing screen width x height -> 960, 540...
*       then having changed the normalized coordinates to:
*               left 0, right 960, top 540, bottom 0
*       Only points between 0 and 960 in the x-axis could be visible
*       while only those between 0 and 540 in y-axis could be visible.
* 
*   The concept is not difficult; By default normalized coordinates in OpenGL is left -1, right 1, top 1, bottom -1
*   Usually, in GLSL, this is represented by the `fragCoord` variable in the GLSL language.
*   Then iResolution is the screen's actual resolution.
* 
*   So people would normalize this fragment shader render coordinates to fit what they want, which is what
*   is being done in this episode.
* 
*   Examples of what people do:
*   
*   normalized_coord = (fragCoord - 0.5*iResolution.xy) / iResolution.y
*   if iResolution = (x{800}, y{450})...
*   new_ires = 0.5 * (800, 450) = 400, 225
*   new_fragCoord = fragCoord - new_ires ->> left -400, right 400, top 225, bottom -225
*   
*   new_fragCoord / iResolution.y ->> left -0.89, right 0.89, top 0.5, bottom -0.5
* 
*   So in the end, the goal is to make a normalized_coord that can accommodate a larger range of values or points
*   so that it is mapped to the original fragCoord by converting that value to be between the OG:
*       left -1, right +1, top +1, bottom -1, as explained above and demonstrated below.
* 
*   Because if you see what the values here:
*           glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
*           glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
*
*           glm::vec4 result = proj * vp
* 
*   It is seen that `result`'s x and y values are:
* 
*           x 100 -> x = -0.791666627
*           y 100 -> y = -0.629629612
*   This kind of Projection matrix is a Pixel-Space Projection because every unit is one pixel.
*   So, this whole process simply specifies a Projection so that it can accommodate points withing that projection's range.
*   Then, the Projection Matrix converts each point it multiplies to fit the permanent normalized render coordinate system
*   that the GLSL language uses, left -1, right +1, top +1, bottom -1.
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
        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);

        glm::vec4 result = proj * vp;


        //  Create Shader Object
        Shader shader("res/shaders/ep20/Basic.shader");
        shader.Bind();

        //  Setting the value the uniform of that location.
        shader.SetUniform4f("u_Color", 0.65f, 0.08f, 0.58f, 1.0f);
        //  Setting the Matric uniform
        shader.SetUniformMat4f("u_MVP", proj);

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
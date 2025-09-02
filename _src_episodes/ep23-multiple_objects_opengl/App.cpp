/**
*   EP23: "Rendering Multiple Objects in OpenGL"
* 
*   HEre the Vertex Points are fixed so the object works well
*   
*/

//  External
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
//#include <imgui/imconfig.h>
//#include <imgui/imstb_rectpack.h>
//#include <imgui/imstb_textedit.h>
//#include <imgui/imstb_truetype.h>

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
//#include "glm/gtx/io.hpp"
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

        //  Positions after changing window resolution to 960x540
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,   //  0
             50.0f, -50.0f, 1.0f, 0.0f,   //  1
             50.0f,  50.0f, 1.0f, 1.0f,    //  2
            -50.0f,  50.0f, 0.0f, 1.0f   //  3
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
        
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, 0.0f));
   

        //  Create Shader Object
        Shader shader("res/shaders/ep20/Basic.shader");
        shader.Bind();

        //  Setting the value the uniform of that location.
        shader.SetUniform4f("u_Color", 0.65f, 0.08f, 0.58f, 1.0f);

        //  Setting the Matric uniform -- removed and put in loop so that the model
        //  matrix can be continously recalculated
        //shader.SetUniformMat4f("u_MVP", mvp);

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

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


        //  The vec3 used in the Model view matrix to translate the model
        glm::vec3 translationA = glm::vec3(200, 200, 0);
        glm::vec3 translationB = glm::vec3(400, 200, 0);

        //  Animating the Color
        float r = 0.0f;
        float increment = 0.05f;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        const char* glsl_version = "#version 130";
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            


            /**
            *   Although calling Shader.Bind() when a Shader has already been bound can affect performance, it's still done here.
            *   just for semantic assurance.
            * 
            *   Normally, a caching system that checks if the Shader has already been bound will be implemented in the Bind() method
            *   to prevent rebinding an already bound shader, but here it isn't.
            */
            {
                //  setup uniform
                //shader.SetUniform4f("u_Color", r, 0.05f, 0.58f, 1.0f);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
            
                //  Specifying different MVP matrix to render more than one models
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                //  the Renderer Binds the VAO and IBO and the Shader
                renderer.Draw(vao, ibo, shader);

            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(vao, ibo, shader);
            }


            if (r > 1.0f)
                increment = -0.01f;
            else if (r < 0.0f)
                increment = 0.01f;

            r += increment;


            //  A Simple Custom Window
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Window 1");                          // Create a window called "Hello, world!" and append into it.

                /**
                *   The below is not the ordinary SliderFloat
                *   Rather, it's to manipulate an array of 3 values
                *   But it needs a pointer to the memory address of a float array...
                *   With this it can get the rest of the memory layout, since they're all floats.
                *   0.0 and 960.0 are the min and max values for the float
                *   The downside is that each slide generated uses this same min and max
                *   
                *   ImGui::SliderFloat3("float", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 960.0f
                *   
                *   So the Up and down one likewise uses this, which is not the best.
                *   Hence why I changed it to use separate SliderFloats
                */
                ImGui::SliderFloat("x_slider_A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat("y_slider_A", &translationA.y, 0.0f, 960.0f);

                ImGui::SliderFloat("x_slider_B", &translationB.x, 0.0f, 960.0f);
                ImGui::SliderFloat("y_slider_B", &translationB.y, 0.0f, 540.0f);
                
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
        //  Imgui Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        //  No need for this because the shader is destroyed by the destruvtor
        //GLCall(glDeleteProgram(shader_program));
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
#include <iostream>

#include "TestBatchRendering.h"


namespace test
{

    TestBatchRendering::TestBatchRendering()
        : m_Name{ "Batch Rendering Test" }, m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_TranslationA(200, 200, 0)//, m_TranslationB(400, 200, 0)
    {
        
        //float positions[] = {
        //     -100.0f, -100.0f, 0.0f, 0.0f,   //  0
        //     100.0f, -100.0f, 1.0f, 0.0f,   //  1
        //     100.0f,  100.0f, 1.0f, 1.0f,    //  2
        //    -100.0f,  100.0f, 0.0f, 1.0f   //  3
        //};

        //float positions[] = {
        //    -50.0f, -50.0f,
        //     50.0f, -50.0f,
        //     50.0f,  50.0f,
        //    -50.0f, 50.0f,
        //};

        float positions[] = {

             -50.0f, -50.0f,
             50.0f, -50.0f,
             50.0f,  50.0f,
            -50.0f, 50.0f,

             70.0f, -50.0f,
             170.0f, -50.0f,
             170.0f, 50.0f,
            70.0f, 50.0f,
        };

        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle for first square

            4, 5, 6, 6, 7, 4 // second square
        };

        /*GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));*/

        m_VAO = std::make_unique<VertexArray>();

        m_VBO = std::make_unique<VertexBuffer>(positions, 8 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2u);
        //layout.Push<float>(2u);
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IBO = std::make_unique<IndexBuffer>(indices, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/ep28/BasicBatch.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", glm::vec4(0.65f, 0.08f, 0.58f, 1.0f));
        //m_Texture = std::make_unique<Texture>("res/textures/star_rasengan.png");

        //  No need to bind the texture here yet
        //m_Shader->SetUniform1i("u_Texture", 0);
    }

    TestBatchRendering::~TestBatchRendering()
    {
        std::cout << m_Name << " Closed!\n";
    }

    void TestBatchRendering::OnUpdate(float deltaTime)
    {
    }

    void TestBatchRendering::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        //m_Texture->Bind();
        
        //  setup uniform
        //shader.SetUniform4f("u_Color", r, 0.05f, 0.58f, 1.0f);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
        glm::mat4 mvp = m_Proj * m_View * model;

        //  Specifying different MVP matrix to render more than one models
        m_Shader->Bind();
        m_Shader->SetUniformMat4("u_MVP", mvp);
        //  the Renderer Binds the VAO and IBO and the Shader
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

    }
    void TestBatchRendering::OnImGuiRender()
    {
        ImGui::SliderFloat("x_slider_A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat("y_slider_A", &m_TranslationA.y, 0.0f, 540.0f);

        /*ImGui::SliderFloat("x_slider_B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::SliderFloat("y_slider_B", &m_TranslationB.y, 0.0f, 540.0f);*/

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    }
}

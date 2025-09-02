#include <iostream>
#include "TestBatchRenderingTextures.h"

#include "stb_image/stb_image.h"

static GLuint LoadTexture(const std::string& path)
{
    int w, h, bits;

    stbi_set_flip_vertically_on_load(1);
    //  For last parameter, could use STBI_rgb -> 3
    auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, 4);
    GLuint textureID;

    GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    //  So for this, would use GL_RGBA8
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

    stbi_image_free(pixels);

    return textureID;
    
}
namespace test
{

    TestBatchRenderingTextures::TestBatchRenderingTextures()
        : m_Name{ "Batch Rendering Test - Textures" }, m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_TranslationA(200, 200, 0)//, m_TranslationB(400, 200, 0)
    {
        

        /**
        *   Specifying vertivces in format:
        *       x, y, r, g, b, a, u, v
        */
        /*
        float vertices[] = {

             -50.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 0.0f,
             50.0f,  -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 0.0f,
             50.0f,   50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 1.0f,
             -50.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 1.0f,

             70.0f,  -50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
             170.0f, -50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
             170.0f,  50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             70.0f,   50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            
             190.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 0.0f,
             290.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 0.0f,
             290.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 1.0f,
             190.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 1.0f,
        };
        */

        /**
        *   Specifying vertivces in format:
        *       x, y, r, g, b, a, u, v, tex_index
        */
        
        float vertices[] = {
            -50.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 0.0f,  0.0f,
             50.0f,  -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 0.0f, 0.0f,
             50.0f,   50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 1.0f, 0.0f,
             -50.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 1.0f, 0.0f,

             70.0f,  -50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
             170.0f, -50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             170.0f,  50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             70.0f,   50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

             190.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 0.0f, 2.0f,
             290.0f, -50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 0.0f, 2.0f,
             290.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 1.0f, 1.0f, 2.0f,
             190.0f,  50.0f, 0.18f, 0.95f, 0.55f, 1.0f, 0.0f, 1.0f, 2.0f,
        };
        
        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle for first 

            4, 5, 6, 6, 7, 4, // second square
            8, 9, 10, 10, 11, 8 // third square
        };

        /*GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));*/

        m_VAO = std::make_unique<VertexArray>();

        /**
        *   Displays the three quads, using the 'vertices' vertex data
        *   and the normal 'BasicBatch-Textures.shader'
        *   in the way below:
        */
        /*
            m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
            VertexBufferLayout layout;
            layout.Push<float>(2u);
            layout.Push<float>(4u);
            layout.Push<float>(2u);
            m_VAO->AddBuffer(*m_VBO, layout);

            m_IBO = std::make_unique<IndexBuffer>(indices, 18);

            m_Shader = std::make_unique<Shader>("res/shaders/ep28/BasicBatch-Textures.shader");
        */
        
        m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(2u); //  x y
        layout.Push<float>(4u); //  r g b a
        layout.Push<float>(2u); //  u v
        layout.Push<float>(1u); //  tex index
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IBO = std::make_unique<IndexBuffer>(indices, 18);

        m_Shader = std::make_unique<Shader>("res/shaders/ep28/BasicBatch-Textures_v2.shader");
        
        m_Shader->Bind();

        //  Loading Textures

        m_Tex1 = LoadTexture("res/textures/morning glory.jpg");
        m_Tex2 = LoadTexture("res/textures/T-REX.png");
        m_Tex3 = LoadTexture("res/textures/red_diamond_heart.png");

        //int samplers[] = { (int)m_MorningGloryTex, (int)m_RasenganTex, (int)m_DaisyTex };
        int samplers[] = { 0, 1, 2 };
        m_Shader->SetUniform1iv("u_Textures", samplers);

        std::cout << "Tex1: " << (unsigned int)m_Tex1 << '\n';
        std::cout << "Tex2: " << (unsigned int)m_Tex2 << '\n';
        std::cout << "Tex3: " << (unsigned int)m_Tex3 << '\n';
    }

    TestBatchRenderingTextures::~TestBatchRenderingTextures()
    {
        std::cout << m_Name << " Closed!\n";
    }

    void TestBatchRenderingTextures::OnUpdate(float deltaTime)
    {
    }

    void TestBatchRenderingTextures::OnRender()
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
        //  These indices here, the first arguments are the Textures'IDs
        //  They should correspond to the Texture ID in the Vertex Data.
        glBindTextureUnit(0, m_Tex1);
        glBindTextureUnit(1, m_Tex2);
        glBindTextureUnit(2, m_Tex3);

        m_Shader->SetUniformMat4("u_MVP", mvp);
        //  the Renderer Binds the VAO and IBO and the Shader
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

    }
    void TestBatchRenderingTextures::OnImGuiRender()
    {
        ImGui::SliderFloat("x_slider_A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat("y_slider_A", &m_TranslationA.y, 0.0f, 540.0f);

        /*ImGui::SliderFloat("x_slider_B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::SliderFloat("y_slider_B", &m_TranslationB.y, 0.0f, 540.0f);*/

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    }
}

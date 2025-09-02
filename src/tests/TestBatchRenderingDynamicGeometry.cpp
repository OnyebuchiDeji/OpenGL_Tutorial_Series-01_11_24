#include <iostream>
#include "TestBatchRenderingDynamicGeometry.h"

#include "stb_image/stb_image.h"
#include <array>

//  Because Arrays are non-assignable
struct Vec6
{
    unsigned int x, y, z, w, u, v;
};

struct Vec2
{
    float x, y;
};

struct Vec3
{
    float x, y, z;
};

struct Vec4
{
    float x, y, z, w;
};

struct Vertex
{
    Vec2 Position;
    Vec4 Color;
    Vec2 TexCoords;
    float TexID;
};

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

    TestBatchRenderingDynamicGeometry::TestBatchRenderingDynamicGeometry()
        : m_Name{ "Batch Rendering Test - Textures" }, m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_TranslationA(200, 200, 0)//, m_TranslationB(400, 200, 0)
    {



        /**
        *   Specifying vertivces in format:
        *       x, y, r, g, b, a, u, v, tex_index
        */
        
        /*
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
        };*/

        /*
        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle for first 

            4, 5, 6, 6, 7, 4, // second square
            8, 9, 10, 10, 11, 8 // third square
        };*/

        m_VAO = std::make_unique<VertexArray>();
        //  Using this as an allocation function.
        /**
        *   1.  Make it point to nullptr rather than an array
        *   2.  Typing 1024 -> 1kb.
        *   
        *   Below creates a buffer able to store 1000 Vertices.
        *   The flase specifies that it's dunamic
        */
        m_VBO = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 1000, false);

        VertexBufferLayout layout;
        layout.Push<float>(2u); //  x y
        layout.Push<float>(4u); //  r g b a
        layout.Push<float>(2u); //  u v
        layout.Push<float>(1u); //  tex index
        m_VAO->AddBuffer(*m_VBO, layout);

        m_IBO = std::make_unique<IndexBuffer>(nullptr, 6 * 1000, false);

        m_Shader = std::make_unique<Shader>("res/shaders/ep28/BasicBatch-Textures_v2.shader");
        
        m_Shader->Bind();

        //  Loading Textures

        m_Tex1 = LoadTexture("res/textures/star_rasengan.png");
        m_Tex2 = LoadTexture("res/textures/T-REX.png");
        m_Tex3 = LoadTexture("res/textures/red_diamond_heart.png");
        m_Tex4 = LoadTexture("res/textures/morning glory.jpg");
        m_Tex5 = LoadTexture("res/textures/shark.jpg");

        //int samplers[] = { (int)m_MorningGloryTex, (int)m_RasenganTex, (int)m_DaisyTex };
        int samplers[] = { 0, 1, 2, 3, 4 };
        m_Shader->SetUniform1iv("u_Textures", samplers);

        std::cout << "Tex1: " << (unsigned int)m_Tex1 << '\n';
        std::cout << "Tex2: " << (unsigned int)m_Tex2 << '\n';
        std::cout << "Tex3: " << (unsigned int)m_Tex3 << '\n';
        std::cout << "Tex4: " << (unsigned int)m_Tex4 << '\n';
        std::cout << "Tex5: " << (unsigned int)m_Tex5 << '\n';
    }

    TestBatchRenderingDynamicGeometry::~TestBatchRenderingDynamicGeometry()
    {
        std::cout << m_Name << " Closed!\n";
    }

    void TestBatchRenderingDynamicGeometry::OnUpdate(float deltaTime)
    {
    }

    /**
    *   This function returns 4 vertices needed to render a quad, one
    *   that modifies the x and y values according to the arguments specified.
    *   Hence it controls the quad''s position
    */
    static std::array<Vertex, 4> CreateQuad(float x, float y, float length, glm::vec4 color, float texId=0.0f)
    {
        //  Defines that a quad should be a 1x1 unit quad
        //float size = length;

        Vertex v0;
        v0.Position = { x, y };
        v0.Color = { color.r, color.g, color.b, color.a };
        v0.TexCoords = { 0.0f, 0.0f };
        v0.TexID = texId;
        
        Vertex v1;
        v1.Position = { x + length, y};
        v1.Color = { color.r, color.g, color.b, color.a };
        v1.TexCoords = { 1.0f, 0.0f };
        v1.TexID = texId;
        
        Vertex v2;
        v2.Position = { x + length,   y + length};
        v2.Color = { color.r, color.g, color.b, color.a };
        v2.TexCoords = { 1.0f, 1.0f };
        v2.TexID = texId;
        
        Vertex v3;
        v3.Position = { x,  y + length};
        v3.Color = { color.r, color.g, color.b, color.a };
        v3.TexCoords = { 0.0f, 1.0f };
        v3.TexID = texId;

        return { v0, v1, v2, v3 };
    }


    static void CreateQuadIndices(int numOfIndicesPerQuad, unsigned int numOfQuads, unsigned int*& outArray)
    {
        //   Empty Array
        //static unsigned int prevCount = numOfQuads;
        /*if (prevCount != numOfQuads)
        {*/
        //delete[] outArray;
        //outArray = new unsigned int[numOfIndicesPerQuad * (int)numOfQuads] {};
        //}

        unsigned int noipq = numOfIndicesPerQuad;
        /*   0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle for first 

            4, 5, 6, 6, 7, 4, // second square
            8, 9, 10, 10, 11, 8 // third square*/
        //  The outer keeps track of which quad's indices its drawing
        for (unsigned int i= 0; i < numOfQuads; i+=1)
        {
            unsigned int indexStride = i * numOfIndicesPerQuad;
            unsigned int valueStride = i * 4;
            //std::cout << indexStride;
            for (unsigned int j = 0; j < noipq; j++)
            {
                if (j < noipq / 2)
                {
                    outArray[j + indexStride] = j + valueStride;
                }
                else if(j >= noipq /2 && j != noipq - 1)
                {
                    outArray[j + indexStride] = (j - 1) + valueStride;
                    //std::cout << "Ran!\n";
                }
                else
                {
                    outArray[j + indexStride] = outArray[indexStride];

                }
            }
        }
        //prevCount = numOfQuads;
    }
   
    static void printArray(unsigned int*& arrayRef, int size)
    {
        static int once = 0;
        int count = 0;
        //  Print every 50 frames
        if (once % 100 == 0)
        {
            for (unsigned int i = 0; i < 1000u; i++)
            {
                if (arrayRef[i] < 1000u)
                {
                    count += 1;
                }
                else
                {
                    break;
                }
            }
            for (int i = 0; i < size; i++)
            {
                

                if (i + 1 % 5 == 0)
                {
                    std::cout << arrayRef[i] << "\n";
                    continue;
                }
                else
                {
                    std::cout << arrayRef[i] << " ";
                }
            }
           std::cout << "\nSize is: " << count << "\n";
        }
        once += 1;
    }

    void TestBatchRenderingDynamicGeometry::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        //GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer renderer;
        renderer.Clear();

        //  Acts just like before.
        //  Now these values can be controlled.
        /*
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
        };*/
        /*
        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle for first 

            4, 5, 6, 6, 7, 4, // second square
            8, 9, 10, 10, 11, 8 // third square
        };
        */

        //  Dynamically making 5 vertices data
        Vertex vertices[20];

        auto q0 = CreateQuad(m_QuadPosition1[0], m_QuadPosition1[1], 100.0f, glm::vec4(0.55f, 0.12f, 0.67f, 1.0f), 0.0f);
        std::array<Vertex, 4> q1 = CreateQuad(m_QuadPosition2[0], m_QuadPosition2[1], 100.0f, glm::vec4(0.55f, 0.12f, 0.67f, 1.0f), 1.0f);
        memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
        if (m_QuadCount > 2)
        {
            auto q2 = CreateQuad(m_QuadPosition3[0], m_QuadPosition3[1], 100.0f, glm::vec4(0.55f, 0.12f, 0.67f, 1.0f), 2.0f);
            auto q3 = CreateQuad(m_QuadPosition4[0], m_QuadPosition4[1], 100.0f, glm::vec4(0.55f, 0.12f, 0.67f, 1.0f), 2.0f);
            auto q4 = CreateQuad(m_QuadPosition5[0], m_QuadPosition5[1], 100.0f, glm::vec4(0.55f, 0.12f, 0.67f, 1.0f), 1.0f);
            memcpy(vertices + q0.size() + q1.size(), q2.data(), q2.size() * sizeof(Vertex));
            memcpy(vertices + q0.size() + q1.size() + q2.size(), q3.data(), q3.size() * sizeof(Vertex));
            memcpy(vertices + q0.size() + q1.size() + q2.size() + q3.size(), q4.data(), q4.size() * sizeof(Vertex));
        }


        //  Dynamically making 5 indices data;

        unsigned int numOfIndicesPerQuad = 6;
        unsigned int* indices = new unsigned int[(int)numOfIndicesPerQuad * m_QuadCount];

        //printArray(indices, (int)numOfIndicesPerQuad * m_QuadCount);
        CreateQuadIndices(numOfIndicesPerQuad, (unsigned int)m_QuadCount, indices);
        printArray(indices, (int)numOfIndicesPerQuad * m_QuadCount);
        

        static int once = 0;
        
        //  Display every 100 frames
        if (once % 100 == 0)
        {
            std::cout << "Size of Vertex: " << sizeof(Vertex) << "\n";
            std::cout << "Size of Vertices: " << sizeof(vertices) << "\n";
            std::cout << "Size of Indices: " << sizeof(indices) << "\n";
            std::cout << "Size of Unsigned Int: " << sizeof(unsigned int) << "\n";
            std::cout << "No. Of Indices: " << (unsigned int)m_QuadCount * numOfIndicesPerQuad << "\n";
            std::cout << "Calculated Size: " << sizeof(indices) * (unsigned int)m_QuadCount * numOfIndicesPerQuad << "\n";
        }
        once += 1;

        m_VBO->Bind();
        //m_IBO->Bind();

        /**
        * 
        *  The below returns a void * to memory one can directly
        *  write into.
        *  Then at the end of the function, call glUnmapBuffer() which will write it
        *  to the GPU -- though this becomes slightly slower than the method presented here.
        */
        //glMapBuffer()
        /**
        *  The below is faster because unlike glMapBuffer(), it does not allocate memory
        *  for the data before sending it to the target.
        *  It just sends the data directly.
        * 
        */

        
        //  this effectively populates the Vertex Buffer with the vertex data every frame.
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

        //  To dynamically add the index buffer data:
        //  Could do:
        //GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, (unsigned int)m_QuadCount * numOfIndicesPerQuad * sizeof(unsigned int), indices));
        //  OR:
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices) * (unsigned int)m_QuadCount * numOfIndicesPerQuad , indices));




        
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
        glBindTextureUnit(3, m_Tex4);
        glBindTextureUnit(4, m_Tex5);

        m_Shader->SetUniformMat4("u_MVP", mvp);
        //  the Renderer Binds the VAO and IBO and the Shader
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

        /*m_VBO->Unbind();
        m_IBO->Unbind();
        m_Shader->Unbind();*/

        delete[] indices;

    }

    void TestBatchRenderingDynamicGeometry::OnImGuiRender()
    {
        ImGui::SliderFloat("x_slider_A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat("y_slider_A", &m_TranslationA.y, 0.0f, 540.0f);

        ImGui::DragFloat2("Quad 1 Position", m_QuadPosition1, 0.5f);
        ImGui::DragFloat2("Quad 2 Position", m_QuadPosition2, 0.5f);
        ImGui::DragFloat2("Quad 3 Position", m_QuadPosition3, 0.5f);
        ImGui::DragFloat2("Quad 4 Position", m_QuadPosition4, 0.5f);
        ImGui::DragFloat2("Quad 5 Position", m_QuadPosition5, 0.5f);

        ImGui::SliderInt("Quad Count", &m_QuadCount, 0, 5);

        /*ImGui::SliderFloat("x_slider_B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::SliderFloat("y_slider_B", &m_TranslationB.y, 0.0f, 540.0f);*/

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    }
}

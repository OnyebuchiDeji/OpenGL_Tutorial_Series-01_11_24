#pragma once

#include "Test.h"
#include "Renderer.h"
//	all the below are included in 'Renderer.h'
//#include "Shader.h"
//#include "VertexBuffer.h"
//#include "IndexBuffer.h"
//#include "VertexArray.h"
//#include "VertexBufferLayout.h"


namespace test
{
	/**
	*	Legit shows the rendering of a square changing colors.
	*/
	class BasicRendererTest : public Test
	{
	public:
		BasicRendererTest(Renderer& renderer);
		~BasicRendererTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float positions[8] = {
			-0.5f, -0.5f,   //  0
			0.5f, -0.5f,    //  1
			0.5f, 0.5f,     //  2
			-0.5f, 0.5f,    //  3
		};

		unsigned int indices[6] {
			0 ,1, 2,    //  First Triangle
			2, 3, 0,    //  Second Triangle
		};

		const char* m_Name;
		float m_ColorDelta;
		float m_DeltaIncrement;
		const char* m_ShaderPath;
		float m_R;
		float m_G;
		float m_B;

		Renderer& m_RendererRef;
		Shader m_Shader;
		VertexArray m_VAO;
		VertexBuffer m_VBO = VertexBuffer(positions, 8 * sizeof(float));
		IndexBuffer m_IBO = IndexBuffer(indices, 6);
		VertexBufferLayout m_VBL;
	};



}
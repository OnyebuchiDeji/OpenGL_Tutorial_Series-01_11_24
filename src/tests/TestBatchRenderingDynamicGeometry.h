#pragma once
#pragma once

#include <memory>

#include "Test.h"

#include "Renderer.h"
#include "imgui/imgui.h"
//#include "Texture.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test
{
	class TestBatchRenderingDynamicGeometry: public Test
	{
	public:
		TestBatchRenderingDynamicGeometry();
		~TestBatchRenderingDynamicGeometry();


		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

		const char* m_Name;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<Shader> m_Shader;
		//std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA;//, m_TranslationB;
		float m_QuadPosition1[2] = { -50.0f, -50.0f };
		float m_QuadPosition2[2] = { 70.0f, -50.0f };
		float m_QuadPosition3[2] = { 190.0f, -50.0f };
		float m_QuadPosition4[2] = { 310.0f, -50.0f };
		float m_QuadPosition5[2] = { 430.0f, -50.0f };
		int m_QuadCount = 2;

		GLuint m_Tex1, m_Tex2, m_Tex3, m_Tex4, m_Tex5;
	};
}
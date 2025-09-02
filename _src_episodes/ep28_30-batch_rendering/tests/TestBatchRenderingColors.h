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
	class TestBatchRenderingColors: public Test
	{
	public:
		TestBatchRenderingColors();
		~TestBatchRenderingColors();


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
	};
}
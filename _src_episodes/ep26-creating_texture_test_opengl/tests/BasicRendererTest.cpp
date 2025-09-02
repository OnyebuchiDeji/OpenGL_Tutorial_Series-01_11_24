#include "BasicRendererTest.h"

#include "imgui/imgui.h"
#include <iostream>

namespace test
{
	BasicRendererTest::BasicRendererTest(Renderer& renderer)
		: m_Name{ "Basic Renderer Test" }, m_ColorDelta{ 0.0f }, m_DeltaIncrement{ 0.05f },
		m_ShaderPath{ "res/shaders/ep11-16/Basic.shader" }, m_Shader{ Shader(m_ShaderPath) },
		m_RendererRef{ renderer }, m_R{ 1.0f }, m_G{ 0.07 }, m_B{0.58}
	{
		m_VBL.Push<float>(2u);
		m_VAO.AddBuffer(m_VBO, m_VBL);
		m_Shader.Bind();
		m_Shader.SetUniform4f("u_Color", 0.65f, 0.08f, 0.58f, 1.0f);

		m_VAO.Unbind();
		m_Shader.Unbind();
		m_VBO.Unbind();
		m_IBO.Unbind();
	}

	BasicRendererTest::~BasicRendererTest()
	{
		std::cout << "Basic Renderer Test Closed!\n";
	}

	void BasicRendererTest::OnUpdate(float deltaTime)
	{
		static int sign = 1.0f;
		if (m_ColorDelta > 1.0f)
			sign = -1.0f;
		else if (m_ColorDelta < 0.0f)
			sign = 1.0f;
	
		m_ColorDelta += sign * m_DeltaIncrement;
	}

	void BasicRendererTest::OnRender()
	{
		m_Shader.Bind();
		m_Shader.SetUniform4f("u_Color", m_R * m_ColorDelta, m_G * m_ColorDelta + 0.01, m_B * m_ColorDelta + 0.01, 1.0f);
		m_RendererRef.Draw(m_VAO, m_IBO, m_Shader);
	}

	void BasicRendererTest::OnImGuiRender()
	{
		ImGui::Begin(m_Name);
		ImGui::SliderFloat("R", &m_R, 0.0f, 1.0f);
		ImGui::SliderFloat("G", &m_G, 0.0f, 1.0f);
		ImGui::SliderFloat("B", &m_B, 0.0f, 1.0f);
		ImGui::SliderFloat("Increment", &m_DeltaIncrement, 0.05f, 0.1f);
		ImGui::End();
	}

	void BasicRendererTest::OnInit()
	{

	}

	void BasicRendererTest::OnRun()
	{
		OnUpdate(0.0f);
		OnRender();
		OnImGuiRender();
	}
}
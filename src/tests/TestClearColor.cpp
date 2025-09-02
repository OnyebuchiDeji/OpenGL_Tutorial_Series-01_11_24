#include <iostream>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "TestClearColor.h"

namespace test
{
	TestClearColor::TestClearColor()
		: m_ClearColor{ 0.1f, 0.23f, 0.29f, 1.0f }, m_Name{"Clear Color Test"}
	{
	}
	TestClearColor::~TestClearColor()
	{
		std::cout << m_Name << "Clear Color Test Closed!\n";
	}
	void TestClearColor::OnUpdate(float deltaTime)
	{
	}
	void TestClearColor::OnRender()
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

	}
	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}
}
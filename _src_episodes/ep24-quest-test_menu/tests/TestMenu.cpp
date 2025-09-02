

#include "TestMenu.h"
#include "TestClearColor.h"
#include "BasicRendererTest.h"
#include "imgui/imgui.h"


TestMenu::TestMenu(Renderer& renderer)
	: m_CurrentTestObj{ nullptr }, m_MenuOpen{ true }, m_ClearColor{ 0.0f, 0.0f, 0.0f, 1.00f }, m_RendererRef{renderer}
{
}

TestMenu::~TestMenu()
{
}

void TestMenu::Run()
{
	if (m_CurrentTestObj)
		m_CurrentTestObj->OnRun();

	ConstructGUI();
}


void TestMenu::ConstructGUI()
{
	ImGui::Begin("Tests Menu");

	if (ImGui::Button("Close Test"))
		CloseTest();
	if (ImGui::Button("Test1-TestClearColor"))
	{
		InitTestClass(Tests::ClearColorTest);
	}
	if (ImGui::Button("Test2-BasicRendererTest"))
	{
		InitTestClass(Tests::BasicRendererTest);
	}

	ImGui::End();

}

void TestMenu::InitTestClass(Tests testChoice)
{
	using namespace test;
	switch (testChoice)
	{
	case Tests::ClearColorTest:
		m_CurrentTestObj = new TestClearColor();
		break;
	case Tests::BasicRendererTest:
		m_CurrentTestObj = new BasicRendererTest(m_RendererRef);
		break;
	/*case Tests::RenderSquare:
		m_CurrentTestObj = &*/

	};

}

/**
*	Closing the test is as simple as making
*	the pointer point to null.
*	This will remove any reference to that object, thereby
*	calling it's destructor.
*/
void TestMenu::CloseTest()
{
	delete m_CurrentTestObj;
	m_CurrentTestObj = nullptr;
	Reset();
}

void TestMenu::Reset()
{
	GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

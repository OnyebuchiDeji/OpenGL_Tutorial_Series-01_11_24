#pragma once

#include "Test.h"
#include "Renderer.h"

enum class Tests : int
{
	None = -1, ClearColorTest, BasicRendererTest
};

class TestMenu
{
private:
	//	This sets up the menu widgets
	void ConstructGUI();
	//	Does the logic of responding to 
	void InitTestClass(Tests testChoice);
	//	Called when the Back button is clicked; it destroys the current test.
	void CloseTest();
	//	Resets everything, clearing screen display to OG
	void Reset();

public:
	TestMenu(Renderer& renderer);
	~TestMenu();
	
	//	This is the method that will be called to bring everything together;
	//	it should be called after ImGui is initialized
	void Run();

private:
	test::Test* m_CurrentTestObj;
	bool m_MenuOpen;
	float m_ClearColor[4];
	Renderer& m_RendererRef;
};
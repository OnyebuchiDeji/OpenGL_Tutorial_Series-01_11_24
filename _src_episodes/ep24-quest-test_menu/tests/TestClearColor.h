#pragma once


#include "Test.h"


namespace test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();


		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnInit() override;
		void OnRun() override;

	private:
		//	color to clear the screen to
		//	it's allocated on the stack so destroyed automatically.
		float m_ClearColor[4];
		const char* m_Name;
	};
}
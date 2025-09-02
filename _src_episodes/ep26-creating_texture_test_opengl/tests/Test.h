#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>

namespace test {
	
	/**
	* 
	*	This will be a base class for many subclasses...
	*	it takes care of initializing ImGui and such.
	* 
	*	The subclasses will be created when the test is selected and destroyed
	*	when the text is destroyed
	*/
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		//	This will run at a fixe framerate speified by deltaTime
		virtual void OnUpdate(float deltaTime) {}
		//	This is for rendering
		virtual void OnRender() {}
		//	This is where the ImGui things will be drawn.
		virtual void OnImGuiRender() {}

	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		//	This is where the ImGui things will be drawn.
		void OnImGuiRender() override;

		
		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test " << name << "\n";
			//	Returns the object of that type.
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

	private:
		Test*& m_CurrentTest;
		/**
		*	Now, this vector matches each test to its name.
		* 
		*	But the function desired is that when the Button for a Test is pressed
		*	the appropriate Test object should be constructed.
		*	This construction is done by the Lambda.
		*	Then when the Back button is clicked, the object should be deleted
		*/
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};

}
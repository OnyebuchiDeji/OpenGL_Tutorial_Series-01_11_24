#pragma once


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

}
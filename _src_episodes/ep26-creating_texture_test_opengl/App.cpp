/**
*   EP26: "Creating a Texture Test in OpenGL"
*
*   Demonstrates creating a Test that shows rendering a Texture on a Quad.
*/

//  External
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
//#include <imgui/imconfig.h>
//#include <imgui/imstb_rectpack.h>
//#include <imgui/imstb_textedit.h>
//#include <imgui/imstb_truetype.h>

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
//#include "glm/gtx/io.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

int main(void)
{
	/**
	*   From the GLFW library
	*/
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/**
	*   @Exp
	*   By specifying these pieces of code, these are done:
	*       The major version 3 and minor version 3, hence 3.3
	*   Also Core OPENGL Profile is being used.
	*   This profile requires that a VertexArrayObject be made explicitely.
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	/**
	*   The COMPAT profile is the one specified above by the major and minor versions, 3.3
	*   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	*   But Core ensures that no VertexArrayObject is made and bound implicitely.
	*/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Yo! World!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Syncing Frame Rate */
	glfwSwapInterval(1);

	//  Initialize GLEW; now one can use the GL Functions.
	//  All the functions are function pointers.
	if (glewInit() != GLEW_OK)
		std::cout << "Error!\n";

	//  Print out version.
	std::cout << glGetString(GL_VERSION) << "\n";

	{
		//  glEnable(GL_BLEND) can be declared with glBelndFunc9) in any order.
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


		Renderer renderer;

		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		const char* glsl_version = "#version 130";
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		//  The current test
		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		//  The below is because we want the display to start with the menu first.
		currentTest = testMenu;

		/**
		*   Functiont to create Tests
		*
		*   //  The function is templated.
		*/
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");


		//test::TestClearColor test;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */

			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				//  If this back button is called, t
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}


			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;

		//  Imgui Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

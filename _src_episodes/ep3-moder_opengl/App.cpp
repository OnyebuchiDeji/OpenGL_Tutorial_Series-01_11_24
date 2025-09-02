/**
*   EP3: "Modern OpenGL"
*   Included GLEW
*   Printed the Modern OpenGL version
* 
*   Now, GLEW must be included before GLFW because else it will raise an error.
*   The error is a `#error`, a compile time error that makes one have to include GLEW's library before GLFW/
* 
*   Also, at firs tcalling glewInit(), there was a Link error occuring.
*   ON investigating glewInit() in 'glew.h', it has this MACRO specification: GLEWAPI
*   ON investigating GLEWAPI, one will see that the reason for the build error is because the MACRO's
*       if conditions resolves that the build type for GLEW should be for its dynamic library.
*       this happens because the GLEW_STATIC macro was not defined, instead it was GLEW_BUILD (which is for glew's dynamic library):
*       ```
*           GLEW_STATIC is defined for static library.
*           GLEW_BUILD  is defined for building the DLL library.
*            
*           #ifdef GLEW_STATIC
*           #  define GLEWAPI extern
*           #else
*           #  ifdef GLEW_BUILD
*           #    define GLEWAPI extern __declspec(dllexport)
*           #  else
*           #    define GLEWAPI extern __declspec(dllimport)
*           #  endif
*           #endif
*       ```
*   
*   Hence GLEW_STATIC needs to be defined here in our code, since we are compiling glew's static library.
*   this was done in the Project -> Properties -> C/C++ -> Preprocessor -> 'Preprocessor Definitions'
* 
*   The documentation states that for to initalize GLEW with glewInit(), there must be a Working window context.
*   So the glewInit() code:
*       if (glewInit() != GLEW_OK) ...
*   Must be after the window context is made.
* 
*   For function 'glGenBuffers', use Ctrl+left-click to go to the MAcro definition.
*   Then Ctrl+left-click the argument of the Macro to see the Function's signature
*   Then    Ctrl+left-click on the type of the function pointer, which will show more.
*/

//  External
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//  STL
#include <iostream>

int main(void)
{
    /**
    *   From the GLFW library
    */
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //  Initialize GLEW; now one can use the GL Functions.
    //  All the functions are function pointers.
    if (glewInit() != GLEW_OK)
        std::cout << "Error!\n";

    //  Print out version.
    std::cout << glGetString(GL_VERSION) << "\n";



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        glClear(GL_COLOR_BUFFER_BIT);

        //  Simplest Rendering; Legacy Code
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
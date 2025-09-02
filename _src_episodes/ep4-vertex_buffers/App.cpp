/**
*   EP4: "Drawing with OpenGL"
*   
*   this shows the implementation of ModernGL.
* 
* 
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

    //  The vertices
    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f,
    };

    //  Vertex Buffer:
    unsigned int buffer;
    /**
    *  glGenBuffers: arg1: no. of buffers; arg2: pointer to unsigned int
    *  It takes a pointer to the buffer because it will write into the integer's memory.
    *  It returns void, and doesn't return any ID.
    *   
    *   Anything generated in OpenGL is given a unique identifier integer starting from 0
    *   This id is the above buffer itself, since the pointer number stores the id automatically..
    *   OpenGL stores it. For example, to use this buffer amongst several others, just use its ID
    */
    glGenBuffers(1, &buffer);
    //  Specifies what the buffer is used for
    //  binding the buffer shows it's to be used
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    /**
    *   Specify the data: this can be done dynamically, so it can be changed over execution
    *   meaning, for the data, one can add null showing that right now, no data is to be passed
    *   glBufferData(buffer_use_type/target, size of data, pointer to data, use)
    * 
    *   use arg:
    *       GL_STATIC_DRAW: Meaning the data store is defined/modified once and used or drawn many times (like in this case)
    *       GL_DYNAMIC_DRAW: The data store will be modified many times and used or drawn many times (which would have been the case
    *                   if we were changing the data in the while loop, every frame)
    *       GL_STREAM_DRAW: the data store contents are modified once and used a few times.
    * 
    */
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions, GL_STATIC_DRAW);

    //  Now define the order in which to read the coordinate data; layout of the data.
    //  this is needed since the shader needs to read the data in a specific way.
    //  glVertexAttribute shaders will be used for that.


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        glClear(GL_COLOR_BUFFER_BIT);

        //  ModernGL Rendering
        //  glDrawArrays is used when one does not have a buffer.
        //  The 0 is the buffer index to use, which is the recent one that was bound
        //  The count is the number of vertices to get from the data (x1, y1), (x2, y2), (x3, y3)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 3, unsigned int, null);  <- used with an index buffer.

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
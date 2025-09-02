/**
*   EP5: "How Shaders Work in OpenGL"
*   this shows the implementation of Vertex Attributes and Layouts
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

    glGenBuffers(1, &buffer);
    //  Specifies what the buffer is used for
    //  binding the buffer shows it's to be used
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions, GL_STATIC_DRAW);


    /**
    *   Now define the order in which to read the coordinate data; layout of the data.
    *   this is needed since the shader needs to read the data in a specific way.
    *   glVertexAttribute shaders will be used for that.
    *   
    *   arg1: ID/index of this buffer
    *   arg2: Size of the component count. Like there are 2 points for a vertex of the shape to drae
    *   arg3: type; GL_FLOAT
    *   arg4: Stride in terms of byte as that in the bytes between each vertex. Each vertex (of the point coordinate)
    *           is a float, which is 4 bytes, and a coordinate is 2 floats, hence the 2 * sizeof(float) to make the shader
    *           know the amountof stride when reading to get a position coordinate or vertex
    *   arg5: the pointer position/index to start reading from in the `pointer` variable. For example, if in the 'positions' variable there were texture coordinates...
    *       when making a `glVertexAttribPointer`, the pointer would be 8 normally.
    *       However it's meant to be a const void*, so it will be `(const void*)8`.
    */
    //  The data for the positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    //  this can be called anywhere; it enables this buffer to be used
    glEnableVertexAttribArray(0);

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        glClear(GL_COLOR_BUFFER_BIT);

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
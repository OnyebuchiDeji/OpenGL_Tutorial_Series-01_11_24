/**
*   EP13: "Abstracting OpenGL into Classes"
* 
*   There was an issue anytime the code was closed, it gave this error on glfwTerminate()
*   This was not a true error.
*   The issue is that the index buffer and vertex buffer objects are stack allocated, so they are destryed at the end of main()
*   Now, glfwTerminate() destroys the OpenGL context (as it should) and is called before the program terminates at
*   the end of the main() function.
* 
*   The issue is that GLGetError() raises an Exception when the OpenGL context no longer exists, since the Vertex Buffer and Index Buffer objects
*   that need the context are still alive.
* 
* 
*   So here, a workaround is done so this does not happen.
*   SOL:
*       1.  Heap allocate the VertecBuffer and IndexBuffer objects and delete them before calling glfwTerminate()
*           E.g. VertexBuffer* vbo_p = new VertexBuffer(...);
*               Then, delete vbo_p; before glfwTerminate
*       2.  Initialize the VertexBuffer and IndexBuffer in an inner scope such that the code that requires them are also in this scope
*           and at the end of this scope block, which is before glfwTerminate(), they are deleted.
*               This is the solution used here.
*/

//  External
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"



struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::fstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    //  While it does not return 0, that is, there are still lines to read
    //  It is inside the <string> header
    while (getline(stream, line))
    {
        //  If the line contains '#shader' and not an invalid string position in the file
        //  if it finds the string, it returns the position of it
        if (line.find("#shader") != std::string::npos)
        {
            //  Since it's found '#shader', then check if it's 'vertex'
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            //  if line has frament
            else if (line.find("fragment") != std::string::npos)
            {
                //  set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            //    If the line is any kind of code
            //  Using the arrays and the Enum type as an index to automate it
            ss[(int)type] << line << "\n";
        }

    }

    return { ss[0].str(), ss[1].str() };
}


/**
*   Writing a Shader Reader
*   
*   The `CreateShader` takes the source code of the Shader as a String as an argument
*   Compile the texts using OpenGL
*   
*   Note that it's better not to use the GL_Types since it requires to include the GLEW anywhere it's used.
*   
*/

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    //GLCall(unsigned int id = glCreateShader(type));
    GLCall(unsigned int id = glCreateShader(type));

    /**
    *   Because GL uses a const char array rather than STL string
    *   This is done and it returns a pointer to the start of an immutable (unmodifiable) char array
    *   Could do as below or do `const char* src = &source[0]`
    * 
    */
    const char* src = source.c_str();
    /**
    *   docs.gl/gl4/glShaderSource
    * 
    *   glShaderSource(id_of_shader, number_of_source_codes_specified,
    *       pointer_to_string_start <pointer to the pointer>, NULL)
    *   Passing NULL makes GL know that the string source will be NULL terminated.
    *   You can pass in NULL or nullptr, but prefer nullptr in Windows Platform
    */
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    //  TODO: Error Handling / Debugging
    int result;
    //  `glGetShader` is the actual function, while iv specify
    //  the types it tales
    //  i means it takes an integer, and v means it takes a vector, which in this
    //  case corresponds to the memory address of the integer
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    //  Can do `if (result)`, but below makes it clearer.
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to Compile " << (type==GL_VERTEX_SHADER ? "vertex" : "fragment") << "\n";
        std::cout << message << "\n";

        //  Then delete shader
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //  Note it returns an unsigned integer, unlike the glGenBuffer 
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    //  Attach the compiled source to the program
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    //  Since the Shaders' Obj outputs have already been linked into the program...
    //  this deletes those intermediate .obj files produced from compiling the shader source.
    //  This is better than something like glDetach since the latter deletes the source code from
    //  memory but this source code can be used in debugging, and the gain from deleting
    //  the source is not a lot.
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

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
    window = glfwCreateWindow(640, 480, "Yo! World!", NULL, NULL);
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


        /**
        *   The number of vertices are changed for the number for a square
        *   It can be made easier using index buffers.
        *   This are the number of vertices needed when using Index Buffers; there is no need
        *   to repeat vertices, hence the number of vertices reduce.
        *  as only the unique vertices are left
        *
        *   The index buffer is added to the Vertex Buffer
        **/
        float positions[] = {
            -0.5f, -0.5f,   //  0
            0.5f, -0.5f,    //  1
            0.5f, 0.5f,     //  2
            -0.5f, 0.5f,    //  3
        };

        //  Specified the indices for each triangle
        //  Note the indices of the triangle arfe drawin in an anticlockwise direction
        //  Then this data is sent ot the GPU for OpenGL to render it
        unsigned int indices[] = {
            0 ,1, 2,    //  First Triangle
            2, 3, 0,    //  Second Triangle
        };

        /**
        *   For the OpenGL Core profile, a Vertex Array Object needs to be bound
        *
        *   The below is all that is needed.
        *
        */
        unsigned int vao;
        //  The argument 1 specifies the number of arrays it holds
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        /**
        *   By specifying the VertexAttribPointer is what binds the index 0 of the explicitly created
        *   vertex array to the buffer specified in the VAO:
    *   *            GLCall(glBindVertexArray(vao));
        *   This links the VAO to the Buffer:
        *           GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
        *
        *   Now, if there were many other buffers, to be rendered with another VAO...
        *   if that new VAO was bound after the first, its index will be 1
        *   Hence the below:
        *           GLCall(glVertexAttribPointer(<arg1>, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
        *   the <arg1> will be given the value 1 so that the buffer being associated will be rendered using the
        *   second VAO
        *   Then thre last thing will be to bind the Vertex Array.
        */
        //  Vertex Buffer:
        //  On construction, it's already bound.
        //  But if there are multiple, one would have to rebinf the one's they want to use. But the Vertex Array takes care of this.
        VertexBuffer vbo(positions, 8 * sizeof(float));


        //  this can be called anywhere; it enables this buffer to be used
        GLCall(glEnableVertexAttribArray(0));
        //  The data for the positions
        //  @Exp: Notice how the layout of this Vertex Buffer is specified here
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

        //  The Index Buffer Object
        //  Note, only the count of the indices are passed in. It is already known that they are type
        //  unsigned int
        IndexBuffer ibo(indices, 6);

        //  Parse and Use Shader
        /**
        *   Below, a relative path is specified.
        **/
        ShaderProgramSource source = ParseShader("res/shaders/ep11-14/Basic.shader");


        unsigned int shader_program = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader_program));

        /**
        *   Setting up the Uniform.
        *
        *   1.  Retrieve the location of the uniform variable: Once the OpenGL program is created, it assigns an ID to every uniform so that
        *       uniform can be referenced.
        *       The way these Uniforms are looked up is by their names.
        *       So the shader is prompted to get the location of that Uniform variable, which gives an integer back as the response.
        *       But Modern versions of OpenGL allows one to set an index like it was done with:
        *           `
        *               layout(location=0) out vec4 color;
        *           `
        *       So from OpenGL 4.3 one is able to specify an explicit Uniform location.
        *       But this case retrieves it using th ename
        */
        GLCall(int location = glGetUniformLocation(shader_program, "u_Color"));
        //  If -1 it means uniform could not be found.
        //  But it is not required that the program should stop execution if the uniform is not there
        //  But in this case it's being ASSERTED to make sure everythin gis done correctly
        ASSERT(location != -1);
        //  Setting the value the uniform of that location.
        //GLCall(glUniform4f(location, 0.65f, 0.05f, 0.58f, 1.0f));



        /**
        *   @Exp
        *
        *   Unbind Everything so anything: The Program, the Vertex Buffer, and the Index Buffer
        *   So they can be bound only when it's time to draw.
        */
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        //  Animating the Color
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            /**
            *   @Exp
            *   Now, bind them all but no need to bind the buffer itself...
            *   just binding shader_program, vertex array and index buffer.
            *
            *   instead just the Vertex Array Object in its place:
            *       GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
            *   and no need for these:
            *           GLCall(glEnableVertexAttribArray(0));
            *           GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
            */
            //  bind shader
            GLCall(glUseProgram(shader_program));
            //  setup uniform
            GLCall(glUniform4f(location, r, 0.05f, 0.58f, 1.0f));
            //  Bind just the Vertex Array Object
            GLCall(glBindVertexArray(vao));
            //  bind index buffer
            ibo.Bind();

            /**
            *   Now, considering the below draw call, on that perhaps it's something to put as part of the Index Buffer class...
            *   If one has a space ship model, the Vertex Buffer will contain every single vertex of that spaceship.
            *   But there will most likely be several index buffers that draw parts of that ship because the wings will likely be
            *   a different material than the cockpit for example...
            *   The wings will probably be metal while the cockpit window glass.
            *   Hence one needs to split drawing these different parts into two different draw calls:
            *       1.  the draw call with the index buffer that just draws the wings
            *       2.  the draw call with the index buffer that just draws the cockpit.
            *   These will be covered when loading 3d models is covered.
            *
            *   So, there is one Vertex Buffer containing the model's vertices, and then several index buffers indexed into that Vertex Buffer.
            *   Hence one could tie the draw call to the index buffer sine theoretically the index buffer specifies which part of the model to draw.
            *
            *   But here, the Renderer will take care of the actual draw call.
            *   It will look at the index buffer, prompt for its count and execute the draw call.
            */

            GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr)); //  <- used with an index buffer.


            if (r > 1.0f)
                increment = -0.01f;
            else if (r < 0.0f)
                increment = 0.01f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        GLCall(glDeleteProgram(shader_program));
    }

    glfwTerminate();
    return 0;
}
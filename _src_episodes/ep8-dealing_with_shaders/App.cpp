/**
*   EP8: "How I Deal with Shaders In OpenGL"
*/

//  External
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

/**
*   Now this function, ParseShader is to read the file contents and separate them into the vertex and fragment
*   shader strings to compile.
* 
*   Now, normally the Cherno would use the C API for parsing files because it's faster. But here he uses
*   C++'s because it's not really a game engine
* 
*   It returns multiple types
*/

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
*   Link the two shader source into a shader program
*   Give a unique ID for that shader as returnes
*   ID is used to bind that shader to use it
* 
*   The `CompileShader` takes in the GL Shader Type and then the Source as a String
*   
*   Note that it's better not to use the GL_Types since it requires to include the GLEW anywhere it's used.
*   
*/

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);

    /**
    *   Because GL uses a const char array rather than STL string
    *   This is done and it returns a pointer to the start of an immutable (unmodifiable) char array
    *   Could do as below or do `const char* src = &source[0]`
    * 
    *   The src string is just a pointer to the source object passed in...
    *   meaning that source object must be alive at this point lest `src` points to some random memory
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
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //  TODO: Error Handling / Debugging
    int result;
    //  `glGetShader` is the actual function, while iv specify
    //  the types it tales
    //  i means it takes an integer, and v means it takes a vector, which in this
    //  case corresponds to the memory address of the integer
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    //  Can do `if (result)`, but below makes it clearer.
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        /**
        *   this char message[length] can't be done normally to allocate memory on the stack
        *   as C++ cannot grow the stack by an arbitrary value in some cases.
        *   because C++ finds it risky to memory using a variable whose value is not sure.
        *
        *   A solution could be this:
        *       char* message = new int[length];
        *   
        *   OR The Cherno's Hack:
        *       char* message = alloca(length * sizeof(char));
        *   alloca is a function provided by C to allow one to allocate memory on the stack dynamically.
        * 
        *   But then the compiler tells me that alloca indicates failure by raising a Stack Overflow Exception
        *   so I should use malloca, which does the same thing. But either way, yh...
        */
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
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
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //  Since the Shaders' Obj outputs have already been linked into the program...
    //  this deletes those intermediate .obj files produced from compiling the shader source.
    //  This is better than something like glDetach since the latter deletes the source code from
    //  memory but this source code can be used in debugging, and the gain from deleting
    //  the source is not a lot.
    glDeleteShader(vs);
    glDeleteShader(fs);

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


    //  this can be called anywhere; it enables this buffer to be used
    glEnableVertexAttribArray(0);
    //  The data for the positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  

    //  Parse and Use Shader
    /**
    *   Below, a relative path is specified.
    *   When a relative path is specidfied, it's from the working directory.
    *   If one runs teh executable outside of Visual Studio, the default directory will be the one that contains
    *   the executable.
    *   If one runs it trhough the VS debugger, the working directory is set by the VS Debugging Property.
    * 
    *   OpenGL -> Properties -> Configuration Properties -> Debugging -> 'Working Directory'
    *   'Working Directory => $(ProjectDir)'
    *   It is the directoryt that contains the Project VS File.
    *   So '$(ProjectDir)res/ep8/Basic.shader'
    **/
    ShaderProgramSource source = ParseShader("res/shaders/ep8-10/Basic.shader");
    //std::cout << "VERTEX:\n";
    //std::cout << source.VertexSource << "\n";
    //std::cout << "FRAGMENT:\n";
    //std::cout << source.FragmentSource << "\n";
  

    unsigned int shader_program = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader_program);
    

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

    glDeleteProgram(shader_program);


    glfwTerminate();
    return 0;
}
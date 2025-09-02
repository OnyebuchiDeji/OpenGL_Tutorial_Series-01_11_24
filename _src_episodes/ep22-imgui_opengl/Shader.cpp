#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

//"res/shaders/ep11-14/Basic.shader" ; GLCall(glUseProgram(shader_program));

Shader::Shader(const std::string& filePath)
    : m_FilePath(filePath)
{
    ShaderProgramSource source = ParseShader(filePath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}


Shader::~Shader()
{
    //  Only relevant if the m_RendererID is not 0 -- it was successful
    GLCall(glDeleteProgram(m_RendererID));
}


ShaderProgramSource Shader::ParseShader(const std::string& filePath)
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



unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
        std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "\n";
        std::cout << message << "\n";

        //  Then delete shader
        glDeleteShader(id);
        return 0;
    }

    return id;
}



unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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


void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v)
{
    GLCall(glUniform1i(GetUniformLocation(name), v));
}

void Shader::SetUniform1f(const std::string& name, float v)
{
    GLCall(glUniform1f(GetUniformLocation(name), v));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3)
{
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

    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, f2, f3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    //  'v' specifies that it's an array
    /**
    *   p1: Uniform name
    *   p2: no.of matrices being provided
    *   p3: should the matrix be transposed? True or False
    *   p4: pointer to the matrix itself
    */
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    /**
    *   glGetUniformLocation is retrieved again and again anytime this function is called.
    *   An optimization is to cache this value using an Unordered Map.
    */
    //  Checks if that uniform name is actually there in the cache already...
    //  specifically, the != `.end()` means it is actually there; that it is not null, the pointer to the end
    //  of the memory space.
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

    //  It's ok to have a Uniform Location of -1
    //  because you would want the Shader to still work even if you declare the
    //  Uniform but not use it.
    //  If -1 it means uniform could not be found.
    //  But it is not required that the program should stop execution if the uniform is not there
    //  Hence why in this case ASSERT is not being used
    if (location == -1)
        std::cout << "Warning: Uniform '" << name << "'doesn't exist!" << std::endl;
    
    m_UniformLocationCache[name] = location;

    return location;
}
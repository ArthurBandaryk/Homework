#include "gfx_program.hxx"
#include <fstream>
#include <iostream>

gfx_program::gfx_program()
{
    vertex_shader = fragment_shader = program_object = 0;
}

std::string gfx_program::get_source_code(
    const std::string& path)
{
    std::ifstream fin;
    fin.open(path.data(), std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "Error opening file at " << path
                  << std::endl;
        return "";
    }
    else
    {
        std::string result = "";
        while (!fin.eof())
        {
            std::string str = "";
            std::getline(fin, str);
            if (str == "" && fin.eof())
                break;
            result += str + '\n';
        }
        fin.close();
        return result;
    }
}

GLuint gfx_program::load_shader(GLenum            type,
                                const char* const instructions)
{
    GLuint shader;
    GLint  compiled;

    shader = glCreateShader(type);
    if (!shader)
    {
        std::cerr << "Error on creating empty shader object "
                  << type << std::endl;
        return 0;
    }

    // load source code to shader
    glShaderSource(shader, 1, &instructions, nullptr);
    my_gl_check();

    // Compile the shader
    glCompileShader(shader);
    my_gl_check();

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    my_gl_check();

    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        my_gl_check();
        if (infoLen > 1)
        {
            char str_err[infoLen];
            glGetShaderInfoLog(
                shader, infoLen, nullptr, str_err);
            my_gl_check();
            std::cerr << "Error compiling shader " << type
                      << "." << std::endl;
            std::cerr << str_err << std::endl;
        }
        glDeleteShader(shader);
        my_gl_check();
        return 0;
    }

    return shader;
}

GLuint gfx_program::get_id_program() const
{
    return program_object;
}

GLuint gfx_program::get_vertex_shader_id() const
{
    return vertex_shader;
}

GLuint gfx_program::get_fragment_shader_id() const
{
    return fragment_shader;
}

bool gfx_program::is_validated() const
{
    glValidateProgram(program_object);
    my_gl_check();

    GLint validate_status = 0;
    glGetProgramiv(
        program_object, GL_VALIDATE_STATUS, &validate_status);
    my_gl_check();

    if (!validate_status)
    {
        GLint infoLen = 0;
        glGetProgramiv(
            program_object, GL_INFO_LOG_LENGTH, &infoLen);
        my_gl_check();
        if (infoLen > 1)
        {
            char str_err[infoLen];
            glGetProgramInfoLog(
                program_object, infoLen, nullptr, str_err);
            my_gl_check();
            std::cerr << str_err << std::endl;
            return false;
        }
    }

    return true;
}

size_t gfx_program::prog_init(
    const std::string& vert_code_path,
    const std::string& frag_code_path)
{
    std::string vertex_code =
        this->get_source_code(vert_code_path);
    std::string frag_code =
        this->get_source_code(frag_code_path);

    vertex_shader =
        load_shader(GL_VERTEX_SHADER, vertex_code.data());
    if (!vertex_shader)
        return 1;
    fragment_shader =
        load_shader(GL_FRAGMENT_SHADER, frag_code.data());
    if (!fragment_shader)
        return 1;

    // Create the program object
    program_object = glCreateProgram();
    my_gl_check();
    if (!program_object)
    {
        std::cerr << "Error creating object programm!"
                  << std::endl;
        return 1;
    }

    glAttachShader(program_object, vertex_shader);
    my_gl_check();
    glAttachShader(program_object, fragment_shader);
    my_gl_check();

    // Link the program
    glLinkProgram(program_object);
    my_gl_check();

    GLint linked;

    glGetProgramiv(program_object, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(
            program_object, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char str_err[infoLen];
            glGetProgramInfoLog(
                program_object, infoLen, nullptr, str_err);
            my_gl_check();
            std::cerr << "Error linking programm!"
                      << std::endl;
            std::cerr << str_err << std::endl;
        }
        glDeleteProgram(program_object);
        my_gl_check();
        return 1;
    }

    return 0;
}

#pragma once

#include "gl_check.hxx"
#include "glad/glad.h"
#include <string>

class gfx_program
{
private:
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program_object;

    std::string get_source_code(const std::string& path);
    GLuint      load_shader(GLenum            type,
                            const char* const instructions);

public:
    gfx_program();
    size_t prog_init(const std::string& vert_code_path,
                     const std::string& frag_code_path);

    GLuint get_id_program() const;
    GLuint get_vertex_shader_id() const;
    GLuint get_fragment_shader_id() const;
    bool   is_validated() const;
};

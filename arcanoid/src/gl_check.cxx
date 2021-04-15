#include "gl_check.hxx"

#include "glad/glad.h"
#include <iostream>

// clang-format off
void my_gl_check()
{
    auto error = glGetError();

    if (error != GL_NO_ERROR)
    {
        switch (error)
        {
            case GL_INVALID_ENUM:
                std::cerr << "incorrect Glenum" << std::endl;
                break;
            case GL_INVALID_VALUE:
                std::cerr << "GL_INVALID_VALUE error:"
                             " a numeric argument is "
                             "out of range "
                          << std::endl;
            case GL_INVALID_OPERATION:
                std::cerr
                    << "GL_INVALID_OPERATION: "
                       "the specified operation is not "
                       "allowed in the current state"
                    << std::endl;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr
                    << "GL_INVALID_FRAMEBUFFER_OPERATION:"
                       " the framebuffer "
                       "object is not complete"
                    << std::endl;
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr
                    << "GL_OUT_OF_MEMORY: "
                       "there is not enough memory left to "
                       "execute the command."
                    << std::endl;
                break;
        }
    }
}
// clang-format on

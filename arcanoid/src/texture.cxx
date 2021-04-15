#include <fstream>
#include <iostream>
#include <vector>

#include "gl_check.hxx"
#include "picopng.hxx"
#include "texture.hxx"

#define STB_IMAGE_IMPLEMENTATION
#define DEBUG
#include "stb_image.h"

Texture::~Texture()
{
    GLuint texture_delete[1]{ texture_id };
    glDeleteTextures(1, texture_delete);
    my_gl_check();
}

Texture::Texture(Texture&& texture)
{
    texture_id         = texture.texture_id;
    width              = texture.width;
    height             = texture.height;
    channels_in_file   = texture.channels_in_file;
    path               = texture.path;
    texture.texture_id = 0;
}

Texture& Texture::operator=(const Texture& texture)
{
    texture_id       = texture.texture_id;
    width            = texture.width;
    height           = texture.height;
    channels_in_file = texture.channels_in_file;
    path             = texture.path;
    return *this;
}

std::string Texture::get_path() const
{
    return path;
}

Texture::Texture(const std::string& path)
{
    this->path = path;
    load_texture(
        path, width, height, channels_in_file, texture_id);
}

GLuint Texture::get_texture_id() const
{
    return texture_id;
}

GLuint Texture::get_texture_width() const
{
    return width;
}

GLuint Texture::get_texture_height() const
{
    return height;
}

void load_texture(const std::string& path,
                  int&               width,
                  int&               height,
                  int&               channels_in_file,
                  GLuint&            texture_id)
{
    stbi_set_flip_vertically_on_load(1);

#ifdef DEBUG
    std::cout << path.c_str() << std::endl;
#endif
    unsigned char* data = stbi_load(
        path.c_str(), &width, &height, &channels_in_file, 0);
    if (data == nullptr)
    {
        std::cerr << "Error loading texture for platform!"
                  << std::endl;
    }

#ifdef DEBUG
    std::cout << "width = " << width << " height = " << height
              << std::endl;
#endif

    glGenTextures(1, &texture_id);
    my_gl_check();

    glBindTexture(GL_TEXTURE_2D, texture_id);
    my_gl_check();

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 static_cast<GLsizei>(width),
                 static_cast<GLsizei>(height),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data);
    my_gl_check();

    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    my_gl_check();
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    my_gl_check();
    glBindTexture(GL_TEXTURE_2D, 0);
    my_gl_check();
    stbi_image_free(data);
}

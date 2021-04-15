#pragma once
#include "glad/glad.h"
#include "my_math.hxx"
#include <string>

class Texture
{
private:
    GLuint      texture_id = 0;
    int         width;
    int         height;
    int         channels_in_file;
    std::string path;

public:
    ~Texture();
    Texture(Texture&& texture);
    Texture(const std::string& path);
    GLuint      get_texture_id() const;
    GLuint      get_texture_width() const;
    GLuint      get_texture_height() const;
    std::string get_path() const;
    Texture&    operator=(const Texture& texture);
};

void load_texture(const std::string& path,
                  int&               width,
                  int&               height,
                  int&               channels_in_file,
                  GLuint&            texture_id);

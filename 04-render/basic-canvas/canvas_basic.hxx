#pragma once

#include <array>
#include <iostream>
#include <stdint.h>
#include <string>

namespace canv
{

constexpr unsigned int width  = 800; // width of bitmap
constexpr unsigned int height = 600; // height of bitmap

constexpr unsigned int buffer_size = width * height; // number of bitmap's
                                                     // pixels

#pragma pack(push, 1)
struct Color // specific color for one pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    bool                 operator==(const Color& c);
    bool                 operator!=(const Color& c);
    friend std::ostream& operator<<(std::ostream& os, const Color& c);
};
#pragma pack(pop)

class Image // image(array of pixels)
{
private:
    std::array<Color, buffer_size> pixels; // array of image's pixels

public:
    Image();
    bool  save_image(const std::string& file); // return true if success
    bool  load_image(const std::string& file); // return true if success
    auto& get_pixels();                        // get array of pixels
    void  set_pixel(unsigned int x,
                    unsigned int y,
                    Color&       c); // set color in a specific pos
    Color get_pixel(unsigned int x,
                    unsigned int y) const; // get color of a specific pixel
    std::array<Color, buffer_size>::iterator begin(); // begin of array's pixels
    std::array<Color, buffer_size>::iterator end();   // end of array's pixels
    bool                                     operator==(const Image& img);
    bool                                     operator!=(const Image& img);
};

} // namespace canv

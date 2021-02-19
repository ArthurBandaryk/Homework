#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

namespace canv {

#pragma pack(push, 1)
struct Color // specific color for one pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  Color();
  Color(uint8_t r, uint8_t g, uint8_t b);
  bool operator==(const Color &c);
  bool operator!=(const Color &c);
  friend std::ostream &operator<<(std::ostream &os, const Color &c);
};
#pragma pack(pop)

class Image // image(array of pixels)
{
private:
  std::vector<Color> pixels; // array of image's pixels
  size_t width;
  size_t height;

public:
  Image();
  Image(const size_t width, const size_t height);
  bool save_image(const std::string &file); // return true if success
  bool load_image(const std::string &file); // return true if success
  std::vector<Color> &get_pixels();         // get array of pixels
  void set_pixel(unsigned int x, unsigned int y,
                 Color &c); // set color in a specific pos
  Color get_pixel(unsigned int x,
                  unsigned int y) const; // get color of a specific pixel
  std::vector<Color>::iterator begin();  // begin of array's pixels
  std::vector<Color>::iterator end();    // end of array's pixels
  void set_buffer(const size_t width, const size_t height);
  size_t get_width() const;
  size_t get_height() const;
  size_t get_buffer_size() const;
  bool operator==(const Image &img);
  bool operator!=(const Image &img);
};

struct Position {
  Position(int32_t x, int32_t y);
  double length();
  friend Position operator-(const Position &left, const Position &right);
  friend bool operator==(const Position &left, const Position &right);
  int32_t x = 0;
  int32_t y = 0;
};

using pixels = std::vector<Position>;

class IRender {
public:
  virtual void clear(Color &col) = 0;
  virtual void set_pixel(Position &pos, Color &col) = 0;
  virtual pixels pixels_positions(Position &start, Position &end) = 0;
  virtual ~IRender();
};

} // namespace canv

#pragma once

#include "canvas_basic.hxx"

class LineRender : public canv::IRender {
private:
  canv::Image &img_line;

public:
  LineRender(canv::Image &img);
  ~LineRender();
  void clear(canv::Color &col) override;
  void set_pixel(canv::Position &pos, canv::Color &col) override;
  canv::pixels pixels_positions(canv::Position &start,
                                canv::Position &end) override;
  void draw_line(canv::Position &start, canv::Position &end, canv::Color &col);
  size_t get_width_image() const;
  size_t get_height_image() const;
};

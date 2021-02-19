#include "line_render.hxx"
#include <algorithm>
#include <functional>

LineRender::LineRender(canv::Image& img)
    : img_line(img)
{
}

LineRender::~LineRender() {}

void LineRender::clear(canv::Color& col)
{
    std::fill(img_line.begin(), img_line.end(), col);
}

void LineRender::set_pixel(canv::Position& pos, canv::Color& col)
{
    img_line.set_pixel(pos.x, pos.y, col);
}

canv::pixels LineRender::pixels_positions(canv::Position& start,
                                          canv::Position& end) // bresenham algo
{
    canv::pixels result;
    int          x0, x1, y0, y1;

    x0 = start.x;
    x1 = end.x;
    y0 = start.y;
    y1 = end.y;

    std::function<void(int, int, int, int)> f_line_low;
    std::function<void(int, int, int, int)> f_line_high;

    f_line_low = [&result](int x0, int y0, int x1, int y1) -> void {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int yi = 1; //
        if (dy < 0)
        {
            yi = -1;
            dy = -dy;
        }
        int D = 2 * dy - dx;
        int y = y0;

        for (int x = x0; x <= x1; ++x)
        {
            result.push_back(canv::Position{ x, y });
            if (D > 0)
            {
                y += yi;
                D += (2 * (dy - dx));
            }
            else
                D += 2 * dy;
        }
    };

    f_line_high = [&result](int x0, int y0, int x1, int y1) -> void {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int xi = 1;
        if (dx < 0)
        {
            xi = -1;
            dx = -dx;
        }
        int D = 2 * dx - dy;
        int x = x0;

        for (int y = y0; y <= y1; ++y)
        {
            result.push_back(canv::Position{ x, y });
            if (D > 0)
            {
                x += xi;
                D += (2 * (dx - dy));
            }
            else
                D += 2 * dx;
        }
    };

    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            f_line_low(x1, y1, x0, y0);
        }
        else
        {
            f_line_low(x0, y0, x1, y1);
        }
    }
    else
    {
        if (y0 > y1)
        {
            f_line_high(x1, y1, x0, y0);
        }
        else
        {
            f_line_high(x0, y0, x1, y1);
        }
    }

    return result;
}

void LineRender::draw_line(canv::Position& start,
                           canv::Position& end,
                           canv::Color&    col)
{
    canv::pixels v_l = pixels_positions(start, end);
    std::for_each(
        v_l.begin(), v_l.end(), [&](auto& el) { set_pixel(el, col); });
}

size_t LineRender::get_width_image() const
{
    return img_line.get_width();
}

size_t LineRender::get_height_image() const
{
    return img_line.get_height();
}

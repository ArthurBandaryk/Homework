#pragma once

#include "triangle_indexed_render.hxx"

struct Vertex
{
    double   x       = 0.; // position x
    double   y       = 0.; // position y
    u_int8_t r       = 0;  // r
    u_int8_t g       = 0;  // g
    u_int8_t b       = 0;  // b
    u_int8_t reserve = 0;  //?
};

struct Uniform
{
    double   x       = 0.; // position x
    double   y       = 0.; // position y
    u_int8_t r       = 0;  // r
    u_int8_t g       = 0;  // g
    u_int8_t b       = 0;  // b
    u_int8_t reserve = 0;  //?
};

class gfx_program
{
public:
    virtual ~gfx_program();
    virtual void        set_uniform(Uniform& uni)           = 0;
    virtual Vertex      vertex_shader(const Vertex& v_in)   = 0;
    virtual canv::Color fragment_shader(const Vertex& v_in) = 0;
};

class TriangleInterpolatedRender : public TriangleIndexedRender
{
private:
    gfx_program*        prog = nullptr;
    std::vector<Vertex> rasterize_triangle(const Vertex& v0,
                                           const Vertex& v1,
                                           const Vertex& v2);
    std::vector<Vertex> raster_horizontal_triangle(const Vertex& top,
                                                   const Vertex& left_line,
                                                   const Vertex& right_line);
    void                raster_one_vertical_line(const Vertex&        up,
                                                 const Vertex&        down,
                                                 std::vector<Vertex>& v_out);
    void                raster_one_horizontal_line(const Vertex&        left_vertex,
                                                   const Vertex&        right_vertex,
                                                   std::vector<Vertex>& v_out);

public:
    TriangleInterpolatedRender(canv::Image& img);
    void set_gfx_program(gfx_program& prog);
    void draw_triangles_interpolated(std::vector<Vertex>& v_vertexes,
                                     std::vector<short>&  v_indexes);
    void draw_raster_line(const Vertex& left, const Vertex& right);
};

Vertex interpolate(const Vertex& v0, const Vertex& v1, double t);

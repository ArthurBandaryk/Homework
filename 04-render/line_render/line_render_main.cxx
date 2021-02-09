#include "line_render.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    canv::Color black{ 0, 0, 0 };
    canv::Color white{ 255, 255, 255 };
    canv::Image img;

    LineRender render(img);

    render.clear(black);

    srand(time(NULL));
    for (int i = 0; i < 50; ++i)
    {
        canv::Position start{ rand() % static_cast<int>(canv::width),
                              rand() % static_cast<int>(canv::height) };
        canv::Position end{ rand() % static_cast<int>(canv::width),
                            rand() % static_cast<int>(canv::height) };

        canv::Color color{ static_cast<uint8_t>(rand() % 256),
                           static_cast<uint8_t>(rand() % 256),
                           static_cast<uint8_t>(rand() % 256) };
        render.draw_line(start, end, color);
    }

    std::string file = "line.ppm";
    img.save_image(file);
    return EXIT_SUCCESS;
}

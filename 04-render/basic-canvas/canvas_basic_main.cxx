#include "canvas_basic.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    canv::Color col = { 223, 134, 34 };
    canv::Image img;
    std::fill(img.begin(), img.end(), col);
    std::string file = "color_img.ppm";
    if (!img.save_image(file))
        return EXIT_FAILURE;

    canv::Image img_load;
    if (!img_load.load_image(file))
        return EXIT_FAILURE;

    if (img != img_load)
    {
        std::cerr << "img != img_load" << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "img = img_load" << std::endl;
    }

    return EXIT_SUCCESS;
}

#include "canvas_basic.hxx"
#include <fstream>

namespace canv
{

// Color struct implementation
//----------------------------------------------------
//----------------------------------------------------

Color::Color()
{
    r = g = b = 0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

bool Color::operator==(const Color& c)
{
    return this->r == c.r && this->g == c.g && this->b == c.b;
}

bool Color::operator!=(const Color& c)
{
    return !(this->operator==(c));
}

std::ostream& operator<<(std::ostream& os, const Color& c)
{
    os << "r = " << static_cast<uint8_t>(c.r) << " " << c.g << " " << c.b
       << std::endl;
    return os;
}
//----------------------------------------------------
//----------------------------------------------------

// Image class implementation
//----------------------------------------------------
//----------------------------------------------------

Image::Image() {}

bool Image::save_image(const std::string& file)
{
    std::ofstream fout;
    fout.exceptions(std::ios::failbit);
    fout.open(file, std::ios::binary);
    if (!fout.is_open())
    {
        std::cerr << "Error opening " << file << std::endl;
        return false;
    }
    else
    {
        fout << "P6\n" << width << ' ' << height << "\n" << 255 << "\n";
        std::streamsize size_buf =
            static_cast<std::streamsize>(sizeof(Color) * pixels.size());
        fout.write(reinterpret_cast<char*>(pixels.data()), size_buf);
    }
    fout.close();
    return true;
}

bool Image::load_image(const std::string& file)
{
    std::ifstream fin;
    fin.exceptions(std::ios::failbit);
    fin.open(file, std::ios::binary);
    if (!fin.is_open())
    {
        std::cerr << "Error opening " << file << std::endl;
        return false;
    }
    else
    {
        unsigned int width_load, height_load;
        std::string  header, max_value_color;
        fin >> header >> width_load >> height_load >> max_value_color;
        fin.get();
        if (header != "P6")
        {
            std::cerr << "Header isn't P6 format!" << std::endl;
            return false;
        }
        if (width_load * height_load != buffer_size)
        {
            std::cerr << "Buffer size of loaded image is not " << buffer_size
                      << std::endl;
            return false;
        }
        if (max_value_color != "255")
        {
            std::cerr << "Max value color isn't 255" << std::endl;
            return false;
        }
        std::streamsize buf_size =
            static_cast<std::streamsize>(sizeof(Color) * buffer_size);
        fin.read(reinterpret_cast<char*>(pixels.data()), buf_size);
    }

    fin.close();
    return true;
}

std::array<Color, buffer_size>& Image::get_pixels()
{
    return pixels;
}

void Image::set_pixel(unsigned int x, unsigned int y, Color& c)
{
    unsigned int index = y * width + x;
    pixels.at(index)   = c;
}

Color Image::get_pixel(unsigned int x, unsigned int y) const
{
    unsigned int index = y * width + x;
    return pixels.at(index);
}

std::array<Color, buffer_size>::iterator Image::begin()
{
    return pixels.begin();
}

std::array<Color, buffer_size>::iterator Image::end()
{
    return pixels.end();
}

bool Image::operator==(const Image& img)
{
    if (this->pixels.size() != img.pixels.size())
        return false;
    return std::equal(
        pixels.begin(), pixels.end(), img.pixels.begin(), img.pixels.end());
}

bool Image::operator!=(const Image& img)
{
    return !(this->operator==(img));
}
//----------------------------------------------------
//----------------------------------------------------

// Position class implementation
//----------------------------------------------------
//----------------------------------------------------
Position::Position(int32_t x, int32_t y)
{
    this->x = x;
    this->y = y;
}

double Position::length()
{
    return sqrt(x * x + y * y);
}

Position operator-(const Position& left, const Position& right)
{
    return { left.x - right.x, left.y - right.y };
}

bool operator==(const Position& left, const Position& right)
{
    return left.x == right.x && left.y == right.y;
}

//----------------------------------------------------
//----------------------------------------------------
IRender::~IRender() {}

} // namespace canv

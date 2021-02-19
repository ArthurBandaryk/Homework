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
    os << "r = " << static_cast<size_t>(c.r) << " " << static_cast<size_t>(c.g)
       << " " << static_cast<size_t>(c.b) << std::endl;
    return os;
}
//----------------------------------------------------
//----------------------------------------------------

// Image class implementation
//----------------------------------------------------
//----------------------------------------------------

Image::Image()
{
    width = height = 0;
}

Image::Image(const size_t width, const size_t height)
{
    this->width  = width;
    this->height = height;
    pixels.resize(width * height);
}

size_t Image::get_width() const
{
    return width;
}

size_t Image::get_height() const
{
    return height;
}

size_t Image::get_buffer_size() const
{
    return width * height;
}

void Image::set_buffer(const size_t width, const size_t height)
{
    this->width  = width;
    this->height = height;
}

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
        // fin >> header >> width_load >> height_load >> max_value_color;
        fin >> header;
        fin.get();
        char ch;
        fin.read(&ch, 1);
        while (ch == '#')
        {
            fin.ignore(255, '\n');
            fin.read(&ch, 1);
        }
        fin.seekg(-1, std::ios::cur);
        fin >> width_load >> height_load >> max_value_color;
        fin.get();
        if (header != "P6")
        {
            std::cerr << "Header isn't P6 format!" << std::endl;
            return false;
        }
        if (max_value_color != "255")
        {
            std::cerr << "Max value color isn't 255" << std::endl;
            return false;
        }
        pixels.resize(width_load * height_load);
        std::streamsize buf_size = static_cast<std::streamsize>(
            sizeof(Color) * width_load * height_load);
        fin.read(reinterpret_cast<char*>(pixels.data()), buf_size);
        set_buffer(width_load, height_load);
    }

    fin.close();
    return true;
}

std::vector<Color>& Image::get_pixels()
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

std::vector<Color>::iterator Image::begin()
{
    return pixels.begin();
}

std::vector<Color>::iterator Image::end()
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

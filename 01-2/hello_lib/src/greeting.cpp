#include <iostream>
#include <greeting.hpp>

bool greeting(std::string_view uname, std::string_view upath)
{
    std::cout << "Hello, " << uname << std::endl;
    std::cout << "PATH = " << upath << std::endl;

    return std::cout.good();
}

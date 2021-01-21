#include <cstdlib>
#include <greeting.hpp>

int main(int /*argc*/, char */*argv*/[])
{
    const char *user_var = std::getenv("USER");
    const char *user_path= std::getenv("PATH");

    std::string_view user = (user_var != nullptr) ? user_var : "USER - environment variable not found";
    std::string_view pth = (user_path != nullptr) ? user_path : "PATH - environment variable not found";

    bool is_good = greeting(user, pth);
    int result = is_good ? EXIT_SUCCESS : EXIT_FAILURE;
    return result;
}

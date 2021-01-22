#include <iostream>
#include <algorithm>
#include <functional
#include <string_view>
#include "Configure.h"

std::string_view GetUserName(char **env);

int main(int argc, char *argv[], char *env[])
{
    std::cout << "argc = " << argc << std::endl;
    std::cout << "Version " << argv[0]	<< ": " << Hello_VERSION_MAJOR <<
        "." << Hello_VERSION_MINOR << std::endl;

    std::string_view str = GetUserName(env);

    std::cout << "Hello, " << str << std::endl;
	return std::cout.good() ? 0 : 1;
}

char** GetEndEnv(char** env)
{
    while (*env)
    {
        //std::cout << *env << std::endl;
        ++env;
    }
    return env;
}


std::string_view GetUserName(char** env)
{
    char** p_end_env = GetEndEnv(env);
    auto res = std::find_if(env, p_end_env, std::bind([](std::string_view env_str,
                                            std::string_view usr_str)->bool{return (env_str.size() >= usr_str.size()
                                            && std::equal(usr_str.begin(), usr_str.end(), env_str.begin())) ; } ,
                                            std::placeholders::_1, "USER="));
    if(res == p_end_env) return "Unknown";
    else
    {
        std::string_view str = *res;
        auto end_it = std::end(str);
        auto it = std::find(std::begin(str), end_it, '=');
        if (it == end_it) return "";
        ++it;
        if (it == end_it) return "";
        return std::string_view{it, static_cast<size_t>(end_it - it)};
    }
}

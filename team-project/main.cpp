#include "stdafx.h"
#include <filesystem>
#include <iostream>
#include <windows.h>
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "cwd = "
        << std::filesystem::current_path().string()
        << std::endl;
    auto path = "C:/Users/Lee/Desktop/Project/zelda-project/team-project-bin/animations/Link_up.csv";
    std::ifstream f(path);
    std::cerr << "open(" << path << ") ¡æ " << std::boolalpha << f.is_open() << "\n";

    FRAMEWORK.Init(512, 478, "Zelda!!");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}
#include "stdafx.h"
#include <filesystem>
#include <iostream>
#include <windows.h>
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    
    FRAMEWORK.Init(512, 478, "Zelda!!");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}
#include "stdafx.h"
#include "FileDialog.h"
#ifdef _WIN32
#pragma comment(lib, "Comdlg32.lib")
#include <windows.h>
#include <commdlg.h>

std::string OpenAnimationFileDialog()
{
    char file[MAX_PATH] = "";
    OPENFILENAMEA ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = file;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = "Animation Files (*.json;*.csv)\0*.json;*.csv\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameA(&ofn)) return file;
    return {};
}
#else

std::string OpenAnimationFileDialog() { return {}; } // ´Ù¸¥ OS¿ë ½ºÅÓ
#endif
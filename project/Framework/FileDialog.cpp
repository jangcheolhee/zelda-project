#include "stdafx.h"
#include "FileDialog.h"
#ifdef _WIN32
#pragma comment(lib, "Comdlg32.lib")
#include <windows.h>
#include <commdlg.h>

std::string OpenAnimationFileDialog()
{
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "Animation Files (*.json;*.csv)\0*.json;*.csv\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "애니메이션 파일 열기";

    if (GetOpenFileNameA(&ofn))
    {
        return std::string(filename);
    }

    return "";

}
std::string OpenImageFileDialog()
{
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "이미지 파일 열기";

    if (GetOpenFileNameA(&ofn))
    {
        return std::string(filename);
    }

    return "";
}
#else

std::string OpenAnimationFileDialog() { return {}; } // 다른 OS용 스텁
std::string OpenImageFileDialog() { return {}; }
#endif
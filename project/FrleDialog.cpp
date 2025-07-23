#include "stdafx.h"
#include "FrleDialog.h"
#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>

std::string FrleDialog::OpenAnimationFileDialog()
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
std::string OpenAnimationFileDialog()
{
    return {}; // 다른 OS에선 빈 문자열 반환 or 다른 구현
}
#endif
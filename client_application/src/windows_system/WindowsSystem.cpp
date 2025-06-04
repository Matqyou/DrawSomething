//
// Created by Matq on 03/06/2025.
//

#include "WindowsSystem.h"
#include <windows.h>
#include <commdlg.h>
#include <string>

WindowsSystem::WindowsSystem()
{

}

WindowsSystem::~WindowsSystem()
{

}

std::string WindowsSystem::OpenPngFileDialog() {
	char filename[MAX_PATH] = "";

	OPENFILENAMEA ofn = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = "Select a PNG File";

	if (GetOpenFileNameA(&ofn)) {
		return std::string(filename);
	}

	return ""; // User cancelled
}

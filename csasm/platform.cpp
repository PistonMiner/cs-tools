#include "platform.h"

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static unsigned int sOldCodePageID;

void setupConsoleCodePage()
{
	sOldCodePageID = GetConsoleOutputCP();
	SetConsoleOutputCP(CP_UTF8);
}

void resetConsoleCodePage()
{
	SetConsoleOutputCP(sOldCodePageID);
}


std::string sjis2utf8(const std::string& sjis) {
	std::string utf8_string;

	LPCCH pSJIS = (LPCCH)sjis.c_str();
	int utf16size = ::MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, pSJIS, -1, 0, 0);
	if (utf16size != 0) {
		LPWSTR pUTF16 = new WCHAR[utf16size];
		if (::MultiByteToWideChar(CP_ACP, 0, (LPCCH)pSJIS, -1, pUTF16, utf16size) != 0) {
			
			int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, 0, 0, 0, 0);
			if (utf8size != 0) {
				LPTSTR pUTF8 = new TCHAR[utf8size + 16];
				ZeroMemory(pUTF8, utf8size + 16);
				if (::WideCharToMultiByte(CP_UTF8, 0, pUTF16, -1, pUTF8, utf8size, 0, 0) != 0) {
					utf8_string = std::string(pUTF8);
				}
				delete pUTF8;
			}
		}
		delete pUTF16;
	}
	return utf8_string;
}
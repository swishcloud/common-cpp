#include <string>
#include "common.h"
#include <iostream>
#include <memory>
#include <windows.h>
#include <dirent.h>
wchar_t* common::to_wcstr(const char* orig)
{
	size_t newsize = strlen(orig) + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	_locale_t locale = _create_locale(LC_ALL, "zh-CN");
	size_t convertedChars = _mbstowcs_l(wcstring, orig, newsize, locale);
	return wcstring;
}
char* common::to_cstr(const wchar_t* orig)
{
	size_t origsize = wcslen(orig) + 1;
	const size_t newsize = origsize * 2;
	char* cstring = new char[newsize];
	_locale_t locale = _create_locale(LC_ALL, "zh-CN");
	_wcstombs_l(cstring, orig, newsize, locale);
	return cstring;
}
std::string common::to_string(const wchar_t* orig)
{
	std::wstring ws(orig);
	std::string str(ws.begin(), ws.end());
	return str;
}
std::wstring common::full_path(std::wstring path)
{
	const int BUFSIZE = 4096;
	wchar_t buf[BUFSIZE]{};
	GetFullPathNameW(path.c_str(), BUFSIZE, buf, NULL);
	return std::wstring(buf);
}

int common::file_exist(const char* filename)
{
	WIN32_FILE_ATTRIBUTE_DATA info{};
	if (GetFileAttributesExA(filename, GET_FILEEX_INFO_LEVELS(), &info) == 0)
	{
		return 0;
	}
	if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		return 1;
	}
	return 2;
}
void common::run_file(const wchar_t* filename)
{
	ShellExecuteW(GetForegroundWindow(), L"open", filename, NULL, NULL, SW_SHOWNORMAL);
}
int common::find_files(std::wstring path, std::vector<std::wstring>& files)
{
	_WDIR* dir;
	struct _wdirent* ent;
	if ((dir = _wopendir(path.c_str())) != NULL)
	{
		int count = 0;
		while ((ent = _wreaddir(dir)) != NULL)
		{
			wchar_t* file_name_w = ent->d_name;
			files.push_back(std::wstring(file_name_w));
			++count;
		}
		free(ent);
		_wclosedir(dir);
		return count;
	}
	else
	{
		return -1;
	}
}
int common::find_files(std::string path, std::vector<std::string>& files)
{
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		int count = 0;
		while ((ent = readdir(dir)) != NULL)
		{
			char* file_name = ent->d_name;
			if (strcmp(file_name, ".")==0 || strcmp(file_name, "..") == 0)
			{
				continue;
			}
			files.push_back(std::string(file_name));
			++count;
		}
		free(ent);
		closedir(dir);
		return count;
	}
	else
	{
		return -1;
	}
}
char* common::strncpy(const char* source) {
	int size = strlen(source) + 1;
	char* const cpy = new char[size];
	::strncpy(cpy, source, size);
	return cpy;
}
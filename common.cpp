#include <string>
#include "common.h"
#include <iostream>
#include <memory>
#include <dirent.h>
class common_exception : public std::exception
{
public:
	std::string err;
	common_exception(std::string err) : err{err}
	{
	}
	const char *what() const noexcept override
	{
		return err.c_str();
	}
};
std::exception PLATFORM_NOT_SUPPORTED()
{
	return common_exception("this function not supported on the current platform.");
}
wchar_t *common::to_wcstr(const char *orig)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
	throw PLATFORM_NOT_SUPPORTED();
	size_t newsize = strlen(orig) + 1;
	wchar_t *wcstring = new wchar_t[newsize];
	/*_locale_t locale = ::_create_locale(LC_ALL, "zh-CN");
	size_t convertedChars = ::_mbstowcs_l(wcstring, orig, newsize, locale);*/
	return wcstring;
#endif
}
char *common::to_cstr(const wchar_t *orig)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
	size_t origsize = wcslen(orig) + 1;
	const size_t newsize = origsize * 2;
	char *cstring = new char[newsize];
	throw PLATFORM_NOT_SUPPORTED();
	/*_  locale_t locale = _create_locale(LC_ALL, "zh-CN");
	_wcstombs_l(cstring, orig, newsize, locale); */
	return cstring;
#endif
}
std::string common::to_string(const wchar_t *orig)
{
	std::wstring ws(orig);
	std::string str(ws.begin(), ws.end());
	return str;
}
std::wstring common::full_path(std::wstring path)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
	const int BUFSIZE = 4096;
	wchar_t buf[BUFSIZE]{};
	GetFullPathNameW(path.c_str(), BUFSIZE, buf, NULL);
	return std::wstring(buf);
#endif
}

int common::file_exist(const char *filename)
{
#ifdef __linux__
	if (!std::filesystem::exists(filename))
	{
		return 0;
	}
	else if (std::filesystem::is_directory(filename))
	{
		return 1;
	}
	else
	{
		return 2;
	}
#else
	WIN32_FILE_ATTRIBUTE_DATA info{};
	if (GetFileAttributesExA(filename, GET_FILEEX_INFO_LEVELS(), &info) == 0)
	{
		return 0;
	}
	if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		return 1;
	}
	return 2;
#endif
}
void common::run_file(const wchar_t *filename)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
	throw PLATFORM_NOT_SUPPORTED(); //ShellExecuteW(GetForegroundWindow(), L"open", filename, NULL, NULL, SW_SHOWNORMAL);
#endif
}
int common::find_files(std::wstring path, std::vector<std::wstring> &files)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
	_WDIR *dir;
	struct _wdirent *ent;
	if ((dir = _wopendir(path.c_str())) != NULL)
	{
		int count = 0;
		while ((ent = _wreaddir(dir)) != NULL)
		{
			wchar_t *file_name_w = ent->d_name;
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
#endif
}
int common::find_files(std::string path, std::vector<std::string> &files)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		int count = 0;
		while ((ent = readdir(dir)) != NULL)
		{
			char *file_name = ent->d_name;
			if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0)
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
char *common::strncpy(const char *source)
{
	if (!source)
	{
		return NULL;
	}
	int size = strlen(source) + 1;
	char *const cpy = new char[size];
	::strncpy(cpy, source, size);
	return cpy;
}

std::string common::GetLastErrorMsg(LPTSTR lpszFunction)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#endif
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
									  (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	auto str = common::string_format("%s failed with error %d: %s",
									 lpszFunction, dw, lpMsgBuf);
	auto r = str;
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	return r;
}
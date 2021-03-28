#include <string>
#include "common.h"
#include <iostream>
#include <memory>
#include <assert.h>
#include <mutex>
#include <regex>
#ifdef __linux__
#include <dirent.h>
#else
#include <dirent-win.h>
#endif

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
	size_t newsize = strlen(orig) + 1;
	wchar_t *wcstring = new wchar_t[newsize];
	_locale_t locale = ::_create_locale(LC_ALL, "zh-CN");
	size_t convertedChars = ::_mbstowcs_l(wcstring, orig, newsize, locale);
	return wcstring;
#endif
}
char *common::to_cstr(const wchar_t *orig, int size)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
	char *buf = NULL;
	if (size != -1)
	{
		buf = new char[size + 2];
		memcpy(buf, orig, size);
		buf[size] = '\0';
		buf[size + 1] = '\0';
		orig = (wchar_t *)buf;
	}

	size_t origsize = wcslen(orig) + 1;
	const size_t newsize = origsize * 2;
	char *cstring = new char[newsize];
	_locale_t locale = _create_locale(LC_ALL, "zh-CN");
	_wcstombs_l(cstring, orig, newsize, locale);
	delete (buf);
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
void common::find_files(std::string path, std::vector<std::string> &files, bool recursive, int filter)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			char *file_name = ent->d_name;
			if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0)
			{
				continue;
			}
			auto full_path = std::filesystem::path(path).append(file_name).string();
			if (ent->d_type == DT_DIR)
			{
				if (filter != 1)
				{
					files.push_back(full_path);
				}
				if (recursive)
					find_files(full_path, files, recursive, filter);
			}
			else
			{
				if (filter != 2)
				{
					files.push_back(full_path);
				}
			}
		}
		free(ent);
		closedir(dir);
	}
	else
	{
		throw exception(common::string_format("opendir failed:%s", path.c_str()));
	}
}
char *common::strcpy(const char *source, int len)
{
	if (!source)
	{
		return NULL;
	}
	if (len == -1)
	{
		len = strlen(source);
	}
	char *const cpy = new char[len + 1];
	::strncpy(cpy, source, len);
	cpy[len] = 0;
	return cpy;
}

std::string common::GetLastErrorMsg(const char *lpszFunction)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
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
#endif
}
void common::throw_exception(std::string s)
{
	throw common_exception(s);
}

std::mutex print_mtx;
void common::print_debug(std::string str)
{
	std::lock_guard<std::mutex> guard(print_mtx);
	auto level = "DEBUG";
	std::cout << common::string_format("%s>%s", level, str.c_str()) << std::endl;
}
void common::print_info(std::string str)
{
	std::lock_guard<std::mutex> guard(print_mtx);
	auto level = "INFO";
	std::cout << common::string_format("%s>%s", level, str.c_str()) << std::endl;
}
void common::pause()
{
	std::promise<void> promise;
	std::future<void> future = promise.get_future();
	future.get();
}
std::string common::uuid()
{
	return boost::uuids::to_string(boost::uuids::random_generator()());
}
size_t common::to_size_t(std::string str)
{
	std::stringstream sstream(str);
	size_t result;
	sstream >> result;
	return result;
}
std::string common::get_relative_path(std::string parent, std::string child)
{
	if (strncmp(parent.c_str(), child.c_str(), strlen(parent.c_str())) != 0)
	{
		throw common::exception("the two strings is not parent-child relationship.");
	}
	auto relative = child.c_str() + strlen(parent.c_str());
	if (relative[1] == '/' || relative[1] == '\\')
	{
		relative = relative + 1;
	}
	return relative;
}
std::string common::get_file_name(std::string path)
{
	std::regex re("[^/\\\\]+$");
	std::cmatch m;
	if (std::regex_search(path.c_str(), m, re))
		return m[0];
	else
		return "";
}
void common::makedir(std::string path)
{
	std::error_code ec;

	if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
	{
		std::filesystem::remove_all(path, ec);
		if (ec)
		{
			throw common::exception(ec.message());
		}
	}

	if (!std::filesystem::exists(path))
	{
		std::filesystem::create_directory(path, ec);
		if (ec)
		{
			throw common::exception(ec.message());
		}
	}
}
std::string common::url_encode(const char *str)
{
	int len = strlen(str);
	std::string buf{};
	for (int i = 0; i < len; i++)
	{
		unsigned char c = str[i];
		if (isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~')
		{
			buf.push_back(c);
		}
		else
		{
			buf.append(common::string_format("%%%x", c));
		}
	}
	return buf;
}
common::error::error()
{
}
common::error::error(const char *e) : error{e ? e : std::string{}}
{
}
common::error::error(std::string e)
{
	this->err = e;
}
common::error::operator bool()
{
	return !this->err.empty();
}
const char *common::error::message()
{
	return this->err.empty() ? "SUCCESS" : err.c_str();
}
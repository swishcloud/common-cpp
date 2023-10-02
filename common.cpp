#include <string>
#include "common.h"
#include <iostream>
#include <memory>
#include <assert.h>
#include <mutex>
#include <fstream>
#include <regex>
#include <sys/stat.h>
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

bool common::file_exist(const char *filename)
{
	struct stat buffer;
	int exist = stat(filename, &buffer);
	if (exist == 0)
		return 1;
	else
		return 0;
}
void common::run_file(const wchar_t *filename)
{
#ifdef __linux__
	throw PLATFORM_NOT_SUPPORTED();
#else
	throw PLATFORM_NOT_SUPPORTED(); // ShellExecuteW(GetForegroundWindow(), L"open", filename, NULL, NULL, SW_SHOWNORMAL);
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
#ifdef __aarch64__
	throw PLATFORM_NOT_SUPPORTED();
#endif
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
	if (relative[0] == '/' || relative[0] == '\\')
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
#ifdef __aarch64__
	throw PLATFORM_NOT_SUPPORTED();
#endif
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
		std::filesystem::create_directories(path, ec);
		if (ec)
		{
			throw common::exception(ec.message());
		}
	}
}
bool common::delete_file(std::string path)
{
	if (remove(path.c_str()) != 0)
		return 0;
	else
		return 1;
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
char *common::exec_cmd(const char *command, char **err)
{
	auto cmd = std::string(command) + " 2>&1";
	char *err_str = NULL;
	const int buffer_size = 128;
	char buffer[buffer_size];
	std::unique_ptr<char[]> result{new char[1]{}};
	// Open pipe to file
	FILE *pipe;
#ifdef __linux__
	pipe = popen(cmd.c_str(), "r");
#else
	pipe = _popen(cmd.c_str(), "r");
#endif
	if (!pipe)
	{
		err_str = common::strcpy("popen failed!");
		*err = err_str;
		return NULL;
	}
	while (fgets(buffer, buffer_size, pipe) != NULL)
	{
		size_t newsize = strlen(result.get()) + strlen(buffer) + 1;
		char *temp = new char[newsize];
		char *temp2 = temp + strlen(result.get());
		memcpy(temp, result.get(), strlen(result.get()));
		memcpy(temp2, buffer, strlen(buffer));
		temp[newsize - 1] = '\0';
		result.reset(temp);
	}
#ifdef __linux__
	pclose(pipe);
#else
	_pclose(pipe);
#endif
	delete (err_str);
	return result.release();
} // namespace filesync

std::string common::file_md5(const char *filename)
{
#ifdef __aarch64__
	throw PLATFORM_NOT_SUPPORTED();
#endif
	if (!std::filesystem::exists(filename))
	{
		throw exception("failed to calculate md5 due to the file does not exist.");
	}
	if (std::filesystem::file_size(filename) == 0)
		return "d41d8cd98f00b204e9800998ecf8427e";
	char *err{};
	std::string cmd;
	if (linux_os)
		cmd = common::string_format("md5sum '%s'", filename);
	else
		cmd = common::string_format("certutil -hashfile \"%s\" MD5", filename);
	char *cmd_resut = exec_cmd(cmd.c_str(), &err);
	std::cmatch m{};
	std::regex reg;
	if (linux_os)
	{
		reg = "^([a-z\\d]{32})";
	}
	else
	{
		reg = "\\n([a-z\\d]{32})";
	};
	if (!std::regex_search(cmd_resut, m, reg))
	{
		auto e_str = common::string_format("getting uuid failed.%s", cmd_resut);
		delete[] (err);
		delete[] (cmd_resut);
		throw exception(e_str);
	}
	std::string md5 = m[1].str();
	delete[] (err);
	delete[] (cmd_resut);
	return md5;
}

void common::movebycmd(std::string source, std::string destination)
{
	formalize_path(source);
	formalize_path(destination);
	char *err{};
	std::string cmd;
	if (linux_os)
		cmd = common::string_format("mv '%s' '%s' -f", source.c_str(), destination.c_str());
	else
		cmd = common::string_format("move /Y \"%s\" \"%s\"", source.c_str(), destination.c_str());
	std::string cmd_result = std::unique_ptr<char[]>{exec_cmd(cmd.c_str(), &err)}.get();
	std::unique_ptr<char[]> u{err};
	if (err != NULL)
	{
		throw common::exception(err);
	}
	if ("        1 file(s) moved.\n" != cmd_result && "" != cmd_result)
	{
		throw common::exception("ERROR:" + cmd_result);
	}
}

size_t common::file_size(std::string path)
{
	std::ifstream fs{path, std::ios_base::binary | std::ios_base::ate};
	return fs.tellg();
}
bool common::compare_md5(const char *a, const char *b)
{
	if (a == NULL)
	{
		a = "";
	}
	if (b == NULL)
	{
		b = "";
	}
	return strcmp(trim_trailing_space(a).c_str(), trim_trailing_space(b).c_str()) == 0;
}
std::string common::trim_trailing_space(std::string str)
{
	std::smatch m{};
	std::regex reg{"([^ ]*) *$"};
	assert(std::regex_search(str, m, reg));
	return m[1].str();
}
int common::random_num(int least, int greatest)
{
	srand(time(0));
	int port = rand() % (++greatest);
	while (port < least)
	{
		port = rand() % greatest;
	}
	return port;
}
std::string common::currentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	std::regex re(":|-|\\.");
	std::string res = std::regex_replace(buf, re, "");
	return res;
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
void common::formalize_path(std::string &path)
{
	char char_to_replace;
	char new_char;
	if (linux_os)
	{
		char_to_replace = '\\';
		new_char = '/';
	}
	else
	{
		char_to_replace = '/';
		new_char = '\\';
	}
	std::replace(path.begin(), path.end(), char_to_replace, new_char);
}
std::string common::trim_right(std::string str, std::string cutset)
{
	while (1)
	{
		int n = str.size();
		for (char c : cutset)
		{
			if (str[str.size() - 1] == c)
			{
				str = str.substr(0, str.size() - 1);
			}
		}
		if (n == str.size())
		{
			break;
		}
	}
	return str;
}
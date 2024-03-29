#ifndef COMMON_H
#define COMMON_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <cstring>
#include <filesystem>
#include <future>
#include <sstream>
#ifdef __linux__
#else
#include <locale.h>
#endif

#ifdef __linux__
#define linux_os true
#else
#define linux_os false
#endif

namespace common
{
	template <typename... Arg>
	std::string string_format(const std::string &format, Arg... args);
	wchar_t *to_wcstr(const char *orig);
	char *to_cstr(const wchar_t *orig, int size = -1);
	std::string to_string(const wchar_t *orig);
	std::wstring full_path(std::wstring path);
	// result: 0 does not exist;1 directory;2 file.
	bool file_exist(const char *filename);
	void run_file(const wchar_t *filename);
	int find_files(std::wstring path, std::vector<std::wstring> &files);
	/// filter: 0 query all,1 query files,2 query directories
	void find_files(std::string path, std::vector<std::string> &files, bool recursive = false, int filter = 0);
	char *strcpy(const char *source, int len = -1);
	std::string GetLastErrorMsg(const char *lpszFunction);
	void throw_exception(std::string s);
	class exception : public std::exception
	{
	public:
		std::string err;
		exception(std::string err) : err{err}
		{
		}
		const char *what() const noexcept override
		{
			return err.c_str();
		}
	};
	void print_debug(std::string str);
	void print_info(std::string str);
	void pause();
	std::string uuid();
	size_t to_size_t(std::string str);
	std::string get_relative_path(std::string parent, std::string child);
	std::string get_file_name(std::string path);
	void makedir(std::string path);
	bool delete_file(std::string path);
	std::string url_encode(const char *str);
	char *exec_cmd(const char *command, char **err);
	std::string file_md5(const char *filename);
	void movebycmd(std::string source, std::string destination);
	size_t file_size(std::string path);
	bool compare_md5(const char *a, const char *b);
	std::string trim_trailing_space(std::string str);
	int random_num(int least, int greatest);
	std::string currentDateTime();
	struct error
	{
	private:
		std::string err;

	public:
		error();
		error(const char *error);
		error(std::string error);
		operator bool();
		const char *message();
	};
	void formalize_path(std::string &path);
	std::string trim_right(std::string str, std::string cutset);
} // namespace common

template <typename... Arg>
std::string common::string_format(const std::string &format, Arg... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
	if (size <= 0)
	{
		throw std::runtime_error("Error during formatting.");
	}
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
#endif
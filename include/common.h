#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <cstring>
#include <filesystem>
namespace common
{
	template <typename... Arg>
	std::string string_format(const std::string &format, Arg... args);
	wchar_t *to_wcstr(const char *orig);
	char *to_cstr(const wchar_t *orig);
	std::string to_string(const wchar_t *orig);
	std::wstring full_path(std::wstring path);
	//result: 0 does not exist;1 directory;2 file.
	int file_exist(const char *filename);
	void run_file(const wchar_t *filename);
	int find_files(std::wstring path, std::vector<std::wstring> &files);
	int find_files(std::string path, std::vector<std::string> &files);
	char *strncpy(const char *source);
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
//
//  Useful utils for our project
//

#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
std::string string_format(const std::string& fmt_str, ...);
std::string join_path (const std::string& directory, const std::string& fn);
std::string get_directory_path(const std::string& s);
int write_buff_to_file(const std::string& filepath, const std::string& buff);
char* read_file_to_buff(const std::string& filepath, unsigned int& size_out);
std::string replace_str(std::string str, const std::string& from, const std::string& to);

#endif // UTILS_H

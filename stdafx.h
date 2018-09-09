#pragma once

#include <iostream>
using std::cout;
using std::wcout;
using std::cin;
using std::wcin;
using std::endl;
constexpr char newline = '\n';

#include <fstream>
using std::fstream;
using std::wfstream;
using std::ifstream;
using std::wifstream;
using std::ofstream;
using std::wofstream;

#include <string>
using std::string;
using std::wstring;

#include <clocale>

#include <regex>
using std::regex;
using std::wregex;

#include <filesystem>
namespace fs = std::filesystem;

#include <stdexcept>
using std::exception;

#include <utility>

#include <vector>
using std::vector;

#include <cwchar>

constexpr auto path_filename = "path.txt";

#include <limits>
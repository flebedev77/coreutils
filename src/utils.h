#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>

bool string_includes_char(std::string& str, char find);

bool copy_file(
    std::string from_path,
    std::string to_path,
    bool verbose,
    bool show_progress
  );

struct StorageAmount
{
  std::string suffix;
  float amount;
};

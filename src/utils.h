#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <vector>

struct StorageAmount
{
  std::string suffix;
  float amount;
};

struct DirItem
{
  std::string path;
  std::string filename;
  bool is_directory;
};

bool string_includes_char(std::string& str, char find);

bool copy_file(
    std::string from_path,
    std::string to_path,
    bool verbose,
    bool show_progress
  );

std::string get_filename_from_path(const std::string& path);

std::vector<DirItem> walk_dir(const std::string& path);



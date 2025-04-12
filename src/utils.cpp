#include "utils.h"

bool string_includes_char(std::string& str, char find)
{
  return str.find(find) != std::string::npos;
}


bool copy_file(
    std::string from_path,
    std::string to_path,
    bool verbose
  ) 
{
  using namespace std;

  ifstream source(from_path, ios::binary);
  ofstream destination(to_path, ios::binary);

  if (!source.good())
  {
    cout << "cp: cannot stat '" << from_path << "': No such file or directory\n";
    return 1;
  }

  if (verbose)
  {
    cout << "'" << from_path << "' -> '" << to_path << "'\n";
  }

  if (source.is_open() && destination.is_open()) {
    char buffer[4096];
    streamsize bytesRead;

    while ((bytesRead = source.read(buffer, sizeof(buffer)).gcount()) > 0) {
      destination.write(buffer, bytesRead);
    }

    if (!source.eof()) {
      cerr << "cp: cannot copy '" << from_path << "' -> '" << to_path << "' due to early EOF\n";
      return true;
    }

  } else {
    cerr << "cp: cannot copy '" << from_path << "' -> '" << to_path << "'\n";
  }

  return false;
}

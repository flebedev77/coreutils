#include <iostream>
#include <fstream>
#include <string>
#include "cp_data.h"


struct CopyOptions 
{
  bool progress_enabled;
};

int main(int argc, char** argv) 
{
  using namespace std;  

  CopyOptions options = {
    false
  };

  string from_path = "";
  string to_path = "";

  if (argc == 1)
  {
    cout << "cp: missing file operand\n";
    cout << "Try 'cp --help' for more information.\n";
    return 1;
  }

  if (argc > 1)
  {
    for (int i = 1; i < argc; i++) {
      string current_argument(argv[i]);

      if (current_argument == "--help")
      {
        cout << help_message;
        return 1;
      }
    }

    for (int i = 1; i < argc; i++) {
      string current_argument(argv[i]);

      if (
          current_argument == "-p" ||
          current_argument == "--progress"
        )
      {
        options.progress_enabled = true;
      }

      if (current_argument.at(0) != '-')
      {
        if (from_path != "" && to_path == "")
        {
          to_path = current_argument;
        }
        if (from_path == "")
        {
          from_path = current_argument;
        }
      }
    }

  }

  if (from_path == "")
  {
    cout << "cp: missing file operand\n";
    cout << "Try 'cp --help' for more information.\n";
    return 1;
  }

  if (from_path != "" && to_path == "")
  {
    cout << "cp: missing destination file operand after '" << from_path << "'\n";
    cout << "Try 'cp --help' for more information.\n";
    return 1;
  }

  ifstream source(from_path, ios::binary);
  ofstream destination(to_path, ios::binary);
  
  if (source.is_open() && destination.is_open()) {
    char buffer[4096];
    std::streamsize bytesRead;

    while ((bytesRead = source.read(buffer, sizeof(buffer)).gcount()) > 0) {
      destination.write(buffer, bytesRead);
    }

    if (!source.eof()) {
      std::cerr << "Error occurred while copying the file." << std::endl;
      return 1;
    }
    cout << "File copied successfully!" << endl;
  } else {
    cerr << "Error opening files." << endl;
  }

  return 0;
}

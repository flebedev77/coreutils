#include <iostream>
#include <fstream>
#include <string>
#include "cp_data.h"
#include "utils.h"


struct CopyOptions 
{
  bool progress_enabled;
  bool recursive;
  bool verbose;
};

int main(int argc, char** argv) 
{
  using namespace std;  

  CopyOptions options = {
    false, false, false
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

      if (current_argument.at(0) == '-')
      {
        if (
            string_includes_char(current_argument, 'p') ||
            current_argument == "--progress"
           )
        {
          options.progress_enabled = true;
        }

        if (
            string_includes_char(current_argument, 'r') ||
            string_includes_char(current_argument, 'R') ||
            current_argument == "--recursive"
           )
        {
          options.recursive = true;
        }


        if (
            string_includes_char(current_argument, 'v') ||
            current_argument == "--verbose"
           )
        {
          options.verbose = true;
        }

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

  if (!options.recursive)
  {
    copy_file(from_path, to_path, options.verbose);
  } else
  {
    // TODO: Loop through all the files and directories in the from_path and copy them to to_path
  }

  return 0;
}

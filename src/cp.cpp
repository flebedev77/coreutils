#include <iostream>
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



  // cout << argv[0] << " " << argc << endl;
  cout << from_path << " " << to_path << " " << options.progress_enabled << endl;
}

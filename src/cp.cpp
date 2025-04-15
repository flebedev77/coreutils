#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include "cp_data.h"
#include "utils.h"


struct CopyOptions 
{
  bool progress_enabled;
  bool recursive;
  bool verbose;
  bool debug;
};

int main(int argc, char** argv) 
{
  using namespace std;  

  CopyOptions options = {
    false, false, false, false
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

      if (current_argument == "--help" || current_argument == "-h")
      {
        cout << help_message;
        return 1;
      }

      if (current_argument == "--debug")
      {
        options.debug = true;
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

  if (get_filename_from_path(to_path) == "")
  {
    to_path += get_filename_from_path(from_path);
  }

  if (filesystem::is_directory(to_path) && to_path.back() != '/')
  {
    to_path += "/";
    to_path += get_filename_from_path(from_path);
  }


  if (!options.recursive)
  {

    if (get_filename_from_path(from_path) == "" || filesystem::is_directory(from_path))
    {
      cout << "cp: -r not specified; omitting directory '" << from_path << "'\n";
      return 1;
    }

    copy_file(from_path, to_path, options.verbose, options.progress_enabled);
  } else
  {
    // TODO: Loop through all the files and directories in the from_path and copy them to to_path
    
    if (to_path.back() != '/')
    {
      to_path += '/';
    }

    if (!filesystem::exists(to_path))
    {
      if (filesystem::create_directory(to_path))
      {
        if (options.verbose)
        {
          cout << "'" << to_path << "' didn't exist. Created\n";
        }
      }
    }
    
    vector<DirItem> paths_to = walk_dir(from_path);
    vector<DirItem> paths_from(paths_to);


    if (options.verbose)
    {
      cout << "cp: scanning directory '" << from_path << "'\n";
    }

    for (long unsigned int i = 0; i < paths_to.size(); i++)
    {
      DirItem& from_item = paths_from.at(i);

      DirItem& to_item = paths_to.at(i);
      to_item.path = to_path + to_item.path.substr(from_path.length(), to_item.path.length());
      
      fix_path(from_item.path);
      fix_path(to_item.path);


      if (options.debug)
      {
        if (to_item.filename != from_item.filename)
        {
          cout << "cp: internal error '" << to_item.filename << "' != '" << from_item.filename << "'\n";
          return 1;
        }
        cout << from_item.path << " -> " << to_item.path << " is_directory = " << ((to_item.is_directory) ? "yes" : "no") << endl;
      }

      if (from_item.is_directory && to_item.is_directory)
      {
        if (filesystem::create_directory(to_item.path))
        {
          if (options.verbose)
          {
            cout << from_item.path << " -> " << to_item.path << endl;
          }
        } else 
        {
          cout << "cp: error trying to replicate directory '" << to_item.path << "'\n";
        }

      } else
      {
        copy_file(from_item.path, to_item.path, options.verbose, options.progress_enabled);

        if (options.progress_enabled)
        {
          cout << fixed << setprecision(2) << (static_cast<float>(i) / static_cast<float>(paths_to.size())) * 100.f << "% (" << i << "/" << paths_to.size() << ")\n" << flush;
        }
      }

    }

  }

  return 0;
}

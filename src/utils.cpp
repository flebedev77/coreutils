#include "utils.h"

bool string_includes_char(std::string& str, char find)
{
  return str.find(find) != std::string::npos;
}

std::vector<DirItem> walk_dir(const std::string& path)
{
  using namespace std;
  namespace fs = std::filesystem;

  vector<DirItem> paths;

  for (const auto& entry : fs::directory_iterator(path)) {
    paths.push_back(DirItem {entry.path(), entry.path().filename(), fs::is_directory(entry)});
    if (fs::is_regular_file(entry)) {
    } else if (fs::is_directory(entry)) {
      vector<DirItem> sub_paths = walk_dir(entry.path().string());
      paths.insert(paths.end(), sub_paths.begin(), sub_paths.end());
    }
  }

  return paths;
}


std::string get_filename_from_path(const std::string& path) {
    std::string filename;
    std::size_t last_slash_pos = path.find_last_of("/\\");
    if (last_slash_pos != std::string::npos) {
        filename = path.substr(last_slash_pos + 1);
    } else {
        filename = path;
    }
    return filename;
}

void fix_path(std::string& path)
{
  for (size_t i = path.length()-1; i >= 1; i--)
  {
    if (path[i] == '/' && path[i-1] == '/')
    {
      path.erase(i-1);
    }
  }
  if (std::filesystem::is_directory(path) && path.back() != '/')
  {
    path += '/';
  }
}


StorageAmount convert_bytes(float value) 
{
  char suffixes[] = {' ', 'K', 'M', 'G', 'T', 'P'};
  char suffix = ' '; 
  int suffixIndex = 0;
  while (value / 1024.f >= 1.f)
  {
    value = value / 1024.f;
    suffixIndex++;
    suffix = suffixes[suffixIndex];
  }
  std::string formattedSuffix(1, suffix);
  formattedSuffix += 'b';
  return StorageAmount { formattedSuffix, value };
}

bool copy_file(
    std::string from_path,
    std::string to_path,
    bool verbose,
    bool show_progress
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

  ifstream file(from_path, ios::binary | ios::ate);
  streampos pointer_pos = file.tellg();
  size_t filesize = static_cast<size_t>(pointer_pos);
  StorageAmount filesize_formatted = convert_bytes(static_cast<float>(filesize));


  if (show_progress)
  {
    cout << "Copying... \r";
  }

  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
  chrono::steady_clock::time_point last_update = chrono::steady_clock::now();
  chrono::duration<float> elapsed_time;
  chrono::duration<float> update_speed = chrono::milliseconds(100);
  chrono::duration<float> time_since_last_update = chrono::seconds(5);

  if (source.is_open() && destination.is_open()) {
    char buffer[4096];
    streamsize bytes_read;

    size_t bytes_copied = 0;

    while ((bytes_read = source.read(buffer, sizeof(buffer)).gcount()) > 0) {
      destination.write(buffer, bytes_read);
      if (show_progress)
      {
        bytes_copied += bytes_read;

        chrono::steady_clock::time_point current_time = chrono::steady_clock::now();
        elapsed_time = current_time - start_time;
        time_since_last_update = current_time - last_update;

        if (time_since_last_update > update_speed)
        {
          last_update = chrono::steady_clock::now();

          float bytes_per_second = static_cast<float>(bytes_copied) / elapsed_time.count(); 

          StorageAmount copy_speed = convert_bytes(bytes_per_second);
          StorageAmount bytes_copied_formatted = convert_bytes(static_cast<float>(bytes_copied));

          cout << "                                                             \r" << flush;
          cout << fixed << setprecision(2) << bytes_copied_formatted.amount << bytes_copied_formatted.suffix << "/" << filesize_formatted.amount << filesize_formatted.suffix << " - " << (static_cast<float>(bytes_copied) / static_cast<float>(filesize)) * 100.f << "% - " << copy_speed.amount << copy_speed.suffix << "/s - " << elapsed_time.count() << "s \r" << flush;
        }
      }
    }

    if (!source.eof()) {
      cerr << "cp: cannot copy '" << from_path << "' -> '" << to_path << "' due to early EOF\n";
      return true;
    }

  } else {
    cerr << "cp: cannot copy '" << from_path << "' -> '" << to_path << "'\n";
  }

  cout << endl << flush;

  return false;
}

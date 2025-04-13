#include "utils.h"

bool string_includes_char(std::string& str, char find)
{
  return str.find(find) != std::string::npos;
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


  if (show_progress)
  {
    cout << "0.00% \r";
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

          cout << fixed << setprecision(2) << (static_cast<float>(bytes_copied) / static_cast<float>(filesize)) * 100.f << "% - " << copy_speed.amount << copy_speed.suffix << "/s - " << elapsed_time.count() << "s \r" << flush;
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

  return false;
}

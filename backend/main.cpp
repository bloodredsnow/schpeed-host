#include <chrono>
#include <ctime> 
#include <filesystem>
#include <sstream>
#include <fstream>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

void OnFileCreated(const std::string& filename) {
  
  std::string path_to_endpoint = "/var/www/erm.cool/files/endpoint/";
  std::string path_to_sites = "/var/www/erm.cool/files/endpoint/websites/";
   
  std::string file_hash_no_type = filename;
  std::string file_hash_type = filename;
  file_hash_no_type.resize(64);
  
  path_to_sites += file_hash_no_type;
  path_to_endpoint += filename;

  std::string file_destination = path_to_sites;
  std::string file_source = path_to_sites;

  file_source += "/";
  file_source += filename;
  file_destination += "/";
  file_destination += filename;
  
  std::cout << "creating site: " << path_to_sites << std::endl;
  
  std::filesystem::create_directory(path_to_sites);

  std::string index_filename = path_to_sites;
  index_filename += "/index.php";

  std::ofstream write_file(index_filename);

  std::string website_src_chunk1 = "<html> <h1 style=\"z-index: 9999; position: absolute; top:2%\"> File uploaded on:";
  std::string website_src_chunk2 = "</h1> <br> <body style=\"margin: 0; padding: 0; height: 100vh; display: flex; justify-content: center; align-items: center;\"> <img src=";
  std::string website_src_chunk3 = " alt=\"image\" style=\"max-width: calc(100% - 10%); height: auto; margin: 5%; box-sizing: border-box;\"> </body> </html>";

  auto now = std::chrono::system_clock::now();
  
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  
  website_src_chunk1 += std::ctime(&now_c);

  website_src_chunk1 += website_src_chunk2;

  website_src_chunk1 += filename;
  
  website_src_chunk1 += website_src_chunk3;
  
  write_file << website_src_chunk1;

  write_file.close();
  
  std::filesystem::rename(path_to_endpoint , file_destination);
  
}

void WatchDirectory(const std::string& directory) {
    int fd = inotify_init();
    if (fd < 0) {
        std::cerr << "inotify_init failed." << std::endl;
        return;
    }

    int wd = inotify_add_watch(fd, directory.c_str(), IN_CREATE);
    if (wd < 0) {
        std::cerr << "inotify_add_watch failed." << std::endl;
        close(fd);
        return;
    }

    char buffer[1024];
    while (true) {
        int length = read(fd, buffer, sizeof(buffer));
        if (length < 0) {
            std::cerr << "read failed." << std::endl;
            break;
        }

        for (int i = 0; i < length;) {
            struct inotify_event* event = reinterpret_cast<struct inotify_event*>(&buffer[i]);
            if (event->mask & IN_CREATE) {
                OnFileCreated(event->name);
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

int main() {
    std::string directory = "../endpoint/"; // Change this to your directory
    WatchDirectory(directory);
    return 0;
}

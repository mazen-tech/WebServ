#include "../header/read_conf.hpp"

bool find_file(const std::string& dir, const std::string& target, std::string& found_path)
{
    DIR* dp = opendir(dir.c_str());
    if (dp == nullptr) return false;

    struct dirent* entry;
    while ((entry = readdir(dp)) != nullptr) {
        std::string path = dir + "/" + entry->d_name;

        if (entry->d_type == DT_DIR) {
            // Pomijamy '.' i '..'
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (find_file(path, target, found_path)) {
                    closedir(dp);
                    return true;
                }
            }
        } else if (entry->d_type == DT_REG && target == entry->d_name) {
            found_path = path;
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

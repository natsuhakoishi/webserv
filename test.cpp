#include <dirent.h>
#include <iostream>

int main() {
    DIR* dir = opendir("./"); // 打開當前資料夾
    if (!dir) {
        std::cerr << "Failed to open directory\n";
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::cout << entry->d_name << std::endl;
    }

    closedir(dir); // 關閉目錄
    return 0;
}

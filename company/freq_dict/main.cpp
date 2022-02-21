#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>

int main(int argc, const char** argv)
{
    std::string path;
    if (argc > 1) {
        path = argv[1];
    }
    std::unordered_map<std::string, int> dictionary;
    std::ifstream file(path);
    std::string str;
    while (std::getline(file, str))
    {
        size_t offset = 0;
        size_t len = 0;
        std::string key;
        for (size_t i = 0; i < str.length(); i++) {
            if (!std::isalpha(str[i])) {
                if (i > offset) {
                    key = str.substr(offset, len);
                    dictionary[key] += 1;
                }
                offset = i + 1;
                len = 0;
            }
            len++;
        }
        if (str.length() - 1 > offset) {
            key = str.substr(offset, len);
            dictionary[key] += 1;
        }
    }
    for (const auto &it : dictionary) {
        printf("%s: %d\n", it.first.c_str(), it.second);
    }
    return 0;
}

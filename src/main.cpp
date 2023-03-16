#include "hw1.h"
#include "image.h"
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    std::vector<std::string> input_filenames;
    std::string hw_num;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-hw") {
            hw_num = std::string(argv[++i]);
        } else {
            input_filenames.push_back(std::string(argv[i]));
        }
    }

    if (hw_num == "1_1") {
        Image3 img = hw_1_1();
        imwrite("hw_1_1.exr", img);
    }

    return 0;
}

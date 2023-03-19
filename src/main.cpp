#include "hw1.h"
#include "image.h"
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    std::vector<std::string> parameters;
    std::string hw_num;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-hw") {
            hw_num = std::string(argv[++i]);
        } else {
            parameters.push_back(std::string(argv[i]));
        }
    }

    if (hw_num == "1_1") {
        Image3 img = hw_1_1(parameters);
        imwrite("hw_1_1.exr", img);
    } else if (hw_num == "1_2") {
        Image3 img = hw_1_2(parameters);
        imwrite("hw_1_2.exr", img);
    } else if (hw_num == "1_3") {
        Image3 img = hw_1_3(parameters);
        imwrite("hw_1_3.exr", img);
    } else if (hw_num == "1_4") {
        Image3 img = hw_1_4(parameters);
        imwrite("hw_1_4.exr", img);
    } else if (hw_num == "1_5") {
        Image3 img = hw_1_5(parameters);
        imwrite("hw_1_5.exr", img);
    } else if (hw_num == "1_6") {
        Image3 img = hw_1_6(parameters);
        imwrite("hw_1_6.exr", img);
    }

    return 0;
}

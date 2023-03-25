#include "hw1.h"
#include "hw2.h"
#include "hw3.h"
#include "image.h"
#include "parallel.h"
#include <vector>
#include <string>
#include <thread>

int main(int argc, char *argv[]) {
    std::vector<std::string> parameters;
    std::string hw_num;
    int num_threads = std::thread::hardware_concurrency();
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-t") {
            num_threads = std::stoi(std::string(argv[++i]));
        } else if (std::string(argv[i]) == "-hw") {
            hw_num = std::string(argv[++i]);
        } else {
            parameters.push_back(std::string(argv[i]));
        }
    }

    parallel_init(num_threads);

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
    } else if (hw_num == "1_7") {
        Image3 img = hw_1_7(parameters);
        imwrite("hw_1_7.exr", img);
    } else if (hw_num == "1_8") {
        Image3 img = hw_1_8(parameters);
        imwrite("hw_1_8.exr", img);
    } else if (hw_num == "2_1") {
        Image3 img = hw_2_1(parameters);
        imwrite("hw_2_1.exr", img);
    } else if (hw_num == "2_2") {
        Image3 img = hw_2_2(parameters);
        imwrite("hw_2_2.exr", img);
    } else if (hw_num == "2_3") {
        Image3 img = hw_2_3(parameters);
        imwrite("hw_2_3.exr", img);
    } else if (hw_num == "2_4") {
        Image3 img = hw_2_4(parameters);
        imwrite("hw_2_4.exr", img);
    } else if (hw_num == "2_5") {
        Image3 img = hw_2_5(parameters);
        imwrite("hw_2_5.exr", img);
    } else if (hw_num == "3_1") {
        Image3 img = hw_3_1(parameters);
        imwrite("hw_3_1.exr", img);
    }

    parallel_cleanup();

    return 0;
}

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <vector>
#include <cmath>
int main(int argc, char** argv) {
    std::cout << argv[0] << std::endl;
    std::cout << argv[1] << std::endl;
    std::string filename = argv[1];
    std::ifstream input_file(filename);
    std::queue<std::string> lines;
    std::string line;
    std::vector<std::string> ch;
    float arr[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1 } };



    while (std::getline(input_file, line)) {
        lines.push(line);
    }
    /*for (int i = 0; i < lines.size(); i++) {
        std::cout << lines[i] << std::endl;
    }*/
    while (!lines.empty()) {
        line = lines.front();
        //std::cout << line << std::endl;
        lines.pop();
        while (1) {
            //std::cout << "1111" << std::endl;
            if (line.size() == 0) {
                break;
            }
            while (line.find(" ") == 0) {
                line = line.substr(line.find(" ") + 1, line.length());
            }
            ch.push_back(line.substr(0, line.find(" ")));
            line = line.substr(line.find(" ") + 1, line.length());
            if (line.find(" ") == -1) {
                if (line == "reset" || line == "triangle" || line == "square" || line == "clearData" || line == "clearScreen" || line == "end") {
                    break;
                }
                ch.push_back(line);
                break;
            }
        }
        if (line.size() == 0) {
            continue;
        }
        //std::cout << ch[1] << std::endl;
        for (int i = 0; i < ch.size(); i++) {
            std::cout << ch[i] << std::endl;
        }
        if (ch[0] == "#") {
        }
        else if (ch[0] == "reset") {
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (row == col) {
                        arr[row][col] = 1;
                    }
                    else {
                        arr[row][col] = 0;
                    }
                    std::cout << arr[row][col] << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (ch[0] == "translate") {
            float tran[3][3] = { {1, 0, std::atof(ch[1].c_str())}, {0, 1, std::atof(ch[2].c_str())}, {0, 0, 1} };
            float product[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        product[row][col] += tran[row][inner] * arr[inner][col];
                    }
                }
            }
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    arr[row][col] = product[row][col];
                    std::cout << arr[row][col] << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (ch[0] == "scale") {

            float sca[3][3] = { {std::atof(ch[1].c_str()), 0, 0}, {0, std::atof(ch[2].c_str()), 0}, { 0, 0, 1 } };
            float product[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        product[row][col] += sca[row][inner] * arr[inner][col];
                    }
                }
            }
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    arr[row][col] = product[row][col];
                    std::cout << arr[row][col] << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (ch[0] == "rotate") {
            //std::cout << std::atof(ch[1].c_str()) * 3.1415926 / 180 << std::endl;
            float rot[3][3] = { {cos(std::atof(ch[1].c_str()) * 3.1415926 / 180), -sin(std::atof(ch[1].c_str()) * 3.1415926 / 180), 0}, {sin(std::atof(ch[1].c_str()) * 3.1415926 / 180), cos(std::atof(ch[1].c_str()) * 3.1415926 / 180), 0}, {0, 0, 1} };
            /*for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    std::cout << rot[row][col] << " ";
                }
                std::cout << std::endl;
            }*/
            float product[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        product[row][col] += rot[row][inner] * arr[inner][col];
                    }
                }
            }
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    arr[row][col] = product[row][col];
                    std::cout << arr[row][col] << " ";
                }
                std::cout << std::endl;
            }
        }

        ch.clear();
        std::cout << "--------------------------------------" << std::endl;
    }

    /*std::cout << ch[6] << std::endl;
    std::cout << ch[7] << std::endl;
    std::cout << ch[8] << std::endl;
    std::cout << ch[9] << std::endl;
    std::cout << ch[10] << std::endl;
    std::cout << ch[11] << std::endl;
    std::cout << ch[12] << std::endl;*/
}

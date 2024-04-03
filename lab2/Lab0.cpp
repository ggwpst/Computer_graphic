#include <GL/glut.h>
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <vector>
#include <cmath>

int window;
void display();
void clear();

class Color {
public:
    float r;
    float g;
    float b;
    Color() { this->Ran(); }
    void Ran();
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
    float getR();
    float getG();
    float getB();
};

void Color::Ran() {
    r = (float)rand() / (float)(RAND_MAX + 1.0);
    g = (float)rand() / (float)(RAND_MAX + 1.0);
    b = (float)rand() / (float)(RAND_MAX + 1.0);
}
float Color::getG() { return g; }
float Color::getB() { return b; }
float Color::getR() { return r; }

class Point {
public:
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
    float getX();
    float getY();
    void drawPoint(Color);
    void setXY(float, float);
};

void Point::setXY(float a, float b) {
    x = a;
    y = b;
}
float Point::getX() { return x; }
float Point::getY() { return y; }
void Point::drawPoint(Color rgb) {
    glPointSize(3);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(rgb.getR(), rgb.getG(), rgb.getB());
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

class Circle {
public:
    Point O;
    float R;
    Circle(Point o, Point p) : O(o) {  //初始化
        this->R = std::sqrt(std::pow(p.getX() - o.getX(), 2) + std::pow(p.getY() - o.getY(), 2));
    }
    void drawCircle(Color rgb) {
        for (float y = R; y >= -R; y--) {
            float x0 = std::sqrt((y + R) * (-y + R));
            Point(int(std::floor(x0 + this->O.getX())), int(std::floor(y + this->O.getY()))).drawPoint(rgb);
            Point(int(std::floor(-x0 + this->O.getX())), int(std::floor(y + this->O.getY()))).drawPoint(rgb);
        }
        for (float x = R; x >= -R; x--) {
            float y0 = std::sqrt((x + R) * (-x + R));
            Point(int(std::floor(x + this->O.getX())), int(std::floor(y0 + this->O.getY()))).drawPoint(rgb);
            Point(int(std::floor(x + this->O.getX())), int(std::floor(-y0 + this->O.getY()))).drawPoint(rgb);
        }
    }
};

class Line {
public:
    Point S;
    Point E;
    enum Base { X, Y };
    Line(Point start, Point end) : S(start), E(end) {  }
    void drawLine(Color rgb) {
        float baseStore[2];
        float counter;
        float slope;            //斜率
        Base base;              //y軸 或 x軸

        if (std::abs(this->E.getY() - this->S.getY()) > std::abs(this->E.getX() - this->S.getX())) {
            base = Y;
            slope = (float)(this->E.getX() - this->S.getX()) / (float)(this->E.getY() - this->S.getY());

            if (this->S.getY() > this->E.getY()) {
                baseStore[0] = this->E.getY();
                baseStore[1] = this->S.getY();
                counter = this->E.getX();
            }
            else {
                baseStore[0] = this->S.getY();
                baseStore[1] = this->E.getY();
                counter = this->S.getX();
            }
        }
        else {
            base = X;
            slope = (float)(this->E.getY() - this->S.getY()) / (float)(this->E.getX() - this->S.getX());

            if (this->S.getX() > this->E.getX()) {
                baseStore[0] = this->E.getX();
                baseStore[1] = this->S.getX();
                counter = this->E.getY();
            }
            else {
                baseStore[0] = this->S.getX();
                baseStore[1] = this->E.getX();
                counter = this->S.getY();
            }
        }

        float b = baseStore[0];

        if (base == Y) {
            Point(int(std::floor(counter)), b++).drawPoint(rgb);
        }
        else if (base == X) {
            Point(b++, int(std::floor(counter))).drawPoint(rgb);
        }
        counter += slope;
        while (b <= baseStore[1]) {
            if (base == Y) {
                Point(int(std::floor(counter)), b++).drawPoint(rgb);
            }
            else if (base == X) {
                Point(b++, int(std::floor(counter))).drawPoint(rgb);
            }
            counter += slope;
        }
    }
};

GLubyte window_pixels[3 * 800 * 800];
GLubyte tmp_window_pixels[3 * 800 * 800];
Color rgb;
Point polygon_first(-1, -1);
Point polygon_last(-1, -1);
std::queue<std::string> lines;
std::string line;
std::vector<std::string> ch;
float arr[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1 } };
std::vector<std::vector<Point>> object;
std::vector<Point> object_data;
std::vector<Point> object_data_trans_left;
std::vector<Point> object_data_trans_bottom;
std::vector<Point> object_data_trans_right;
std::vector<Point> object_data_trans_top;
float viewpoint[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1 } };
float wind[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1 } };
float S[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1 } };
std::string filename;


int main(int argc, char** argv) {
    filename = argv[1];
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);  //窗口位置
    gluOrtho2D(0, 800, 0, 800);
    window = glutCreateWindow("Lab2!");
    glutSwapBuffers();
    glutDisplayFunc(display);
    gluOrtho2D(0, 800, 0, 800);
    glutMainLoop();
    return 0;
}


void display() {
    
    std::ifstream input_file(filename);
    while (std::getline(input_file, line)) {
        lines.push(line);
    }
    while (!lines.empty()) {
        line = lines.front();
        std::cout << line << std::endl;
        lines.pop();
        while (1) {
            if (line.size() == 0) {
                break;
            }
            while (line.find(" ") == 0) {
                line = line.substr(line.find(" ") + 1, line.length());
            }
            while (line.find(" ") == line.length() - 1) {
                line = line.substr(0, line.length()-1);
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
        /*for (int i = 0; i < ch.size(); i++) {
            std::cout << ch[i] << std::endl;
        }*/
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
                }
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
            std::cout << "hi" << std::endl;
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
        else if (ch[0] == "clearData") {
            object.clear();
        }
        else if (ch[0] == "clearScreen") {
            clear();
            glFlush();
            
        }
        else if (ch[0] == "view") {
            glReadPixels(0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
            glDrawPixels(800, 800, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
            Point lb(std::atof(ch[5].c_str()), std::atof(ch[7].c_str()));
            Point rb(std::atof(ch[6].c_str()), std::atof(ch[7].c_str()));
            Point rt(std::atof(ch[6].c_str()), std::atof(ch[8].c_str()));
            Point lt(std::atof(ch[5].c_str()), std::atof(ch[8].c_str()));
            Line(lb, rb).drawLine(rgb);
            Line(rb, rt).drawLine(rgb);
            Line(rt, lt).drawLine(rgb);
            Line(lt, lb).drawLine(rgb);
            for (int i = 0; i < object.size(); i++) {
                for (int j = 0; j < object[i].size(); j++) {
                    //std::cout << object[i][j].getX() << " " << object[i][j].getY() << std::endl;
                    if (j == 0) {
                        polygon_first.setXY(object[i][j].getX(), object[i][j].getY());
                        polygon_last.setXY(object[i][j].getX(), object[i][j].getY());
                        rgb.Ran();
                        continue;
                    }
                    //left
                    if (object[i][j].getX() > std::atof(ch[1].c_str()) && object[i][j - 1].getX() > std::atof(ch[1].c_str())) {
                        //std::cout << object[i][j].getX() << " " << object[i][j].getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                        polygon_last.setXY(object[i][j].getX(), object[i][j].getY());
                        object_data_trans_left.push_back(polygon_last);
                    }
                    else if (object[i][j].getX() <= std::atof(ch[1].c_str()) && object[i][j - 1].getX() > std::atof(ch[1].c_str())) {
                        float m = (float)(object[i][j].getY() - object[i][j - 1].getY()) / (float)(object[i][j].getX() - object[i][j - 1].getX());
                        float b = object[i][j].getY() - (m * object[i][j].getX());
                        float y = m * atof(ch[1].c_str()) + b;
                        polygon_last.setXY(atof(ch[1].c_str()), y);
                        object_data_trans_left.push_back(polygon_last);
                    }
                    else if (object[i][j].getX() <= std::atof(ch[1].c_str()) && object[i][j - 1].getX() <= std::atof(ch[1].c_str())) {
                        polygon_last.setXY(object[i][j].getX(), object[i][j].getY());
                    }
                    else if (object[i][j].getX() > std::atof(ch[1].c_str()) && object[i][j - 1].getX() <= std::atof(ch[1].c_str())) {
                        float m = (float)(object[i][j].getY() - object[i][j - 1].getY()) / (float)(object[i][j].getX() - object[i][j - 1].getX());
                        float b = object[i][j].getY() - (m * object[i][j].getX());
                        float y = m * atof(ch[1].c_str()) + b;
                        polygon_last.setXY(atof(ch[1].c_str()), y);
                        object_data_trans_left.push_back(polygon_last);
                        polygon_last.setXY(object[i][j].getX(), object[i][j].getY());
                        object_data_trans_left.push_back(polygon_last);
                    }
                }
                //last_to_first_left
                if (polygon_first.getX() > std::atof(ch[1].c_str()) && polygon_last.getX() > std::atof(ch[1].c_str())) {
                    //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    object_data_trans_left.push_back(polygon_last);
                }
                else if (polygon_first.getX() <= std::atof(ch[1].c_str()) && polygon_last.getX() > std::atof(ch[1].c_str())) {
                    float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                    float b = polygon_first.getY() - (m * polygon_first.getX());
                    float y = m * atof(ch[1].c_str()) + b;
                    polygon_last.setXY(atof(ch[1].c_str()), y);
                    object_data_trans_left.push_back(polygon_last);
                }
                else if (polygon_first.getX() <= std::atof(ch[1].c_str()) && polygon_last.getX() <= std::atof(ch[1].c_str())) {
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                }
                else if (polygon_first.getX() > std::atof(ch[1].c_str()) && polygon_last.getX() <= std::atof(ch[1].c_str())) {
                    float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                    float b = polygon_first.getY() - (m * polygon_first.getX());
                    float y = m * atof(ch[1].c_str()) + b;
                    polygon_last.setXY(atof(ch[1].c_str()), y);
                    object_data_trans_left.push_back(polygon_last);
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    object_data_trans_left.push_back(polygon_last);
                }
                for (int v = 0; v < object_data_trans_left.size(); v++) {
                    std::cout << object_data_trans_left[v].getX() << " " << object_data_trans_left[v].getY() << " " << "left" << std::endl;
                }
                std::cout << std::endl;

                for (int j = 0; j < object_data_trans_left.size(); j++) {
                    if (j == 0) {
                        polygon_first.setXY(object_data_trans_left[j].getX(), object_data_trans_left[j].getY());
                        polygon_last.setXY(object_data_trans_left[j].getX(), object_data_trans_left[j].getY());
                        rgb.Ran();
                        continue;
                    }
                    //bottom
                    if (object_data_trans_left[j].getY() > std::atof(ch[3].c_str()) && object_data_trans_left[j - 1].getY() > std::atof(ch[3].c_str())) {
                        //std::cout << object_data_trans_left[j].getX() << " " << object_data_trans_left[j].getY() << " " << std::atof(ch[3].c_str()) << "hi" << std::endl;
                        polygon_last.setXY(object_data_trans_left[j].getX(), object_data_trans_left[j].getY());
                        object_data_trans_bottom.push_back(polygon_last);
                    }
                    else if (object_data_trans_left[j].getY() <= std::atof(ch[3].c_str()) && object_data_trans_left[j - 1].getY() > std::atof(ch[3].c_str())) {
                        if (object_data_trans_left[j].getX() == object_data_trans_left[j - 1].getX()) {
                            polygon_last.setXY(object_data_trans_left[j].getX(), std::atof(ch[3].c_str()));
                            object_data_trans_bottom.push_back(polygon_last);
                        }
                        else {
                            float m = (float)(object_data_trans_left[j].getY() - object_data_trans_left[j - 1].getY()) / (float)(object_data_trans_left[j].getX() - object_data_trans_left[j - 1].getX());
                            float b = object_data_trans_left[j].getY() - (m * object_data_trans_left[j].getX());
                            float x = (std::atof(ch[3].c_str()) - b) / m;
                            polygon_last.setXY(x, std::atof(ch[3].c_str()));
                            object_data_trans_bottom.push_back(polygon_last);
                        }
                    }
                    else if (object_data_trans_left[j].getY() <= std::atof(ch[3].c_str()) && object_data_trans_left[j - 1].getY() <= std::atof(ch[3].c_str())) {
                        polygon_last.setXY(object_data_trans_left[j].getX(), object_data_trans_left[j].getY());
                    }
                    else if (object_data_trans_left[j].getY() > std::atof(ch[3].c_str()) && object_data_trans_left[j - 1].getY() <= std::atof(ch[3].c_str())) {
                        if (object_data_trans_left[j].getX() == object_data_trans_left[j - 1].getX()) {
                            polygon_last.setXY(object_data_trans_left[j].getX(), atof(ch[3].c_str()));
                            object_data_trans_bottom.push_back(polygon_last);
                            polygon_last.setXY(object_data_trans_left[j].getX(), object_data_trans_left[j].getY());
                            object_data_trans_bottom.push_back(polygon_last);
                        }
                        else {
                            float m = (float)(object_data_trans_left[j].getY() - object_data_trans_left[j - 1].getY()) / (float)(object_data_trans_left[j].getX() - object_data_trans_left[j - 1].getX());
                            float b = object_data_trans_left[j].getY() - (m * object_data_trans_left[j].getX());
                            float x = (std::atof(ch[3].c_str()) - b) / m;
                            polygon_last.setXY(x, atof(ch[3].c_str()));
                            object_data_trans_bottom.push_back(polygon_last);
                            polygon_last.setXY(object_data_trans_left[j].getX(), object_data_trans_left[j].getY());
                            object_data_trans_bottom.push_back(polygon_last);
                        }
                    }
                }
                object_data_trans_left.clear();
                //last_to_first_bottom
                if (polygon_first.getY() > std::atof(ch[3].c_str()) && polygon_last.getY() > std::atof(ch[3].c_str())) {
                    
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    object_data_trans_bottom.push_back(polygon_last);
                    //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                    
                }
                else if (polygon_first.getY() <= std::atof(ch[3].c_str()) && polygon_last.getY() > std::atof(ch[3].c_str())) {
                    if (polygon_first.getX() == polygon_last.getX()) {
                        polygon_last.setXY(polygon_first.getX(), atof(ch[3].c_str()));
                        object_data_trans_bottom.push_back(polygon_last);
                    }
                    else {
                        float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                        float b = polygon_first.getY() - (m * polygon_first.getX());
                        float x = (std::atof(ch[3].c_str()) - b) / m;
                        polygon_last.setXY(x, atof(ch[3].c_str()));
                        object_data_trans_bottom.push_back(polygon_last);
                        //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                    }
                }
                else if (polygon_first.getY() <= std::atof(ch[3].c_str()) && polygon_last.getY() <= std::atof(ch[3].c_str())) {
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                }
                else if (polygon_first.getY() > std::atof(ch[3].c_str()) && polygon_last.getY() <= std::atof(ch[3].c_str())) {
                    if (polygon_first.getX() == polygon_last.getX()) {
                        polygon_last.setXY(polygon_first.getX(), atof(ch[3].c_str()));
                        object_data_trans_bottom.push_back(polygon_last);
                        polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                        object_data_trans_bottom.push_back(polygon_last);
                    }
                    else {
                        float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                        float b = polygon_first.getY() - (m * polygon_first.getX());
                        float x = (std::atof(ch[3].c_str()) - b) / m;
                        polygon_last.setXY(x, atof(ch[3].c_str()));
                        object_data_trans_bottom.push_back(polygon_last);
                        polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                        object_data_trans_bottom.push_back(polygon_last);
                        //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                    }
                }
                for (int v = 0; v < object_data_trans_bottom.size(); v++) {
                    std::cout << object_data_trans_bottom[v].getX() << " " << object_data_trans_bottom[v].getY() << " " << "bottom" << std::endl;
                }
                std::cout << std::endl;
                
                for (int j = 0; j < object_data_trans_bottom.size(); j++) {
                    if (j == 0) {
                        std::cout << "1" << std::endl;
                        polygon_first.setXY(object_data_trans_bottom[j].getX(), object_data_trans_bottom[j].getY());
                        polygon_last.setXY(object_data_trans_bottom[j].getX(), object_data_trans_bottom[j].getY());
                        rgb.Ran();
                        for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "1" << std::endl;
                        }
                        continue;
                    }
                    //right
                    if (object_data_trans_bottom[j].getX() < std::atof(ch[2].c_str()) && object_data_trans_bottom[j - 1].getX() < std::atof(ch[2].c_str())) {
                        std::cout << "2" << std::endl;
                        polygon_last.setXY(object_data_trans_bottom[j].getX(), object_data_trans_bottom[j].getY());
                        object_data_trans_right.push_back(polygon_last);
                        
                    }
                    else if (object_data_trans_bottom[j].getX() >= std::atof(ch[2].c_str()) && object_data_trans_bottom[j - 1].getX() < std::atof(ch[2].c_str())) {
                        std::cout << "3" << std::endl;
                        float m = (float)(object_data_trans_bottom[j].getY() - object_data_trans_bottom[j - 1].getY()) / (float)(object_data_trans_bottom[j].getX() - object_data_trans_bottom[j - 1].getX());
                        float b = object_data_trans_bottom[j].getY() - (m * object_data_trans_bottom[j].getX());
                        float y = m * atof(ch[2].c_str()) + b;
                        polygon_last.setXY(atof(ch[2].c_str()), y);
                        object_data_trans_right.push_back(polygon_last);
                        for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "3" << std::endl;
                        }
                    }
                    else if (object_data_trans_bottom[j].getX() >= std::atof(ch[2].c_str()) && object_data_trans_bottom[j - 1].getX() >= std::atof(ch[2].c_str())) {
                        std::cout << "4" << std::endl;
                        polygon_last.setXY(object_data_trans_bottom[j].getX(), object_data_trans_bottom[j].getY());
                        for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "4" << std::endl;
                        }
                    }
                    else if (object_data_trans_bottom[j].getX() < std::atof(ch[2].c_str()) && object_data_trans_bottom[j - 1].getX() >= std::atof(ch[2].c_str())) {
                        std::cout << "5" << std::endl;
                        float m = (float)(object_data_trans_bottom[j].getY() - object_data_trans_bottom[j - 1].getY()) / (float)(object_data_trans_bottom[j].getX() - object_data_trans_bottom[j - 1].getX());
                        float b = object_data_trans_bottom[j].getY() - (m * object_data_trans_bottom[j].getX());
                        float y = m * atof(ch[2].c_str()) + b;
                        //std::cout << m << " " << b << " " << y << std::endl;
                        polygon_last.setXY(atof(ch[2].c_str()), y);
                        object_data_trans_right.push_back(polygon_last);
                        polygon_last.setXY(object_data_trans_bottom[j].getX(), object_data_trans_bottom[j].getY());
                        object_data_trans_right.push_back(polygon_last);
                        for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "5" << std::endl;
                        }
                    }
                }
                object_data_trans_bottom.clear();
                //last_to_first_right
                //std::cout << object_data_trans_right.size() << std::endl;
                //std::cout << polygon_first.getX() << " " << std::atof(ch[2].c_str()) << " " << polygon_last.getX() << std::endl;
                if (polygon_first.getX() < std::atof(ch[2].c_str()) && polygon_last.getX() < std::atof(ch[2].c_str())) {
                    std::cout << "6" << std::endl;
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    object_data_trans_right.push_back(polygon_last);
                    /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                        std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << "hi+++++++++++++++++" << std::endl;
                    }*/
                }
                else if (polygon_first.getX() >= std::atof(ch[2].c_str()) && polygon_last.getX() < std::atof(ch[2].c_str())) {
                    std::cout << "7" << std::endl;
                    float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                    float b = polygon_first.getY() - (m * polygon_first.getX());
                    float y = m * atof(ch[2].c_str()) + b;
                    polygon_last.setXY(atof(ch[2].c_str()), y);
                    object_data_trans_right.push_back(polygon_last);
                }
                else if (polygon_first.getX() >= std::atof(ch[2].c_str()) && polygon_last.getX() >= std::atof(ch[2].c_str())) {
                    std::cout << "8" << std::endl;
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    for (int v = 0; v < object_data_trans_right.size(); v++) {
                        std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "8" << std::endl;
                    }
                }
                else if (polygon_first.getX() < std::atof(ch[2].c_str()) && polygon_last.getX() >= std::atof(ch[2].c_str())) {
                    //std::cout << "9" << std::endl;
                    float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                    float b = polygon_first.getY() - (m * polygon_first.getX());
                    float y = m * atof(ch[2].c_str()) + b;
                    polygon_last.setXY(atof(ch[2].c_str()), y);
                    object_data_trans_right.push_back(polygon_last);
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    object_data_trans_right.push_back(polygon_last);
                }
                //std::cout << object_data_trans_right.size() << std::endl;
                for (int v = 0; v < object_data_trans_right.size(); v++) {
                    std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "right" << std::endl;
                }
                /*std::cout << std::endl;
                std::cout << object_data_trans_right.size() << std::endl;*/

                for (int j = 0; j < object_data_trans_right.size(); j++) {
                    //std::cout << "hi" << std:: endl;
                    if (j == 0) {
                        polygon_first.setXY(object_data_trans_right[j].getX(), object_data_trans_right[j].getY());
                        polygon_last.setXY(object_data_trans_right[j].getX(), object_data_trans_right[j].getY());
                        rgb.Ran();
                        std::cout << "hi2" << std::endl;
                        continue;
                    }
                    //top
                    if (object_data_trans_right[j].getY() <= std::atof(ch[4].c_str()) && object_data_trans_right[j - 1].getY() <= std::atof(ch[4].c_str())) {
                        polygon_last.setXY(object_data_trans_right[j].getX(), object_data_trans_right[j].getY());
                        object_data_trans_top.push_back(polygon_last);
                    }
                    else if (object_data_trans_right[j].getY() > std::atof(ch[4].c_str()) && object_data_trans_right[j - 1].getY() <= std::atof(ch[4].c_str())) {
                        if (object_data_trans_right[j].getX() == object_data_trans_right[j - 1].getX()) {
                            polygon_last.setXY(object_data_trans_right[j].getX(), std::atof(ch[4].c_str()));
                            object_data_trans_top.push_back(polygon_last);
                        }
                        else {
                            float m = (float)(object_data_trans_right[j].getY() - object_data_trans_right[j - 1].getY()) / (float)(object_data_trans_right[j].getX() - object_data_trans_right[j - 1].getX());
                            float b = object_data_trans_right[j].getY() - (m * object_data_trans_right[j].getX());
                            float x = (std::atof(ch[4].c_str()) - b) / m;
                            polygon_last.setXY(x, std::atof(ch[4].c_str()));
                            object_data_trans_top.push_back(polygon_last);
                        }
                    }
                    else if (object_data_trans_right[j].getY() > std::atof(ch[4].c_str()) && object_data_trans_right[j - 1].getY() > std::atof(ch[4].c_str())) {
                        std::cout << "456789" << std::endl;
                        polygon_last.setXY(object_data_trans_right[j].getX(), object_data_trans_right[j].getY());
                    }
                    else if (object_data_trans_right[j].getY() <= std::atof(ch[4].c_str()) && object_data_trans_right[j - 1].getY() > std::atof(ch[4].c_str())) {
                        if (object_data_trans_right[j].getX() == object_data_trans_right[j - 1].getX()) {
                            polygon_last.setXY(object_data_trans_right[j].getX(), atof(ch[4].c_str()));
                            object_data_trans_top.push_back(polygon_last);
                            polygon_last.setXY(object_data_trans_right[j].getX(), object_data_trans_right[j].getY());
                            object_data_trans_top.push_back(polygon_last);
                        }
                        else {
                            float m = (float)(object_data_trans_right[j].getY() - object_data_trans_right[j - 1].getY()) / (float)(object_data_trans_right[j].getX() - object_data_trans_right[j - 1].getX());
                            float b = object_data_trans_right[j].getY() - (m * object_data_trans_right[j].getX());
                            float x = (std::atof(ch[4].c_str()) - b) / m;
                            polygon_last.setXY(x, atof(ch[4].c_str()));
                            object_data_trans_top.push_back(polygon_last);
                            polygon_last.setXY(object_data_trans_right[j].getX(), object_data_trans_right[j].getY());
                            object_data_trans_top.push_back(polygon_last);
                        }
                    }
                }
                object_data_trans_right.clear();
                //last_to_first_top
                if (polygon_first.getY() <= std::atof(ch[4].c_str()) && polygon_last.getY() <= std::atof(ch[4].c_str())) {
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                    object_data_trans_top.push_back(polygon_last);
                }
                else if (polygon_first.getY() > std::atof(ch[4].c_str()) && polygon_last.getY() <= std::atof(ch[4].c_str())) {
                    if (polygon_first.getX() == polygon_last.getX()) {
                        polygon_last.setXY(polygon_first.getX(), atof(ch[4].c_str()));
                        object_data_trans_top.push_back(polygon_last);
                    }
                    else {
                        float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                        float b = polygon_first.getY() - (m * polygon_first.getX());
                        float x = (std::atof(ch[4].c_str()) - b) / m;
                        polygon_last.setXY(x, atof(ch[4].c_str()));
                        object_data_trans_top.push_back(polygon_last);
                    }
                }
                else if (polygon_first.getY() > std::atof(ch[4].c_str()) && polygon_last.getY() > std::atof(ch[4].c_str())) {
                    polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                }
                else if (polygon_first.getY() <= std::atof(ch[4].c_str()) && polygon_last.getY() > std::atof(ch[4].c_str())) {
                    if (polygon_first.getX() == polygon_last.getX()) {
                        polygon_last.setXY(polygon_first.getX(), atof(ch[4].c_str()));
                        object_data_trans_top.push_back(polygon_last);
                        polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                        object_data_trans_top.push_back(polygon_last);
                    }
                    else {
                        float m = (float)(polygon_first.getY() - polygon_last.getY()) / (float)(polygon_first.getX() - polygon_last.getX());
                        float b = polygon_first.getY() - (m * polygon_first.getX());
                        float x = (std::atof(ch[4].c_str()) - b) / m;
                        polygon_last.setXY(x, atof(ch[4].c_str()));
                        object_data_trans_top.push_back(polygon_last);
                        polygon_last.setXY(polygon_first.getX(), polygon_first.getY());
                        object_data_trans_top.push_back(polygon_last);
                    }
                }
                for (int v = 0; v < object_data_trans_top.size(); v++) {
                    std::cout << object_data_trans_top[v].getX() << " " << object_data_trans_top[v].getY() << " " << "top" << std::endl;
                }
                std::cout << std::endl;

                float WMV[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0 } };
                float tmp[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0 } };
                wind[0][2] = -atof(ch[1].c_str());
                wind[1][2] = -atof(ch[3].c_str());
                S[0][0] = (atof(ch[6].c_str()) - atof(ch[5].c_str())) / (atof(ch[2].c_str()) - atof(ch[1].c_str()));
                S[1][1] = (atof(ch[8].c_str()) - atof(ch[7].c_str())) / (atof(ch[4].c_str()) - atof(ch[3].c_str()));
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        for (int inner = 0; inner < 3; inner++) {
                            tmp[row][col] += S[row][inner] * wind[inner][col];
                        }
                    }
                }
                for (int s = 0; s < 3; s++) {
                    for (int t = 0; t < 3; t++) {
                        std::cout << wind[s][t] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
                for (int s = 0; s < 3; s++) {
                    for (int t = 0; t < 3; t++) {
                        std::cout << S[s][t] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
                for (int s = 0; s < 3; s++) {
                    for (int t = 0; t < 3; t++) {
                        std::cout << tmp[s][t] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
                viewpoint[0][2] = atof(ch[5].c_str());
                viewpoint[1][2] = atof(ch[7].c_str());
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        for (int inner = 0; inner < 3; inner++) {
                            WMV[row][col] += viewpoint[row][inner] * tmp[inner][col];
                        }
                    }
                }
                for (int s = 0; s < 3; s++) {
                    for(int t = 0; t < 3; t++) {
                        std::cout << WMV[s][t] << " ";
                    }
                    std::cout << std::endl;
                }

                
                //std::cout << "size : " << object_data_trans_top.size() << std::endl;
                for (int k = 0; k < object_data_trans_top.size(); k++) {
                    float point[3][1] = { {1}, {1}, {1} };
                    float point_trans[3][1] = { {0}, {0}, {0} };
                    std::cout << object_data_trans_top[k].getX() << " " << object_data_trans_top[k].getY() << std::endl;
                    point[0][0] = object_data_trans_top[k].getX();
                    point[1][0] = object_data_trans_top[k].getY();
                    for (int s = 0; s < 3; s++) {
                        for (int t = 0; t < 1; t++) {
                            std::cout<< point[s][t];
                        }
                        std::cout << std::endl;
                    }
                    for (int row = 0; row < 3; row++) {
                        for (int col = 0; col < 1; col++) {
                            for (int inner = 0; inner < 3; inner++) {
                                point_trans[row][col] += WMV[row][inner] * point[inner][col];
                            }
                        }
                    }
                    std::cout << std::endl;
                    for (int s = 0; s < 3; s++) {
                        for (int t = 0; t < 1; t++) {
                            std::cout<<point_trans[s][t];
                        }
                        std::cout<<std::endl;
                    }
                    object_data_trans_top[k].setXY(int(std::floor(point_trans[0][0] + 0.5)), int(std::floor(point_trans[1][0] + 0.5)));
                    std::cout << object_data_trans_top[k].getX() << " " << object_data_trans_top[k].getY() << std::endl;
                    //std::cout << k << std::endl;
                    if (k == 0) {
                        //glReadPixels(0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
                        //object_data_trans_top[0].drawPoint(rgb);
                        std::cout << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl;
                        continue;
                    }
                    else if (k == object_data_trans_top.size() - 1) {
                        Line(object_data_trans_top[k], object_data_trans_top[k-1]).drawLine(rgb);
                        Line(object_data_trans_top[k], object_data_trans_top[0]).drawLine(rgb);
                        //std::cout << "456" << std::endl;
                    }
                    else {
                        Line(object_data_trans_top[k], object_data_trans_top[k-1]).drawLine(rgb);
                        //std::cout << "789" << std::endl;
                    }
                }
                object_data_trans_top.clear();
            }
            glFlush();
            system("pause");
        }
        else if (ch[0] == "square") {
            Point lt(-1, 1);
            Point lb(-1, -1);
            Point rt(1, 1);
            Point rb(1, -1);
            float point_lt_b[3][1] = { {-1}, {1}, {1} };
            float point_lb_b[3][1] = { {-1}, {-1}, {1} };
            float point_rt_b[3][1] = { {1}, {1}, {1} };
            float point_rb_b[3][1] = { {1}, {-1}, {1} };
            float product_lt[3][1] = { {0}, {0}, {0} };
            float product_lb[3][1] = { {0}, {0}, {0} };
            float product_rt[3][1] = { {0}, {0}, {0} };
            float product_rb[3][1] = { {0}, {0}, {0} };
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 1; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        product_lt[row][col] += arr[row][inner] * point_lt_b[inner][col];
                        product_lb[row][col] += arr[row][inner] * point_lb_b[inner][col];
                        product_rt[row][col] += arr[row][inner] * point_rt_b[inner][col];
                        product_rb[row][col] += arr[row][inner] * point_rb_b[inner][col];
                    }
                }
            }
            lt.setXY(product_lt[0][0], product_lt[1][0]);
            lb.setXY(product_lb[0][0], product_lb[1][0]);
            rt.setXY(product_rt[0][0], product_rt[1][0]);
            rb.setXY(product_rb[0][0], product_rb[1][0]);
            object_data.push_back(lt);
            object_data.push_back(lb);
            object_data.push_back(rb);
            object_data.push_back(rt);
            for (int p = 0; p < object_data.size(); p++) {
                std::cout << object_data[p].getX() << " " << object_data[p].getY() << std::endl;
            }
            object.push_back(object_data);
            object_data.clear();
        }
        else if (ch[0] == "triangle") {
            Point tri1(0, 1);
            Point tri2(-1, -1);
            Point tri3(1, -1);
            float point_tri1_b[3][1] = { {0}, {1}, {1} };
            float point_tri2_b[3][1] = { {-1}, {-1}, {1} };
            float point_tri3_b[3][1] = { {1}, {-1}, {1} };
            float product_tri1[3][1] = { {0}, {0}, {0} };
            float product_tri2[3][1] = { {0}, {0}, {0} };
            float product_tri3[3][1] = { {0}, {0}, {0} };
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 1; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        product_tri1[row][col] += arr[row][inner] * point_tri1_b[inner][col];
                        product_tri2[row][col] += arr[row][inner] * point_tri2_b[inner][col];
                        product_tri3[row][col] += arr[row][inner] * point_tri3_b[inner][col];
                    }
                }
            }
            for (int m = 0; m < 3; m++) {
                for (int n = 0; n < 1; n++) {
                    std::cout << product_tri1[m][n] << " ";
                }
                std::cout << std::endl;
            }

            tri1.setXY(product_tri1[0][0], product_tri1[1][0]);
            tri2.setXY(product_tri2[0][0], product_tri2[1][0]);
            tri3.setXY(product_tri3[0][0], product_tri3[1][0]);
            object_data.push_back(tri1);
            object_data.push_back(tri2);
            object_data.push_back(tri3);
            for (int p = 0; p < object_data.size(); p++) {
                std::cout << object_data[p].getX() << " " << object_data[p].getY() << std::endl;
            }
            object.push_back(object_data);
            object_data.clear();
        }
        else if (ch[0] == "end") {
            system("pause");
            glutDestroyWindow(window);
        }
        ch.clear();
        std::cout << "--------------------------------------" << std::endl;
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClear(GL_COLOR_BUFFER_BIT);  
    glFlush();
}

void clear() {
    glReadPixels(0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
}
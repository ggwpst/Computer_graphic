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
    Color() {};
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
    float getR();
    float getG();
    float getB();
    void setRGB(float, float, float);
};
float Color::getG() { return g; }
float Color::getB() { return b; }
float Color::getR() { return r; }
void Color::setRGB(float a, float c, float d) {
    r = a;
    g = c;
    b = d;
}

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
    glPointSize(1);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(rgb.getR(), rgb.getG(), rgb.getB());
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

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

Color RGB;
int window_width;
int window_height;
GLubyte window_pixels[3 * 800 * 800];
float polygon_first[3][1];
float polygon_last[3][1];
std::queue<std::string> lines;
std::string line;
std::vector<std::string> ch;
float arr[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0 }, {0, 0, 0, 1} };

std::vector<std::vector<float>> object_data_trans_left;
std::vector<float> object_data_trans_left_tmp;
std::vector<std::vector<float>> object_data_trans_bottom;
std::vector<float> object_data_trans_bottom_tmp;
std::vector<std::vector<float>> object_data_trans_right;
std::vector<float> object_data_trans_right_tmp;
std::vector<std::vector<float>> object_data_trans_top;
std::vector<float> object_data_trans_top_tmp;
std::vector<std::vector<std::vector<float>>> object_image_space;
std::vector<std::vector<std::vector<float>>> object_screen_space;

float WMV[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0 } };
std::string filename;
std::vector<std::vector<float>> obs_v;
std::string ob;
std::vector<float> ob_v_split;
std::vector<float> ob_f_split;
std::vector<std::vector<std::vector<float>>> obs_v_trans;
std::vector<std::vector<float>> v_trans;
std::vector<std::vector<std::vector<float>>> v_eye_space_plane;
std::vector<std::vector<std::vector<float>>> v_projection_space_plane;
std::vector<std::vector<float>> v_eye_space_tri;
std::vector<std::vector<float>> v_projection_space_tri;
std::vector<float> v_eye_space_co;
std::vector<float> v_projection_space_co;
std::vector<std::vector<std::vector<float>>> clip;
std::vector<float> co;
std::vector<std::vector<float>> clip_w_plus_x;
std::vector<std::vector<float>> clip_w_minus_x;
std::vector<std::vector<float>> clip_w_plus_y;
std::vector<std::vector<float>> clip_w_minus_y;
std::vector<std::vector<float>> clip_z;
std::vector<std::vector<float>> clip_w_minus_z;
float eye_x;
float eye_y;
float eye_z;
float Tilt_Mirror_GRM_Eye[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
float perspective_projection[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
float left;
float bottom;
float right;
float top;
float Or;
float Og;
float Ob;
float KIr;
float KIg;
float KIb;
float Kd;
float Ks;
float N;
float Br;
float Bg;
float Bb;
int index;
std::vector<std::vector<float>> light;
int count = 0;
std::vector<float> op;
float Ir;
float Ig;
float Ib;
std::vector<std::vector<std::vector<float>>> z_value;
std::vector<std::vector<float>> z_value_tmp;
std::vector<float> z_value_co;
std::vector<std::vector<float>> ob_color;
std::vector<float> ob_color_tmp;
float vxl;
float vxr;
float vyb;
float vyt;


int main(int argc, char** argv) {
    filename = argv[1];
    glutInit(&argc, argv);
    std::ifstream input_file(filename);
    while (std::getline(input_file, line)) {
        lines.push(line);
    }
    line = lines.front();
    std::cout << line << std::endl;
    lines.pop();
    window_width = atoi(line.substr(0, line.find(" ")).c_str());
    window_height = atoi(line.substr(line.find(" ") + 1, line.length()).c_str());

    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);  //窗口位置
    gluOrtho2D(0, window_width, 0, window_height);
    window = glutCreateWindow("Lab3!");
    glutSwapBuffers();
    glutDisplayFunc(display);
    gluOrtho2D(0, window_width, 0, window_height);
    glutMainLoop();
    return 0;
}


void display() {
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
            while (line.find_last_of(" ") == line.length() - 1) {
                line = line.substr(0, line.length() - 1);
            }
            ch.push_back(line.substr(0, line.find(" ")));
            line = line.substr(line.find(" ") + 1, line.length());
            if (line.find(" ") == -1) {
                if (line == "reset" || line == "clearData" || line == "diaplay" || line == "end") {
                    break;
                }
                ch.push_back(line);
                break;
            }
            /*for (int i = 0; i < ch.size(); i++) {
                std::cout << ch[i] << std::endl;
            }*/
        }
        //std::cout << ch[0] << std::endl;
        if (line.size() == 0) {
            continue;
        }
        /*for (int i = 0; i < ch.size(); i++) {
            std::cout << ch[i] << std::endl;
        }*/
        if (ch[0] == "#") {

        }
        else if (ch[0] == "ambient") {
            KIr = atof(ch[1].c_str());
            KIg = atof(ch[2].c_str());
            KIb = atof(ch[3].c_str());
        }
        else if (ch[0] == "reset") {
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
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
            float tran[4][4] = { {1, 0, 0, std::atof(ch[1].c_str())}, {0, 1, 0, std::atof(ch[2].c_str())}, {0, 0, 1, std::atof(ch[3].c_str())}, {0, 0, 0, 1} };
            float product[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int inner = 0; inner < 4; inner++) {
                        product[row][col] += tran[row][inner] * arr[inner][col];
                    }
                }
            }
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    arr[row][col] = product[row][col];
                    std::cout << arr[row][col] << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (ch[0] == "scale") {
            //std::cout << "hi" << std::endl;
            float sca[4][4] = { {std::atof(ch[1].c_str()), 0, 0, 0}, {0, std::atof(ch[2].c_str()), 0, 0}, { 0, 0, std::atof(ch[3].c_str()), 0 }, {0, 0, 0, 1} };
            float product[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int inner = 0; inner < 4; inner++) {
                        product[row][col] += sca[row][inner] * arr[inner][col];
                    }
                }
            }
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    arr[row][col] = product[row][col];
                    std::cout << arr[row][col] << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (ch[0] == "rotate") {
            //std::cout << std::atof(ch[1].c_str()) * 3.1415926 / 180 << std::endl;
            float rotX[4][4] = { {1, 0, 0, 0}, {0, cos(std::atof(ch[1].c_str()) * 3.1415926 / 180), -sin(std::atof(ch[1].c_str()) * 3.1415926 / 180), 0}, {0, sin(std::atof(ch[1].c_str()) * 3.1415926 / 180), cos(std::atof(ch[1].c_str()) * 3.1415926 / 180), 0}, {0, 0, 0, 1} };
            float rotY[4][4] = { {cos(std::atof(ch[2].c_str()) * 3.1415926 / 180), 0, sin(std::atof(ch[2].c_str()) * 3.1415926 / 180), 0}, {0, 1, 0, 0}, {-sin(std::atof(ch[2].c_str()) * 3.1415926 / 180), 0, cos(std::atof(ch[2].c_str()) * 3.1415926 / 180), 0}, {0, 0, 0, 1} };
            float rotZ[4][4] = { {cos(std::atof(ch[3].c_str()) * 3.1415926 / 180), -sin(std::atof(ch[3].c_str()) * 3.1415926 / 180), 0, 0}, {sin(std::atof(ch[3].c_str()) * 3.1415926 / 180), cos(std::atof(ch[3].c_str()) * 3.1415926 / 180), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
            /*for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    std::cout << rot[row][col] << " ";
                }
                std::cout << std::endl;
            }*/
            float productY[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
            float productZ[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
            float productX[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int inner = 0; inner < 4; inner++) {
                        productY[row][col] += rotY[row][inner] * arr[inner][col];
                    }
                }
            }
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int inner = 0; inner < 4; inner++) {
                        productZ[row][col] += rotZ[row][inner] * productY[row][col];
                    }
                }
            }
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int inner = 0; inner < 4; inner++) {
                        productX[row][col] += rotX[row][inner] * productZ[row][col];
                    }
                }
            }
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    arr[row][col] = productX[row][col];
                    std::cout << arr[row][col] << " ";
                }
                std::cout << std::endl;
            }
        }
        else if (ch[0] == "object") {
            
            Or = atof(ch[2].c_str());
            Og = atof(ch[3].c_str());
            Ob = atof(ch[4].c_str());
            Kd = atof(ch[5].c_str());
            Ks = atof(ch[6].c_str());
            N = atof(ch[7].c_str());
            ob_color_tmp.push_back(Or);
            ob_color_tmp.push_back(Og);
            ob_color_tmp.push_back(Ob);
            ob_color_tmp.push_back(Kd);
            ob_color_tmp.push_back(Ks);
            ob_color_tmp.push_back(N);
            //std::cout << "hi" << std::endl;
            //std::cout << ch[1] << std::endl;
            std::ifstream input_file(ch[1]);
            //std::cout << "hi" << std::endl;
            while (std::getline(input_file, ob)) {
                if (ob.size() == 0) {
                    continue;
                }
                while (ob.find(" ") == 0) {
                    ob = ob.substr(ob.find(" ") + 1, ob.length());
                }
                while (ob.find_last_of(" ") == ob.length() - 1) {
                    ob = ob.substr(0, ob.length() - 1);
                }

                if (ob[0] == 'v') {
                    ob = ob.substr(ob.find(" ") + 1, ob.length());
                    while (1) {
                        ob_v_split.push_back(atof(ob.substr(0, ob.find(" ")).c_str()));
                        ob = ob.substr(ob.find(" ") + 1, ob.length());
                        if (ob.find(" ") == -1) {
                            ob_v_split.push_back(atof(ob.c_str()));
                            break;
                        }
                    }
                    float tri[4][1] = { {1}, {1}, {1}, {1} };
                    tri[0][0] = ob_v_split[0];
                    tri[1][0] = ob_v_split[1];
                    tri[2][0] = ob_v_split[2];
                    float product[4][1] = { {0}, {0}, {0}, {0} };
                    for (int row = 0; row < 4; row++) {
                        for (int col = 0; col < 1; col++) {
                            for (int in = 0; in < 4; in++) {
                                product[row][col] += arr[row][in] * tri[in][col];
                            }
                        }
                    }
                    ob_v_split.clear();
                    ob_v_split.push_back(product[0][0]);
                    ob_v_split.push_back(product[1][0]);
                    ob_v_split.push_back(product[2][0]);
                    obs_v.push_back(ob_v_split);
                    ob_v_split.clear();
                }
                else if (ob[0] == 'f') {

                    ob = ob.substr(ob.find(" ") + 1, ob.length());
                    while (1) {
                        ob_f_split.push_back(atoi(ob.substr(0, ob.find(" ")).c_str()) - 1);
                        ob = ob.substr(ob.find(" ") + 1, ob.length());
                        if (ob.find(" ") == -1) {
                            ob_f_split.push_back(atoi(ob.c_str()) - 1);
                            break;
                        }
                    }
                    //std::cout << ob_f_split.size() << std::endl;
                    if (ob_f_split.size() == 4) {
                        //std::cout << "hi" << std::endl;
                        v_trans.push_back(obs_v[ob_f_split[0]]);
                        v_trans.push_back(obs_v[ob_f_split[1]]);
                        v_trans.push_back(obs_v[ob_f_split[3]]);
                        obs_v_trans.push_back(v_trans);
                        v_trans.clear();
                        ob_color.push_back(ob_color_tmp);
                        v_trans.push_back(obs_v[ob_f_split[1]]);
                        v_trans.push_back(obs_v[ob_f_split[2]]);
                        v_trans.push_back(obs_v[ob_f_split[3]]);
                        obs_v_trans.push_back(v_trans);
                        v_trans.clear();
                        ob_color.push_back(ob_color_tmp);
                        ob_f_split.clear();
                    }
                    else if (ob_f_split.size() == 3) {
                        v_trans.push_back(obs_v[ob_f_split[0]]);
                        v_trans.push_back(obs_v[ob_f_split[1]]);
                        v_trans.push_back(obs_v[ob_f_split[2]]);
                        obs_v_trans.push_back(v_trans);
                        v_trans.clear();
                        ob_color.push_back(ob_color_tmp);
                        ob_f_split.clear();
                        //std::cout << obs_f.size() << std::endl;
                        /*for (int i = 0; i < obs_f.size(); i++) {
                            for (int j = 0; j < obs_f[i].size(); j++) {
                                std::cout << obs_f[i][j] << " ";
                            }
                            std::cout << std::endl;
                        }*/
                    }
                }
            }
            /*for (int i = 0; i < obs_v_trans.size(); i++) {
                for (int j = 0; j < obs_v_trans[i].size(); j++) {
                    for (int k = 0; k < obs_v_trans[i][j].size(); k++) {
                        std::cout << obs_v_trans[i][j][k] << " ";
                    }
                    std::cout << "hi ";
                }
                std::cout << std::endl;
            }*/
            obs_v.clear();
            /*for (int i = 0; i < obs_f.size(); i++) {
                for (int j = 0; j < obs_f[i].size(); j++) {
                    std::cout << obs_f[i][j] << " ";
                }
                std::cout << std::endl;
            }*/
            ob_color_tmp.clear();
        }
        else if (ch[0] == "background") {
            Br = atof(ch[1].c_str());
            Bg = atof(ch[2].c_str());
            Bb = atof(ch[3].c_str());
        }
        else if (ch[0] == "observer") {
            float Tilt[4][4] = { {cos(std::atof(ch[7].c_str()) * 3.1415926 / 180), -sin(std::atof(ch[7].c_str()) * 3.1415926 / 180), 0, 0}, {sin(std::atof(ch[7].c_str()) * 3.1415926 / 180), cos(std::atof(ch[7].c_str()) * 3.1415926 / 180), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
            float Mirror[4][4] = { {-1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
            float Forward[3][1] = { {atof(ch[4].c_str()) - atof(ch[1].c_str())}, {atof(ch[5].c_str()) - atof(ch[2].c_str())}, {atof(ch[6].c_str()) - atof(ch[3].c_str())} };
            float Right[3][1] = { {1 * Forward[2][0] - 0 * Forward[1][0]}, {0 * Forward[0][0] - 0 * Forward[2][0]}, {0 * Forward[1][0] - 1 * Forward[0][0]} };
            float Up[3][1] = { {Forward[1][0] * Right[2][0] - Forward[2][0] * Right[1][0]}, {Forward[2][0] * Right[0][0] - Forward[0][0] * Right[2][0]}, {Forward[0][0] * Right[1][0] - Forward[1][0] * Right[0][0]} };
            float GRM[4][4] = { {Right[0][0] / sqrt(pow(Right[0][0], 2) + pow(Right[1][0], 2) + pow(Right[2][0], 2)), Right[1][0] / sqrt(pow(Right[0][0], 2) + pow(Right[1][0], 2) + pow(Right[2][0], 2)), Right[2][0] / sqrt(pow(Right[0][0], 2) + pow(Right[1][0], 2) + pow(Right[2][0], 2)), 0}, {Up[0][0] / sqrt(pow(Up[0][0], 2) + pow(Up[1][0], 2) + pow(Up[2][0], 2)), Up[1][0] / sqrt(pow(Up[0][0], 2) + pow(Up[1][0], 2) + pow(Up[2][0], 2)), Up[2][0] / sqrt(pow(Up[0][0], 2) + pow(Up[1][0], 2) + pow(Up[2][0], 2)), 0}, {Forward[0][0] / sqrt(pow(Forward[0][0], 2) + pow(Forward[1][0], 2) + pow(Forward[2][0], 2)), Forward[1][0] / sqrt(pow(Forward[0][0], 2) + pow(Forward[1][0], 2) + pow(Forward[2][0], 2)), Forward[2][0] / sqrt(pow(Forward[0][0], 2) + pow(Forward[1][0], 2) + pow(Forward[2][0], 2)), 0}, {0, 0, 0, 1} };
            float Eye[4][4] = { {1, 0, 0, -atof(ch[1].c_str())}, {0, 1, 0, -atof(ch[2].c_str())}, {0, 0, 1, -atof(ch[3].c_str())}, {0, 0, 0, 1} };
            eye_x = atof(ch[1].c_str());
            eye_y = atof(ch[2].c_str());
            eye_z = atof(ch[3].c_str());
            float GRM_Eye[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
            float Mirror_GRM_Eye[4][4] = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    Tilt_Mirror_GRM_Eye[i][j] = 0;
                }
            }

            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int in = 0; in < 4; in++) {
                        GRM_Eye[row][col] += GRM[row][in] * Eye[in][col];
                    }
                }
            }
            /*for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << GRM_Eye[i][j] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;*/
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int in = 0; in < 4; in++) {
                        Mirror_GRM_Eye[row][col] += Mirror[row][in] * GRM_Eye[in][col];
                    }
                }
            }
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    for (int in = 0; in < 4; in++) {
                        Tilt_Mirror_GRM_Eye[row][col] += Tilt[row][in] * Mirror_GRM_Eye[in][col];
                    }
                }
            }
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << Tilt_Mirror_GRM_Eye[i][j] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            
            perspective_projection[1][1] = window_width / window_height;
            perspective_projection[2][2] = (atof(ch[9].c_str()) / (atof(ch[9].c_str()) - atof(ch[8].c_str()))) * std::tan(atof(ch[10].c_str()) * 3.1415926 / 180);
            perspective_projection[2][3] = atof(ch[8].c_str()) * atof(ch[9].c_str()) / (atof(ch[8].c_str()) - atof(ch[9].c_str())) * std::tan(atof(ch[10].c_str()) * 3.1415926 / 180);
            perspective_projection[3][2] = std::tan(atof(ch[10].c_str()) * 3.1415926 / 180);

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << perspective_projection[i][j] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        else if (ch[0] == "viewport") {
            //std::cout << "hi" << std::endl;
            vxl = atof(ch[1].c_str());
            vxr = atof(ch[2].c_str());
            vyb = atof(ch[3].c_str());
            vyt = atof(ch[4].c_str());
            std::cout << vxl << " " << vxr << " " << vyb << " " << vyt << std::endl;
            float viewpoint[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
            float wind[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1 } };
            float S[3][3] = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
            for (int m = 0; m < 3; m++) {
                for (int n = 0; n < 3; n++) {
                    WMV[m][n] = 0;
                }
            }
            float tmp[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0 } };
            wind[0][2] = 1;
            wind[1][2] = 1;
            S[0][0] = (window_width / 2 + window_width / 2 * atof(ch[2].c_str()) - (window_width / 2 + window_width / 2 * atof(ch[1].c_str()))) / (1 - (-1));
            S[1][1] = (window_height / 2 + window_height / 2 * atof(ch[4].c_str()) - (window_height / 2 + window_height / 2 * atof(ch[3].c_str()))) / (1 - (-1));
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        tmp[row][col] += S[row][inner] * wind[inner][col];
                    }
                }
            }
            /*for (int s = 0; s < 3; s++) {
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
            std::cout << std::endl;*/
            viewpoint[0][2] = window_width / 2 + window_width / 2 * atof(ch[1].c_str());
            viewpoint[1][2] = window_height / 2 + window_height / 2 * atof(ch[3].c_str());
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        WMV[row][col] += viewpoint[row][inner] * tmp[inner][col];
                    }
                }
            }
            /*float lb_p[3][1] = {{std::atof(ch[1].c_str())}, {std::atof(ch[3].c_str())}, {1}};
            float rb_p[3][1] = { {std::atof(ch[2].c_str())}, {std::atof(ch[3].c_str())}, {1} };
            float rt_p[3][1] = { {std::atof(ch[2].c_str())}, {std::atof(ch[4].c_str())}, {1} };
            float lt_p[3][1] = { {std::atof(ch[1].c_str())}, {std::atof(ch[4].c_str())}, {1} };
            float lb_trans[3][1] = { {0}, {0}, {0} };
            float rb_trans[3][1] = { {0}, {0}, {0} };
            float rt_trans[3][1] = { {0}, {0}, {0} };
            float lt_trans[3][1] = { {0}, {0}, {0} };
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 1; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        lb_trans[row][col] += WMV[row][inner] * lb_p[inner][col];
                    }
                }
            }
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 1; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        rb_trans[row][col] += WMV[row][inner] * rb_p[inner][col];
                    }
                }
            }
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 1; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        rt_trans[row][col] += WMV[row][inner] * rt_p[inner][col];
                    }
                }
            }
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 1; col++) {
                    for (int inner = 0; inner < 3; inner++) {
                        lt_trans[row][col] += WMV[row][inner] * lt_p[inner][col];
                    }
                }
            }*/
            //std::cout << window_width / 2 + window_width / 2 * atof(ch[1].c_str()) << " " << window_width / 2 + window_width / 2 * atof(ch[2].c_str()) << std::endl;
            left = window_width / 2 + window_width / 2 * atof(ch[1].c_str());
            bottom = window_height / 2 + window_height / 2 * atof(ch[3].c_str());
            right = window_width / 2 + window_width / 2 * atof(ch[2].c_str());
            top = window_height / 2 + window_height / 2 * atof(ch[4].c_str());
            
            /*for (int s = 0; s < 3; s++) {
                for (int t = 0; t < 3; t++) {
                    std::cout << WMV[s][t] << " ";
                }
                std::cout << std::endl;
            }*/
        }
        else if (ch[0] == "light") {
            index = atoi(ch[1].c_str());
            if (index > count) {
                op.push_back(atoi(ch[2].c_str()));
                op.push_back(atoi(ch[3].c_str()));
                op.push_back(atoi(ch[4].c_str()));
                op.push_back(atoi(ch[5].c_str()));
                op.push_back(atoi(ch[6].c_str()));
                op.push_back(atoi(ch[7].c_str()));
                light.push_back(op);
                count++;
            }
            else {
                light[index - 1][0] = atoi(ch[2].c_str());
                light[index - 1][1] = atoi(ch[3].c_str());
                light[index - 1][2] = atoi(ch[4].c_str());
                light[index - 1][3] = atoi(ch[5].c_str());
                light[index - 1][4] = atoi(ch[6].c_str());
                light[index - 1][5] = atoi(ch[7].c_str());
            }
        }
        else if (ch[0] == "display") {
            //illumination
            std::vector<std::vector<float>> I;
            std::vector<float> I_tmp;
            for (int i = 0; i < obs_v_trans.size(); i++) {
                float x1 = obs_v_trans[i][1][0] - obs_v_trans[i][0][0];
                float y1 = obs_v_trans[i][1][1] - obs_v_trans[i][0][1];
                float z1 = obs_v_trans[i][1][2] - obs_v_trans[i][0][2];
                float x2 = obs_v_trans[i][2][0] - obs_v_trans[i][1][0];
                float y2 = obs_v_trans[i][2][1] - obs_v_trans[i][1][1];
                float z2 = obs_v_trans[i][2][2] - obs_v_trans[i][1][2]; 
                float Normal[3][1] = { {y1 * z2 - y2 * z1}, {z1 * x2 - z2 * x1}, {x1 * y2 - x2 * y1} };
                float unit_Normal[3][1] = { {Normal[0][0] / sqrt(pow(Normal[0][0], 2) + pow(Normal[1][0], 2) + pow(Normal[2][0], 2))}, {Normal[1][0] / sqrt(pow(Normal[0][0], 2) + pow(Normal[1][0], 2) + pow(Normal[2][0], 2))}, {Normal[2][0] / sqrt(pow(Normal[0][0], 2) + pow(Normal[1][0], 2) + pow(Normal[2][0], 2))} };
                float Idr = 0;
                float Idg = 0;
                float Idb = 0;
                float Isr = 0;
                float Isg = 0;
                float Isb = 0;
                for (int k = 0; k < light.size(); k++) { // 燈
                    float L[3][1] = { {light[k][3] - obs_v_trans[i][0][0]}, {light[k][4] - obs_v_trans[i][0][1]}, {light[k][5] - obs_v_trans[i][0][2]} };
                    float unit_L[3][1] = { {L[0][0] / sqrt(pow(L[0][0], 2) + pow(L[1][0], 2) + pow(L[2][0], 2))}, {L[1][0] / sqrt(pow(L[0][0], 2) + pow(L[1][0], 2) + pow(L[2][0], 2))}, {L[2][0] / sqrt(pow(L[0][0], 2) + pow(L[1][0], 2) + pow(L[2][0], 2))} };
                    float NL = unit_Normal[0][0] * unit_L[0][0] + unit_Normal[1][0] * unit_L[1][0] + unit_Normal[2][0] * unit_L[2][0];
                    if (NL < 0) {
                        NL = 0;
                    }
                    Idr += ob_color[i][3] * light[k][0] * NL * ob_color[i][0];
                    Idg += ob_color[i][3] * light[k][1] * NL * ob_color[i][1];
                    Idb += ob_color[i][3] * light[k][2] * NL * ob_color[i][2];
                    float V[3][1] = { {eye_x - obs_v_trans[i][0][0]}, {eye_y - obs_v_trans[i][0][1]}, {eye_z - obs_v_trans[i][0][2]} };
                    float unit_V[3][1] = { {V[0][0] / sqrt(pow(V[0][0], 2) + pow(V[1][0], 2) + pow(V[2][0], 2))}, {V[1][0] / sqrt(pow(V[0][0], 2) + pow(V[1][0], 2) + pow(V[2][0], 2))}, {V[2][0] / sqrt(pow(V[0][0], 2) + pow(V[1][0], 2) + pow(V[2][0], 2))}};
                    float LV[3][1] = { {unit_L[0][0] + unit_V[0][0]}, {unit_L[1][0] + unit_V[1][0]}, {unit_L[2][0] + unit_V[2][0]} };
                    float H[3][1] = { {(LV[0][0]) / sqrt(pow(LV[0][0], 2) + pow(LV[1][0], 2) + pow(LV[2][0], 2))}, {{(LV[1][0]) / sqrt(pow(LV[0][0], 2) + pow(LV[1][0], 2) + pow(LV[2][0], 2))}}, {{(LV[2][0]) / sqrt(pow(LV[0][0], 2) + pow(LV[1][0], 2) + pow(LV[2][0], 2))}} };
                    float unit_H[3][1] = { {H[0][0] / sqrt(pow(H[0][0], 2) + pow(H[1][0], 2) + pow(H[2][0], 2))}, {H[1][0] / sqrt(pow(H[0][0], 2) + pow(H[1][0], 2) + pow(H[2][0], 2))}, {H[2][0] / sqrt(pow(H[0][0], 2) + pow(H[1][0], 2) + pow(H[2][0], 2))}};
                    float HN = unit_H[0][0] * unit_Normal[0][0] + unit_H[1][0] * unit_Normal[1][0] + unit_H[2][0] * unit_Normal[2][0];
                    if (HN < 0) {
                        HN = 0;
                    }
                    Isr += ob_color[i][4] * light[k][0] * pow(HN, ob_color[i][5]);
                    Isg += ob_color[i][4] * light[k][1] * pow(HN, ob_color[i][5]);
                    Isb += ob_color[i][4] * light[k][2] * pow(HN, ob_color[i][5]);
                }
                Ir = KIr * ob_color[i][0] + Idr + Isr;
                Ig = KIg * ob_color[i][1] + Idg + Isg;
                Ib = KIb * ob_color[i][2] + Idb + Isb;
                if (Ir > 1.0) {
                    Ir = 1;
                }
                if (Ig > 1.0) {
                    Ig = 1;
                }
                if (Ib > 1.0) {
                    Ib = 1;
                }
                I_tmp.push_back(Ir);
                I_tmp.push_back(Ig);
                I_tmp.push_back(Ib);
                I.push_back(I_tmp);
                I_tmp.clear();
                
            }
            //observer
            float p[4][1] = { {1}, {1}, {1}, {1} };
            for (int m = 0; m < obs_v_trans.size(); m++) {
                for (int n = 0; n < obs_v_trans[m].size(); n++) {
                    float EM[4][1] = { {0}, {0}, {0}, {0} };
                    p[0][0] = obs_v_trans[m][n][0];
                    p[1][0] = obs_v_trans[m][n][1];
                    p[2][0] = obs_v_trans[m][n][2];
                    for (int row = 0; row < 4; row++) {
                        for (int col = 0; col < 1; col++) {
                            for (int in = 0; in < 4; in++) {
                                EM[row][col] += Tilt_Mirror_GRM_Eye[row][in] * p[in][col];
                            }
                        }
                    }
                    v_eye_space_co.push_back(EM[0][0]);
                    v_eye_space_co.push_back(EM[1][0]);
                    v_eye_space_co.push_back(EM[2][0]);
                    v_eye_space_tri.push_back(v_eye_space_co);
                    v_eye_space_co.clear();
                }
                v_eye_space_plane.push_back(v_eye_space_tri);
                v_eye_space_tri.clear();
            }
            for (int m = 0; m < v_eye_space_plane.size(); m++) {
                for (int n = 0; n < v_eye_space_plane[m].size(); n++) {
                    float PM[4][1] = { {0}, {0}, {0}, {0} };
                    p[0][0] = v_eye_space_plane[m][n][0];
                    p[1][0] = v_eye_space_plane[m][n][1];
                    p[2][0] = v_eye_space_plane[m][n][2];
                    for (int row = 0; row < 4; row++) {
                        for (int col = 0; col < 1; col++) {
                            for (int in = 0; in < 4; in++) {
                                PM[row][col] += perspective_projection[row][in] * p[in][col];
                            }
                        }
                    }
                    v_projection_space_co.push_back(PM[0][0]);
                    v_projection_space_co.push_back(PM[1][0]);
                    v_projection_space_co.push_back(PM[2][0]);
                    v_projection_space_co.push_back(PM[3][0]);
                    v_projection_space_tri.push_back(v_projection_space_co);
                    v_projection_space_co.clear();
                }
                v_projection_space_plane.push_back(v_projection_space_tri);
                v_projection_space_tri.clear();
            }
            v_eye_space_plane.clear();
            // w+x >= 0
            // w-x >= 0
            // w+y >= 0
            // w-y >= 0
            // z >= 0
            // w-z >= 0 
            for (int i = 0; i < v_projection_space_plane.size(); i++) {
                // w+x
                for (int j = 0; j < v_projection_space_plane[i].size(); j++) {
                    if (j == 0) {
                        continue;
                        //std::cout << "qwe" << std::endl;
                    }
                    if (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0] < 0 && v_projection_space_plane[i][j][3] + v_projection_space_plane[i][j][0] < 0) {
                        //std::cout << "rty" << std::endl;
                        //reject
                    }
                    else if (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0] >= 0 && v_projection_space_plane[i][j][3] + v_projection_space_plane[i][j][0] >= 0) {
                        //no clip
                        //std::cout << "yui" << std::endl;
                        co.push_back(v_projection_space_plane[i][j][0]);
                        co.push_back(v_projection_space_plane[i][j][1]);
                        co.push_back(v_projection_space_plane[i][j][2]);
                        co.push_back(v_projection_space_plane[i][j][3]);
                        clip_w_plus_x.push_back(co);
                        co.clear();
                    }
                    else if (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0] >= 0 && v_projection_space_plane[i][j][3] + v_projection_space_plane[i][j][0] < 0) {
                        //std::cout << "iop" << std::endl;
                        float t = (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0]) / (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0] - (v_projection_space_plane[i][j][3] + v_projection_space_plane[i][j][0]));
                        float x = v_projection_space_plane[i][j - 1][0] + t * (v_projection_space_plane[i][j][0] - v_projection_space_plane[i][j - 1][0]);
                        float y = v_projection_space_plane[i][j - 1][1] + t * (v_projection_space_plane[i][j][1] - v_projection_space_plane[i][j - 1][1]);
                        float z = v_projection_space_plane[i][j - 1][2] + t * (v_projection_space_plane[i][j][2] - v_projection_space_plane[i][j - 1][2]);
                        float w = v_projection_space_plane[i][j - 1][3] + t * (v_projection_space_plane[i][j][3] - v_projection_space_plane[i][j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_plus_x.push_back(co);
                        co.clear();
                    }
                    else if (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0] < 0 && v_projection_space_plane[i][j][3] + v_projection_space_plane[i][j][0] >= 0) {
                        //std::cout << "ppp" << std::endl;
                        float t = (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0]) / (v_projection_space_plane[i][j - 1][3] + v_projection_space_plane[i][j - 1][0] - (v_projection_space_plane[i][j][3] + v_projection_space_plane[i][j][0]));
                        float x = v_projection_space_plane[i][j - 1][0] + t * (v_projection_space_plane[i][j][0] - v_projection_space_plane[i][j - 1][0]);
                        float y = v_projection_space_plane[i][j - 1][1] + t * (v_projection_space_plane[i][j][1] - v_projection_space_plane[i][j - 1][1]);
                        float z = v_projection_space_plane[i][j - 1][2] + t * (v_projection_space_plane[i][j][2] - v_projection_space_plane[i][j - 1][2]);
                        float w = v_projection_space_plane[i][j - 1][3] + t * (v_projection_space_plane[i][j][3] - v_projection_space_plane[i][j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_plus_x.push_back(co);
                        co.clear();
                        co.push_back(v_projection_space_plane[i][j][0]);
                        co.push_back(v_projection_space_plane[i][j][1]);
                        co.push_back(v_projection_space_plane[i][j][2]);
                        co.push_back(v_projection_space_plane[i][j][3]);
                        /*for (int m = 0; m < co.size(); m++) {
                            std::cout << co[m] << " ";
                        }
                        std::cout << std::endl;*/
                        clip_w_plus_x.push_back(co);
                        co.clear();
                    }
                    /*for (int m = 0; m < clip_w_plus_x.size(); m++) {
                        for (int n = 0; n < clip_w_plus_x[m].size(); n++) {
                            std::cout << clip_w_plus_x[m][n] << " ";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;*/
                }
                /*std::cout << "11111" << " ";
                for (int m = 0; m < clip_w_plus_x.size(); m++) {
                    for (int n = 0; n < clip_w_plus_x[m].size(); n++) {
                        std::cout << clip_w_plus_x[m][n] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
                std::cout << "11111" << " ";*/
                // w+x last
                if (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] < 0 && v_projection_space_plane[i][0][3] + v_projection_space_plane[i][0][0] < 0) {
                    //std::cout << "asd" << std::endl;
                    //reject
                }
                else if (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] >= 0 && v_projection_space_plane[i][0][3] + v_projection_space_plane[i][0][0] >= 0) {
                    //no clip
                    //std::cout << "dfg" << std::endl;
                    co.push_back(v_projection_space_plane[i][0][0]);
                    co.push_back(v_projection_space_plane[i][0][1]);
                    co.push_back(v_projection_space_plane[i][0][2]);
                    co.push_back(v_projection_space_plane[i][0][3]);
                    clip_w_plus_x.push_back(co);
                    co.clear();
                }
                else if (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] >= 0 && v_projection_space_plane[i][0][3] + v_projection_space_plane[i][0][0] < 0) {
                    //std::cout << "ghj" << std::endl;
                    float t = (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0]) / (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] - (v_projection_space_plane[i][0][3] + v_projection_space_plane[i][0][0]));
                    float x = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] + t * (v_projection_space_plane[i][0][0] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0]);
                    float y = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][1] + t * (v_projection_space_plane[i][0][1] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][1]);
                    float z = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][2] + t * (v_projection_space_plane[i][0][2] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][2]);
                    float w = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + t * (v_projection_space_plane[i][0][3] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_plus_x.push_back(co);
                    co.clear();
                }
                else if (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] < 0 && v_projection_space_plane[i][0][3] + v_projection_space_plane[i][0][0] >= 0) {
                    //std::cout << "jkl" << std::endl;
                    float t = (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0]) / (v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] - (v_projection_space_plane[i][0][3] + v_projection_space_plane[i][0][0]));
                    float x = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0] + t * (v_projection_space_plane[i][0][0] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][0]);
                    float y = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][1] + t * (v_projection_space_plane[i][0][1] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][1]);
                    float z = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][2] + t * (v_projection_space_plane[i][0][2] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][2]);
                    float w = v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3] + t * (v_projection_space_plane[i][0][3] - v_projection_space_plane[i][v_projection_space_plane[i].size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_plus_x.push_back(co);
                    co.clear();
                    co.push_back(v_projection_space_plane[i][0][0]);
                    co.push_back(v_projection_space_plane[i][0][1]);
                    co.push_back(v_projection_space_plane[i][0][2]);
                    co.push_back(v_projection_space_plane[i][0][3]);
                    clip_w_plus_x.push_back(co);
                    co.clear();
                }

                if (clip_w_plus_x.size() == 0) {
                    continue;
                }
                //std::cout << "11111" << std::endl;
                /*for (int m = 0; m < clip_w_plus_x.size(); m++) {
                    for (int n = 0; n < clip_w_plus_x[m].size(); n++) {
                        std::cout << clip_w_plus_x[m][n] << " ";
                    }
                    std::cout << std::endl;
                }*/
                // w-x
                for (int j = 0; j < clip_w_plus_x.size(); j++) {
                    if (j == 0) {
                        continue;
                    }
                    if (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0] < 0 && clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0] < 0) {
                        //reject
                    }
                    else if (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0] >= 0 && clip_w_plus_x[j][3] - clip_w_plus_x[j][0] >= 0) {
                        //no clip
                        co.push_back(clip_w_plus_x[j][0]);
                        co.push_back(clip_w_plus_x[j][1]);
                        co.push_back(clip_w_plus_x[j][2]);
                        co.push_back(clip_w_plus_x[j][3]);
                        clip_w_minus_x.push_back(co);
                        co.clear();
                    }
                    else if (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0] >= 0 && clip_w_plus_x[j][3] - clip_w_plus_x[j][0] < 0) {
                        float t = (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0]) / (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0] - (clip_w_plus_x[j][3] - clip_w_plus_x[j][0]));
                        float x = clip_w_plus_x[j - 1][0] + t * (clip_w_plus_x[j][0] - clip_w_plus_x[j - 1][0]);
                        float y = clip_w_plus_x[j - 1][1] + t * (clip_w_plus_x[j][1] - clip_w_plus_x[j - 1][1]);
                        float z = clip_w_plus_x[j - 1][2] + t * (clip_w_plus_x[j][2] - clip_w_plus_x[j - 1][2]);
                        float w = clip_w_plus_x[j - 1][3] + t * (clip_w_plus_x[j][3] - clip_w_plus_x[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_minus_x.push_back(co);
                        co.clear();
                    }
                    else if (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0] < 0 && clip_w_plus_x[j][3] - clip_w_plus_x[j][0] >= 0) {
                        float t = (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0]) / (clip_w_plus_x[j - 1][3] - clip_w_plus_x[j - 1][0] - (clip_w_plus_x[j][3] - clip_w_plus_x[j][0]));
                        float x = clip_w_plus_x[j - 1][0] + t * (clip_w_plus_x[j][0] - clip_w_plus_x[j - 1][0]);
                        float y = clip_w_plus_x[j - 1][1] + t * (clip_w_plus_x[j][1] - clip_w_plus_x[j - 1][1]);
                        float z = clip_w_plus_x[j - 1][2] + t * (clip_w_plus_x[j][2] - clip_w_plus_x[j - 1][2]);
                        float w = clip_w_plus_x[j - 1][3] + t * (clip_w_plus_x[j][3] - clip_w_plus_x[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_minus_x.push_back(co);
                        co.clear();
                        co.push_back(clip_w_plus_x[j][0]);
                        co.push_back(clip_w_plus_x[j][1]);
                        co.push_back(clip_w_plus_x[j][2]);
                        co.push_back(clip_w_plus_x[j][3]);
                        clip_w_minus_x.push_back(co);
                        co.clear();
                    }
                }
                // w-x last
                if (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0] < 0 && clip_w_plus_x[0][3] - clip_w_plus_x[0][0] < 0) {
                    //reject
                }
                else if (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0] >= 0 && clip_w_plus_x[0][3] - clip_w_plus_x[0][0] >= 0) {
                    //no clip
                    co.push_back(clip_w_plus_x[0][0]);
                    co.push_back(clip_w_plus_x[0][1]);
                    co.push_back(clip_w_plus_x[0][2]);
                    co.push_back(clip_w_plus_x[0][3]);
                    clip_w_minus_x.push_back(co);
                    co.clear();
                }
                else if (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0] >= 0 && clip_w_plus_x[0][3] - clip_w_plus_x[0][0] < 0) {
                    float t = (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0]) / (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0] - (clip_w_plus_x[0][3] - clip_w_plus_x[0][0]));
                    float x = clip_w_plus_x[clip_w_plus_x.size() - 1][0] + t * (clip_w_plus_x[0][0] - clip_w_plus_x[clip_w_plus_x.size() - 1][0]);
                    float y = clip_w_plus_x[clip_w_plus_x.size() - 1][1] + t * (clip_w_plus_x[0][1] - clip_w_plus_x[clip_w_plus_x.size() - 1][1]);
                    float z = clip_w_plus_x[clip_w_plus_x.size() - 1][2] + t * (clip_w_plus_x[0][2] - clip_w_plus_x[clip_w_plus_x.size() - 1][2]);
                    float w = clip_w_plus_x[clip_w_plus_x.size() - 1][3] + t * (clip_w_plus_x[0][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_minus_x.push_back(co);
                    co.clear();
                }
                else if (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0] < 0 && clip_w_plus_x[0][3] - clip_w_plus_x[0][0] >= 0) {
                    float t = (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0]) / (clip_w_plus_x[clip_w_plus_x.size() - 1][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][0] - (clip_w_plus_x[0][3] - clip_w_plus_x[0][0]));
                    float x = clip_w_plus_x[clip_w_plus_x.size() - 1][0] + t * (clip_w_plus_x[0][0] - clip_w_plus_x[clip_w_plus_x.size() - 1][0]);
                    float y = clip_w_plus_x[clip_w_plus_x.size() - 1][1] + t * (clip_w_plus_x[0][1] - clip_w_plus_x[clip_w_plus_x.size() - 1][1]);
                    float z = clip_w_plus_x[clip_w_plus_x.size() - 1][2] + t * (clip_w_plus_x[0][2] - clip_w_plus_x[clip_w_plus_x.size() - 1][2]);
                    float w = clip_w_plus_x[clip_w_plus_x.size() - 1][3] + t * (clip_w_plus_x[0][3] - clip_w_plus_x[clip_w_plus_x.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_minus_x.push_back(co);
                    co.clear();
                    co.push_back(clip_w_plus_x[0][0]);
                    co.push_back(clip_w_plus_x[0][1]);
                    co.push_back(clip_w_plus_x[0][2]);
                    co.push_back(clip_w_plus_x[0][3]);
                    clip_w_minus_x.push_back(co);
                    co.clear();
                }
                clip_w_plus_x.clear();

                if (clip_w_minus_x.size() == 0) {
                    continue;
                }
                /*for (int m = 0; m < clip_w_minus_x.size(); m++) {
                    for (int n = 0; n < clip_w_minus_x[m].size(); n++) {
                        std::cout << clip_w_minus_x[m][n] << " ";
                    }
                    std::cout << std::endl;
                }*/
                //std::cout << "22222" << std::endl;
                // w+y
                for (int j = 0; j < clip_w_minus_x.size(); j++) {
                    if (j == 0) {
                        continue;
                    }
                    if (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1] < 0 && clip_w_minus_x[j][3] + clip_w_minus_x[j][1] < 0) {
                        //reject
                    }
                    else if (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1] >= 0 && clip_w_minus_x[j][3] + clip_w_minus_x[j][1] >= 0) {
                        //no clip
                        co.push_back(clip_w_minus_x[j][0]);
                        co.push_back(clip_w_minus_x[j][1]);
                        co.push_back(clip_w_minus_x[j][2]);
                        co.push_back(clip_w_minus_x[j][3]);
                        clip_w_plus_y.push_back(co);
                        co.clear();
                    }
                    else if (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1] >= 0 && clip_w_minus_x[j][3] + clip_w_minus_x[j][1] < 0) {
                        float t = (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1]) / (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1] - (clip_w_minus_x[j][3] + clip_w_minus_x[j][1]));
                        float x = clip_w_minus_x[j - 1][0] + t * (clip_w_minus_x[j][0] - clip_w_minus_x[j - 1][0]);
                        float y = clip_w_minus_x[j - 1][1] + t * (clip_w_minus_x[j][1] - clip_w_minus_x[j - 1][1]);
                        float z = clip_w_minus_x[j - 1][2] + t * (clip_w_minus_x[j][2] - clip_w_minus_x[j - 1][2]);
                        float w = clip_w_minus_x[j - 1][3] + t * (clip_w_minus_x[j][3] - clip_w_minus_x[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_plus_y.push_back(co);
                        co.clear();
                    }
                    else if (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1] < 0 && clip_w_minus_x[j][3] + clip_w_minus_x[j][1] >= 0) {
                        float t = (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1]) / (clip_w_minus_x[j - 1][3] + clip_w_minus_x[j - 1][1] - (clip_w_minus_x[j][3] + clip_w_minus_x[j][1]));
                        float x = clip_w_minus_x[j - 1][0] + t * (clip_w_minus_x[j][0] - clip_w_minus_x[j - 1][0]);
                        float y = clip_w_minus_x[j - 1][1] + t * (clip_w_minus_x[j][1] - clip_w_minus_x[j - 1][1]);
                        float z = clip_w_minus_x[j - 1][2] + t * (clip_w_minus_x[j][2] - clip_w_minus_x[j - 1][2]);
                        float w = clip_w_minus_x[j - 1][3] + t * (clip_w_minus_x[j][3] - clip_w_minus_x[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_plus_y.push_back(co);
                        co.clear();
                        co.push_back(clip_w_minus_x[j][0]);
                        co.push_back(clip_w_minus_x[j][1]);
                        co.push_back(clip_w_minus_x[j][2]);
                        co.push_back(clip_w_minus_x[j][3]);
                        clip_w_plus_y.push_back(co);
                        co.clear();
                    }
                }
                // w+y last
                if (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1] < 0 && clip_w_minus_x[0][3] + clip_w_minus_x[0][1] < 0) {
                    //reject
                }
                else if (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1] >= 0 && clip_w_minus_x[0][3] + clip_w_minus_x[0][1] >= 0) {
                    //no clip
                    co.push_back(clip_w_minus_x[0][0]);
                    co.push_back(clip_w_minus_x[0][1]);
                    co.push_back(clip_w_minus_x[0][2]);
                    co.push_back(clip_w_minus_x[0][3]);
                    clip_w_plus_y.push_back(co);
                    co.clear();
                }
                else if (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1] >= 0 && clip_w_minus_x[0][3] + clip_w_minus_x[0][1] < 0) {
                    float t = (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1]) / (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1] - (clip_w_minus_x[0][3] + clip_w_minus_x[0][1]));
                    float x = clip_w_minus_x[clip_w_minus_x.size() - 1][0] + t * (clip_w_minus_x[0][0] - clip_w_minus_x[clip_w_minus_x.size() - 1][0]);
                    float y = clip_w_minus_x[clip_w_minus_x.size() - 1][1] + t * (clip_w_minus_x[0][1] - clip_w_minus_x[clip_w_minus_x.size() - 1][1]);
                    float z = clip_w_minus_x[clip_w_minus_x.size() - 1][2] + t * (clip_w_minus_x[0][2] - clip_w_minus_x[clip_w_minus_x.size() - 1][2]);
                    float w = clip_w_minus_x[clip_w_minus_x.size() - 1][3] + t * (clip_w_minus_x[0][3] - clip_w_minus_x[clip_w_minus_x.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_plus_y.push_back(co);
                    co.clear();
                }
                else if (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1] < 0 && clip_w_minus_x[0][3] + clip_w_minus_x[0][1] >= 0) {
                    float t = (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1]) / (clip_w_minus_x[clip_w_minus_x.size() - 1][3] + clip_w_minus_x[clip_w_minus_x.size() - 1][1] - (clip_w_minus_x[0][3] + clip_w_minus_x[0][1]));
                    float x = clip_w_minus_x[clip_w_minus_x.size() - 1][0] + t * (clip_w_minus_x[0][0] - clip_w_minus_x[clip_w_minus_x.size() - 1][0]);
                    float y = clip_w_minus_x[clip_w_minus_x.size() - 1][1] + t * (clip_w_minus_x[0][1] - clip_w_minus_x[clip_w_minus_x.size() - 1][1]);
                    float z = clip_w_minus_x[clip_w_minus_x.size() - 1][2] + t * (clip_w_minus_x[0][2] - clip_w_minus_x[clip_w_minus_x.size() - 1][2]);
                    float w = clip_w_minus_x[clip_w_minus_x.size() - 1][3] + t * (clip_w_minus_x[0][3] - clip_w_minus_x[clip_w_minus_x.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_plus_y.push_back(co);
                    co.clear();
                    co.push_back(clip_w_minus_x[0][0]);
                    co.push_back(clip_w_minus_x[0][1]);
                    co.push_back(clip_w_minus_x[0][2]);
                    co.push_back(clip_w_minus_x[0][3]);
                    clip_w_plus_y.push_back(co);
                    co.clear();
                }
                //std::cout << clip_w_plus_y.size() << std::endl;
                clip_w_minus_x.clear();
                if (clip_w_plus_y.size() == 0) {
                    continue;
                }
                /*for (int p = 0; p < clip_w_plus_y.size(); p++) {
                    for (int q = 0; q < clip_w_plus_y[p].size(); q++) {
                        std::cout << clip_w_plus_y[p][q] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;*/
                //std::cout << "33333" << std::endl;
                // w-y
                for (int j = 0; j < clip_w_plus_y.size(); j++) {
                    if (j == 0) {
                        continue;
                    }
                    if (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1] < 0 && clip_w_plus_y[j][3] - clip_w_plus_y[j][1] < 0) {
                        //reject
                    }
                    else if (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1] >= 0 && clip_w_plus_y[j][3] - clip_w_plus_y[j][1] >= 0) {
                        //no clip
                        co.push_back(clip_w_plus_y[j][0]);
                        co.push_back(clip_w_plus_y[j][1]);
                        co.push_back(clip_w_plus_y[j][2]);
                        co.push_back(clip_w_plus_y[j][3]);
                        clip_w_minus_y.push_back(co);
                        co.clear();
                        /*for (int p = 0; p < clip_w_minus_y.size(); p++) {
                            for (int q = 0; q < clip_w_minus_y[p].size(); q++) {
                                std::cout << clip_w_minus_y[p][q] << " ";
                            }
                            std::cout << std::endl;
                        }
                        std::cout << std::endl;*/
                    }
                    else if (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1] >= 0 && clip_w_plus_y[j][3] - clip_w_plus_y[j][1] < 0) {
                        float t = (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1]) / (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1] - (clip_w_plus_y[j][3] - clip_w_plus_y[j][1]));
                        float x = clip_w_plus_y[j - 1][0] + t * (clip_w_plus_y[j][0] - clip_w_plus_y[j - 1][0]);
                        float y = clip_w_plus_y[j - 1][1] + t * (clip_w_plus_y[j][1] - clip_w_plus_y[j - 1][1]);
                        float z = clip_w_plus_y[j - 1][2] + t * (clip_w_plus_y[j][2] - clip_w_plus_y[j - 1][2]);
                        float w = clip_w_plus_y[j - 1][3] + t * (clip_w_plus_y[j][3] - clip_w_plus_y[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_minus_y.push_back(co);
                        co.clear();

                    }
                    else if (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1] < 0 && clip_w_plus_y[j][3] - clip_w_plus_y[j][1] >= 0) {
                        float t = (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1]) / (clip_w_plus_y[j - 1][3] - clip_w_plus_y[j - 1][1] - (clip_w_plus_y[j][3] - clip_w_plus_y[j][1]));
                        float x = clip_w_plus_y[j - 1][0] + t * (clip_w_plus_y[j][0] - clip_w_plus_y[j - 1][0]);
                        float y = clip_w_plus_y[j - 1][1] + t * (clip_w_plus_y[j][1] - clip_w_plus_y[j - 1][1]);
                        float z = clip_w_plus_y[j - 1][2] + t * (clip_w_plus_y[j][2] - clip_w_plus_y[j - 1][2]);
                        float w = clip_w_plus_y[j - 1][3] + t * (clip_w_plus_y[j][3] - clip_w_plus_y[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_minus_y.push_back(co);
                        co.clear();
                        co.push_back(clip_w_plus_y[j][0]);
                        co.push_back(clip_w_plus_y[j][1]);
                        co.push_back(clip_w_plus_y[j][2]);
                        co.push_back(clip_w_plus_y[j][3]);
                        clip_w_minus_y.push_back(co);
                        co.clear();
                    }
                }
                /*for (int p = 0; p < clip_w_minus_y.size(); p++) {
                    for (int q = 0; q < clip_w_minus_y[p].size(); q++) {
                        std::cout << clip_w_minus_y[p][q] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;*/
                // w-y last
                if (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1] < 0 && clip_w_plus_y[0][3] - clip_w_plus_y[0][1] < 0) {
                    //reject
                }
                else if (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1] >= 0 && clip_w_plus_y[0][3] - clip_w_plus_y[0][1] >= 0) {
                    //no clip
                    co.push_back(clip_w_plus_y[0][0]);
                    co.push_back(clip_w_plus_y[0][1]);
                    co.push_back(clip_w_plus_y[0][2]);
                    co.push_back(clip_w_plus_y[0][3]);
                    clip_w_minus_y.push_back(co);
                    co.clear();
                }
                else if (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1] >= 0 && clip_w_plus_y[0][3] - clip_w_plus_y[0][1] < 0) {
                    float t = (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1]) / (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1] - (clip_w_plus_y[0][3] - clip_w_plus_y[0][1]));
                    float x = clip_w_plus_y[clip_w_plus_y.size() - 1][0] + t * (clip_w_plus_y[0][0] - clip_w_plus_y[clip_w_plus_y.size() - 1][0]);
                    float y = clip_w_plus_y[clip_w_plus_y.size() - 1][1] + t * (clip_w_plus_y[0][1] - clip_w_plus_y[clip_w_plus_y.size() - 1][1]);
                    float z = clip_w_plus_y[clip_w_plus_y.size() - 1][2] + t * (clip_w_plus_y[0][2] - clip_w_plus_y[clip_w_plus_y.size() - 1][2]);
                    float w = clip_w_plus_y[clip_w_plus_y.size() - 1][3] + t * (clip_w_plus_y[0][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_minus_y.push_back(co);
                    co.clear();
                }
                else if (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1] < 0 && clip_w_plus_y[0][3] - clip_w_plus_y[0][1] >= 0) {
                    float t = (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1]) / (clip_w_plus_y[clip_w_plus_y.size() - 1][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][1] - (clip_w_plus_y[0][3] - clip_w_plus_y[0][1]));
                    //std::cout << t << std::endl;
                    float x = clip_w_plus_y[clip_w_plus_y.size() - 1][0] + t * (clip_w_plus_y[0][0] - clip_w_plus_y[clip_w_plus_y.size() - 1][0]);
                    //std::cout << x << std::endl;
                    float y = clip_w_plus_y[clip_w_plus_y.size() - 1][1] + t * (clip_w_plus_y[0][1] - clip_w_plus_y[clip_w_plus_y.size() - 1][1]);
                    //std::cout << y << std::endl;
                    float z = clip_w_plus_y[clip_w_plus_y.size() - 1][2] + t * (clip_w_plus_y[0][2] - clip_w_plus_y[clip_w_plus_y.size() - 1][2]);
                    //std::cout << z << std::endl;
                    float w = clip_w_plus_y[clip_w_plus_y.size() - 1][3] + t * (clip_w_plus_y[0][3] - clip_w_plus_y[clip_w_plus_y.size() - 1][3]);
                    //std::cout << w << std::endl;
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_minus_y.push_back(co);
                    co.clear();
                    co.push_back(clip_w_plus_y[0][0]);
                    co.push_back(clip_w_plus_y[0][1]);
                    co.push_back(clip_w_plus_y[0][2]);
                    co.push_back(clip_w_plus_y[0][3]);
                    clip_w_minus_y.push_back(co);
                    co.clear();
                    /*for (int p = 0; p < clip_w_minus_y.size(); p++) {
                        for (int q = 0; q < clip_w_minus_y[p].size(); q++) {
                            std::cout << clip_w_minus_y[p][q] << " ";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;*/
                }


                clip_w_plus_y.clear();
                if (clip_w_minus_y.size() == 0) {
                    continue;
                }
                //std::cout << "44444" << std::endl;
                // z
                for (int j = 0; j < clip_w_minus_y.size(); j++) {
                    if (j == 0) {
                        continue;
                    }
                    if (clip_w_minus_y[j - 1][2] < 0 && clip_w_minus_y[j][2] < 0) {
                        //reject
                    }
                    else if (clip_w_minus_y[j - 1][2] >= 0 && clip_w_minus_y[j][2] >= 0) {
                        //no clip
                        co.push_back(clip_w_minus_y[j][0]);
                        co.push_back(clip_w_minus_y[j][1]);
                        co.push_back(clip_w_minus_y[j][2]);
                        co.push_back(clip_w_minus_y[j][3]);
                        clip_z.push_back(co);
                        co.clear();
                    }
                    else if (clip_w_minus_y[j - 1][2] >= 0 && clip_w_minus_y[j][2] < 0) {
                        float t = clip_w_minus_y[j - 1][2] / (clip_w_minus_y[j - 1][2] - clip_w_minus_y[j][2]);
                        float x = clip_w_minus_y[j - 1][0] + t * (clip_w_minus_y[j][0] - clip_w_minus_y[j - 1][0]);
                        float y = clip_w_minus_y[j - 1][1] + t * (clip_w_minus_y[j][1] - clip_w_minus_y[j - 1][1]);
                        float z = clip_w_minus_y[j - 1][2] + t * (clip_w_minus_y[j][2] - clip_w_minus_y[j - 1][2]);
                        float w = clip_w_minus_y[j - 1][3] + t * (clip_w_minus_y[j][3] - clip_w_minus_y[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_z.push_back(co);
                        co.clear();
                    }
                    else if (clip_w_minus_y[j - 1][2] < 0 && clip_w_minus_y[j][2] >= 0) {
                        float t = clip_w_minus_y[j - 1][2] / (clip_w_minus_y[j - 1][2] - clip_w_minus_y[j][2]);
                        float x = clip_w_minus_y[j - 1][0] + t * (clip_w_minus_y[j][0] - clip_w_minus_y[j - 1][0]);
                        float y = clip_w_minus_y[j - 1][1] + t * (clip_w_minus_y[j][1] - clip_w_minus_y[j - 1][1]);
                        float z = clip_w_minus_y[j - 1][2] + t * (clip_w_minus_y[j][2] - clip_w_minus_y[j - 1][2]);
                        float w = clip_w_minus_y[j - 1][3] + t * (clip_w_minus_y[j][3] - clip_w_minus_y[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_z.push_back(co);
                        co.clear();
                        co.push_back(clip_w_minus_y[j][0]);
                        co.push_back(clip_w_minus_y[j][1]);
                        co.push_back(clip_w_minus_y[j][2]);
                        co.push_back(clip_w_minus_y[j][3]);
                        clip_z.push_back(co);
                        co.clear();
                    }
                }
                // z last
                if (clip_w_minus_y[clip_w_minus_y.size() - 1][2] < 0 && clip_w_minus_y[0][2] < 0) {
                    //reject
                }
                else if (clip_w_minus_y[clip_w_minus_y.size() - 1][2] >= 0 && clip_w_minus_y[0][2] >= 0) {
                    //no clip
                    co.push_back(clip_w_minus_y[0][0]);
                    co.push_back(clip_w_minus_y[0][1]);
                    co.push_back(clip_w_minus_y[0][2]);
                    co.push_back(clip_w_minus_y[0][3]);
                    clip_z.push_back(co);
                    co.clear();
                }
                else if (clip_w_minus_y[clip_w_minus_y.size() - 1][2] >= 0 && clip_w_minus_y[0][2] < 0) {
                    float t = clip_w_minus_y[clip_w_minus_y.size() - 1][2] / (clip_w_minus_y[clip_w_minus_y.size() - 1][2] - clip_w_minus_y[0][2]);
                    float x = clip_w_minus_y[clip_w_minus_y.size() - 1][0] + t * (clip_w_minus_y[0][0] - clip_w_minus_y[clip_w_minus_y.size() - 1][0]);
                    float y = clip_w_minus_y[clip_w_minus_y.size() - 1][1] + t * (clip_w_minus_y[0][1] - clip_w_minus_y[clip_w_minus_y.size() - 1][1]);
                    float z = clip_w_minus_y[clip_w_minus_y.size() - 1][2] + t * (clip_w_minus_y[0][2] - clip_w_minus_y[clip_w_minus_y.size() - 1][2]);
                    float w = clip_w_minus_y[clip_w_minus_y.size() - 1][3] + t * (clip_w_minus_y[0][3] - clip_w_minus_y[clip_w_minus_y.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_z.push_back(co);
                    co.clear();
                }
                else if (clip_w_minus_y[clip_w_minus_y.size() - 1][2] < 0 && clip_w_minus_y[0][2] >= 0) {
                    float t = clip_w_minus_y[clip_w_minus_y.size() - 1][2] / (clip_w_minus_y[clip_w_minus_y.size() - 1][2] - clip_w_minus_y[0][2]);
                    float x = clip_w_minus_y[clip_w_minus_y.size() - 1][0] + t * (clip_w_minus_y[0][0] - clip_w_minus_y[clip_w_minus_y.size() - 1][0]);
                    float y = clip_w_minus_y[clip_w_minus_y.size() - 1][1] + t * (clip_w_minus_y[0][1] - clip_w_minus_y[clip_w_minus_y.size() - 1][1]);
                    float z = clip_w_minus_y[clip_w_minus_y.size() - 1][2] + t * (clip_w_minus_y[0][2] - clip_w_minus_y[clip_w_minus_y.size() - 1][2]);
                    float w = clip_w_minus_y[clip_w_minus_y.size() - 1][3] + t * (clip_w_minus_y[0][3] - clip_w_minus_y[clip_w_minus_y.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_z.push_back(co);
                    co.clear();
                    co.push_back(clip_w_minus_y[0][0]);
                    co.push_back(clip_w_minus_y[0][1]);
                    co.push_back(clip_w_minus_y[0][2]);
                    co.push_back(clip_w_minus_y[0][3]);
                    clip_z.push_back(co);
                    co.clear();
                }

                clip_w_minus_y.clear();
                if (clip_z.size() == 0) {
                    continue;
                }
                //std::cout << "55555" << std::endl;
                // w-z
                for (int j = 0; j < clip_z.size(); j++) {
                    if (j == 0) {
                        continue;
                    }
                    if (clip_z[j - 1][3] - clip_z[j - 1][2] < 0 && clip_z[j][3] - clip_z[j][2] < 0) {
                        //reject
                    }
                    else if (clip_z[j - 1][3] - clip_z[j - 1][2] >= 0 && clip_z[j][3] - clip_z[j][2] >= 0) {
                        //no clip
                        co.push_back(clip_z[j][0]);
                        co.push_back(clip_z[j][1]);
                        co.push_back(clip_z[j][2]);
                        co.push_back(clip_z[j][3]);
                        clip_w_minus_z.push_back(co);
                        co.clear();
                    }
                    else if (clip_z[j - 1][3] - clip_z[j - 1][2] >= 0 && clip_z[j][3] - clip_z[j][2] < 0) {
                        float t = clip_z[j - 1][3] - clip_z[j - 1][2] / (clip_z[j - 1][3] - clip_z[j - 1][2] - (clip_z[j][3] - clip_z[j][2]));
                        float x = clip_z[j - 1][0] + t * (clip_z[j][0] - clip_z[j - 1][0]);
                        float y = clip_z[j - 1][1] + t * (clip_z[j][1] - clip_z[j - 1][1]);
                        float z = clip_z[j - 1][2] + t * (clip_z[j][2] - clip_z[j - 1][2]);
                        float w = clip_z[j - 1][3] + t * (clip_z[j][3] - clip_z[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_minus_z.push_back(co);
                        co.clear();
                    }
                    else if (clip_z[j - 1][3] - clip_z[j - 1][2] < 0 && clip_z[j][3] - clip_z[j][2] >= 0) {
                        float t = clip_z[j - 1][3] - clip_z[j - 1][2] / (clip_z[j - 1][3] - clip_z[j - 1][2] - (clip_z[j][3] - clip_z[j][2]));
                        float x = clip_z[j - 1][0] + t * (clip_z[j][0] - clip_z[j - 1][0]);
                        float y = clip_z[j - 1][1] + t * (clip_z[j][1] - clip_z[j - 1][1]);
                        float z = clip_z[j - 1][2] + t * (clip_z[j][2] - clip_z[j - 1][2]);
                        float w = clip_z[j - 1][3] + t * (clip_z[j][3] - clip_z[j - 1][3]);
                        co.push_back(x);
                        co.push_back(y);
                        co.push_back(z);
                        co.push_back(w);
                        clip_w_minus_z.push_back(co);
                        co.clear();
                        co.push_back(clip_z[j][0]);
                        co.push_back(clip_z[j][1]);
                        co.push_back(clip_z[j][2]);
                        co.push_back(clip_z[j][3]);
                        clip_w_minus_z.push_back(co);
                        co.clear();
                    }
                }
                // w-z last
                if (clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] < 0 && clip_z[0][3] - clip_z[0][2] < 0) {
                    //reject
                }
                else if (clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] >= 0 && clip_z[0][3] - clip_z[0][2] >= 0) {
                    //no clip
                    co.push_back(clip_z[0][0]);
                    co.push_back(clip_z[0][1]);
                    co.push_back(clip_z[0][2]);
                    co.push_back(clip_z[0][3]);
                    clip_w_minus_z.push_back(co);
                    co.clear();
                }
                else if (clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] >= 0 && clip_z[0][3] - clip_z[0][2] < 0) {
                    float t = clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] / (clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] - (clip_z[0][3] - clip_z[0][2]));
                    float x = clip_z[clip_z.size() - 1][0] + t * (clip_z[0][0] - clip_z[clip_z.size() - 1][0]);
                    float y = clip_z[clip_z.size() - 1][1] + t * (clip_z[0][1] - clip_z[clip_z.size() - 1][1]);
                    float z = clip_z[clip_z.size() - 1][2] + t * (clip_z[0][2] - clip_z[clip_z.size() - 1][2]);
                    float w = clip_z[clip_z.size() - 1][3] + t * (clip_z[0][3] - clip_z[clip_z.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_minus_z.push_back(co);
                    co.clear();
                }
                else if (clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] < 0 && clip_z[0][3] - clip_z[0][2] >= 0) {
                    float t = clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] / (clip_z[clip_z.size() - 1][3] - clip_z[clip_z.size() - 1][2] - (clip_z[0][3] - clip_z[0][2]));
                    float x = clip_z[clip_z.size() - 1][0] + t * (clip_z[0][0] - clip_z[clip_z.size() - 1][0]);
                    float y = clip_z[clip_z.size() - 1][1] + t * (clip_z[0][1] - clip_z[clip_z.size() - 1][1]);
                    float z = clip_z[clip_z.size() - 1][2] + t * (clip_z[0][2] - clip_z[clip_z.size() - 1][2]);
                    float w = clip_z[clip_z.size() - 1][3] + t * (clip_z[0][3] - clip_z[clip_z.size() - 1][3]);
                    co.push_back(x);
                    co.push_back(y);
                    co.push_back(z);
                    co.push_back(w);
                    clip_w_minus_z.push_back(co);
                    co.clear();
                    co.push_back(clip_z[0][0]);
                    co.push_back(clip_z[0][1]);
                    co.push_back(clip_z[0][2]);
                    co.push_back(clip_z[0][3]);
                    clip_w_minus_z.push_back(co);
                    co.clear();
                }
                clip_z.clear();
                if (clip_w_minus_z.size() == 0) {
                    continue;
                }
                //std::cout << "66666" << std::endl;
                // perspective divide
                for (int per = 0; per < clip_w_minus_z.size(); per++) {
                    for (int div = 0; div < 3; div++) {
                        clip_w_minus_z[per][div] = clip_w_minus_z[per][div] / clip_w_minus_z[per][3];
                    }
                }
                clip.push_back(clip_w_minus_z);
                //std::cout << clip_w_minus_z.size() << std::endl;
                clip_w_minus_z.clear();
                //std::cout << "hi" << std::endl;
            }
            //std::cout << clip.size() << " ";
            v_projection_space_plane.clear();
            /*std::vector<std::vector<std::vector<float>>> object;
            std::vector<std::vector<float>> object_data;
            std::vector<float> object_data_tmp;
            for (int p = 0; p < clip.size(); p++) {
                for (int q = 0; q < clip[p].size(); q++) {
                    Point objects(clip[p][q][0], clip[p][q][1]);
                    object_data.push_back(objects);
                    object_data_z.push_back(clip[p][q][2]);
                }
                object.push_back(object_data);
                object_z.push_back(object_data_z);
                object_data.clear();
                object_data_z.clear();
            }
            clip.clear();*/
            for (int i = 0; i < clip.size(); i++) {
                for (int j = 0; j < clip[i].size(); j++) {
                    //std::cout << object[i][j].getX() << " " << object[i][j].getY() << std::endl;
                    if (j == 0) {
                        polygon_first[0][0] = clip[i][0][0];
                        polygon_first[1][0] = clip[i][0][1];
                        polygon_first[2][0] = clip[i][0][2];
                        polygon_last[0][0] = clip[i][0][0];
                        polygon_last[1][0] = clip[i][0][1];
                        polygon_last[2][0] = clip[i][0][2];
                        continue;
                    }
                    //left
                    if (clip[i][j][0] > -1 && clip[i][j - 1][0] > -1) {
                        //std::cout << object[i][j].getX() << " " << object[i][j].getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                        polygon_last[0][0] = clip[i][j][0];
                        polygon_last[1][0] = clip[i][j][1];
                        polygon_last[2][0] = clip[i][j][2];
                        object_data_trans_left_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_left_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_left_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_left.push_back(object_data_trans_left_tmp);
                        object_data_trans_left_tmp.clear();
                    }
                    else if (clip[i][j][0] <= -1 && clip[i][j - 1][0] > -1) {
                        float m = (float)(clip[i][j][1] - clip[i][j - 1][1]) / (float)(clip[i][j][0] - clip[i][j - 1][0]);
                        float b = clip[i][j][1] - (m * clip[i][j][0]);
                        float y = m * (-1) + b;
                        float z = (y - clip[i][j - 1][1]) / (clip[i][j][1] - clip[i][j - 1][1]) * (clip[i][j][2] - clip[i][j - 1][2]) + clip[i][j - 1][2];
                        polygon_last[0][0] = -1;
                        polygon_last[1][0] = y;
                        polygon_last[2][0] = z;
                        object_data_trans_left_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_left_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_left_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_left.push_back(object_data_trans_left_tmp);
                        object_data_trans_left_tmp.clear();
                    }
                    else if (clip[i][j][0] <= -1 && clip[i][j - 1][0] <= -1) {
                        polygon_last[0][0] = clip[i][j][0];
                        polygon_last[1][0] = clip[i][j][1];
                        polygon_last[2][0] = clip[i][j][2];
                    }
                    else if (clip[i][j][0] > -1 && clip[i][j - 1][0] <= -1) {
                        float m = (float)(clip[i][j][1] - clip[i][j - 1][1]) / (float)(clip[i][j][0] - clip[i][j - 1][0]);
                        float b = clip[i][j][1] - (m * clip[i][j][0]);
                        float y = m * (-1) + b;
                        float z = (y - clip[i][j - 1][1]) / (clip[i][j][1] - clip[i][j - 1][1]) * (clip[i][j][2] - clip[i][j - 1][2]) + clip[i][j - 1][2];
                        polygon_last[0][0] = -1;
                        polygon_last[1][0] = y;
                        polygon_last[2][0] = z;
                        object_data_trans_left_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_left_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_left_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_left.push_back(object_data_trans_left_tmp);
                        object_data_trans_left_tmp.clear();
                        polygon_last[0][0] = clip[i][j][0];
                        polygon_last[1][0] = clip[i][j][1];
                        polygon_last[2][0] = clip[i][j][2];
                        object_data_trans_left.push_back(object_data_trans_left_tmp);
                        object_data_trans_left_tmp.clear();
                    }
                }
                //last_to_first_left
                if (polygon_first[0][0] > -1 && polygon_last[0][0] > -1) {
                    //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    object_data_trans_left_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_left.push_back(object_data_trans_left_tmp);
                    object_data_trans_left_tmp.clear();
                }
                else if (polygon_first[0][0] <= -1 && polygon_last[0][0] > -1) {
                    float m = (float)(polygon_first[1][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                    float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                    float y = m * (-1) + b;
                    float z = (y - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                    polygon_last[0][0] = -1;
                    polygon_last[1][0] = y;
                    polygon_last[2][0] = z;
                    object_data_trans_left_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_left.push_back(object_data_trans_left_tmp);
                    object_data_trans_left_tmp.clear();
                }
                else if (polygon_first[0][0] <= -1 && polygon_last[0][0] <= -1) {
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                }
                else if (polygon_first[0][0] > -1 && polygon_last[0][0] <= -1) {
                    float m = (float)(polygon_first[1][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                    float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                    float y = m * (-1) + b;
                    float z = (y - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                    polygon_last[0][0] = -1;
                    polygon_last[1][0] = y;
                    polygon_last[2][0] = z;
                    object_data_trans_left_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_left.push_back(object_data_trans_left_tmp);
                    object_data_trans_left_tmp.clear();
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    object_data_trans_left_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_left_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_left.push_back(object_data_trans_left_tmp);
                    object_data_trans_left_tmp.clear();
                }
                /*for (int v = 0; v < object_data_trans_left.size(); v++) {
                    std::cout << object_data_trans_left[v].getX() << " " << object_data_trans_left[v].getY() << " " << "left" << std::endl;
                }
                std::cout << std::endl;*/

                for (int j = 0; j < object_data_trans_left.size(); j++) {
                    if (j == 0) {
                        polygon_first[0][0] = object_data_trans_left[0][0];
                        polygon_first[1][0] = object_data_trans_left[0][1];
                        polygon_first[2][0] = object_data_trans_left[0][2];
                        polygon_last[0][0] = object_data_trans_left[0][0];
                        polygon_last[1][0] = object_data_trans_left[0][1];
                        polygon_last[2][0] = object_data_trans_left[0][2];
                        continue;
                    }
                    //bottom
                    if (object_data_trans_left[j][1] > -1 && object_data_trans_left[j - 1][1] > -1) {
                        //std::cout << object_data_trans_left[j].getX() << " " << object_data_trans_left[j].getY() << " " << std::atof(ch[3].c_str()) << "hi" << std::endl;
                        polygon_last[0][0] = object_data_trans_left[j][0];
                        polygon_last[1][0] = object_data_trans_left[j][1];
                        polygon_last[2][0] = object_data_trans_left[j][2];
                        object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                        object_data_trans_bottom_tmp.clear();
                    }
                    else if (object_data_trans_left[j][1] <= -1 && object_data_trans_left[j - 1][1] > -1) {
                        if (object_data_trans_left[j][0] == object_data_trans_left[j - 1][0]) {
                            float z = (-1 - object_data_trans_left[j - 1][1]) / (object_data_trans_left[j][1] - object_data_trans_left[j - 1][1]) * (object_data_trans_left[j][2] - object_data_trans_left[j - 1][2]) + object_data_trans_left[j - 1][2];
                            polygon_last[0][0] = object_data_trans_left[j][0];
                            polygon_last[1][0] = -1;
                            polygon_last[2][0] = z;
                            object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                            object_data_trans_bottom_tmp.clear();
                        }
                        else {
                            float m = (float)(object_data_trans_left[j][1] - object_data_trans_left[j - 1][1]) / (float)(object_data_trans_left[j][0] - object_data_trans_left[j - 1][0]);
                            float b = object_data_trans_left[j][1] - (m * object_data_trans_left[j][0]);
                            float x = (-1 - b) / m;
                            float z = (-1 - object_data_trans_left[j - 1][1]) / (object_data_trans_left[j][1] - object_data_trans_left[j - 1][1]) * (object_data_trans_left[j][2] - object_data_trans_left[j - 1][2]) + object_data_trans_left[j - 1][2];
                            polygon_last[0][0] = x;
                            polygon_last[1][0] = -1;
                            polygon_last[2][0] = z;
                            object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                            object_data_trans_bottom_tmp.clear();
                        }
                    }
                    else if (object_data_trans_left[j][1] <= -1 && object_data_trans_left[j - 1][1] <= -1) {
                        polygon_last[0][0] = object_data_trans_left[j][0];
                        polygon_last[1][0] = object_data_trans_left[j][1];
                        polygon_last[2][0] = object_data_trans_left[j][2];
                    }
                    else if (object_data_trans_left[j][1] > -1 && object_data_trans_left[j - 1][1] <= -1) {
                        if (object_data_trans_left[j][0] == object_data_trans_left[j - 1][0]) {
                            float z = (-1 - object_data_trans_left[j - 1][1]) / (object_data_trans_left[j][1] - object_data_trans_left[j - 1][1]) * (object_data_trans_left[j][2] - object_data_trans_left[j - 1][2]) + object_data_trans_left[j - 1][2];
                            polygon_last[0][0] = object_data_trans_left[j][0];
                            polygon_last[1][0] = -1;
                            polygon_last[2][0] = z;
                            object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                            object_data_trans_bottom_tmp.clear();
                            polygon_last[0][0] = object_data_trans_left[j][0];
                            polygon_last[1][0] = object_data_trans_left[j][1];
                            polygon_last[2][0] = object_data_trans_left[j][2];
                            object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                            object_data_trans_bottom_tmp.clear();
                        }
                        else {
                            float m = (float)(object_data_trans_left[j][1] - object_data_trans_left[j - 1][1]) / (float)(object_data_trans_left[j][0] - object_data_trans_left[j - 1][0]);
                            float b = object_data_trans_left[j][1] - (m * object_data_trans_left[j][0]);
                            float x = (-1 - b) / m;
                            float z = (-1 - object_data_trans_left[j - 1][1]) / (object_data_trans_left[j][1] - object_data_trans_left[j - 1][1]) * (object_data_trans_left[j][2] - object_data_trans_left[j - 1][2]) + object_data_trans_left[j - 1][2];
                            polygon_last[0][0] = x;
                            polygon_last[1][0] = -1;
                            polygon_last[2][0] = z;
                            object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                            object_data_trans_bottom_tmp.clear();
                            polygon_last[0][0] = object_data_trans_left[j][0];
                            polygon_last[1][0] = object_data_trans_left[j][1];
                            polygon_last[2][0] = object_data_trans_left[j][2];
                            object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                            object_data_trans_bottom_tmp.clear();
                        }
                    }
                }
                object_data_trans_left.clear();
                //last_to_first_bottom
                if (polygon_first[1][0] > -1 && polygon_last[1][0] > -1) {
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                    object_data_trans_bottom_tmp.clear();
                    //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                }
                else if (polygon_first[1][0] <= -1 && polygon_last[1][0] > -1) {
                    if (polygon_first[0][0] == polygon_last[0][0]) {
                        float z = (-1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = -1;
                        polygon_last[2][0] = z;
                        object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                        object_data_trans_bottom_tmp.clear();
                    }
                    else {
                        float m = (float)(polygon_first[1][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                        float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                        float x = (-1 - b) / m;
                        float z = (-1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = x;
                        polygon_last[1][0] = -1;
                        polygon_last[2][0] = z;
                        object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                        object_data_trans_bottom_tmp.clear();
                        //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                    }
                }
                else if (polygon_first[1][0] <= -1 && polygon_last[1][0] <= -1) {
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                }
                else if (polygon_first[1][0] > -1 && polygon_last[1][0] <= -1) {
                    if (polygon_first[0][0] == polygon_last[0][0]) {
                        float z = (-1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = -1;
                        polygon_last[2][0] = z;
                        object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                        object_data_trans_bottom_tmp.clear();
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = polygon_first[1][0];
                        polygon_last[2][0] = polygon_first[2][0];
                        object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                        object_data_trans_bottom_tmp.clear();
                    }
                    else {
                        float m = (float)(polygon_first[1][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                        float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                        float x = (-1 - b) / m;
                        float z = (-1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = x;
                        polygon_last[1][0] = -1;
                        polygon_last[2][0] = z;
                        object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                        object_data_trans_bottom_tmp.clear();
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = polygon_first[1][0];
                        polygon_last[2][0] = polygon_first[2][0];
                        object_data_trans_bottom_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_bottom_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_bottom.push_back(object_data_trans_bottom_tmp);
                        object_data_trans_bottom_tmp.clear();
                        //std::cout << polygon_last.getX() << " " << polygon_last.getY() << " " << std::atof(ch[1].c_str()) << "hi" << std::endl;
                    }
                }
                /*for (int v = 0; v < object_data_trans_bottom.size(); v++) {
                    std::cout << object_data_trans_bottom[v].getX() << " " << object_data_trans_bottom[v].getY() << " " << "bottom" << std::endl;
                }
                std::cout << std::endl;*/

                for (int j = 0; j < object_data_trans_bottom.size(); j++) {
                    if (j == 0) {
                        //std::cout << "1" << std::endl;
                        polygon_first[0][0] = object_data_trans_bottom[0][0];
                        polygon_first[1][0] = object_data_trans_bottom[0][1];
                        polygon_first[2][0] = object_data_trans_bottom[0][2];
                        polygon_last[0][0] = object_data_trans_bottom[0][0];
                        polygon_last[1][0] = object_data_trans_bottom[0][1];
                        polygon_last[2][0] = object_data_trans_bottom[0][2];
                        /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "1" << std::endl;
                        }*/
                        continue;
                    }
                    //right
                    if (object_data_trans_bottom[j][0] < 1 && object_data_trans_bottom[j - 1][0] < 1) {
                        //std::cout << "2" << std::endl;
                        polygon_last[0][0] = object_data_trans_bottom[j][0];
                        polygon_last[1][0] = object_data_trans_bottom[j][1];
                        polygon_last[2][0] = object_data_trans_bottom[j][2];
                        object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_right.push_back(object_data_trans_right_tmp);
                        object_data_trans_right_tmp.clear();
                    }
                    else if (object_data_trans_bottom[j][0] >= 1 && object_data_trans_bottom[j - 1][0] < 1) {
                        //std::cout << "3" << std::endl;
                        float m = (float)(object_data_trans_bottom[j][1] - object_data_trans_bottom[j - 1][1]) / (float)(object_data_trans_bottom[j][0] - object_data_trans_bottom[j - 1][0]);
                        float b = object_data_trans_bottom[j][1] - (m * object_data_trans_bottom[j][0]);
                        float y = m * 1 + b;
                        float z = (y - object_data_trans_bottom[j - 1][1]) / (object_data_trans_bottom[j][1] - object_data_trans_bottom[j - 1][1]) * (object_data_trans_bottom[j][2] - object_data_trans_bottom[j - 1][2]) + object_data_trans_bottom[j - 1][2];
                        polygon_last[0][0] = 1;
                        polygon_last[1][0] = y;
                        polygon_last[2][0] = z;
                        object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_right.push_back(object_data_trans_right_tmp);
                        object_data_trans_right_tmp.clear();
                        /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "3" << std::endl;
                        }*/
                    }
                    else if (object_data_trans_bottom[j][0] >= 1 && object_data_trans_bottom[j - 1][0] >= 1) {
                        //std::cout << "4" << std::endl;
                        polygon_last[0][0] = object_data_trans_bottom[j][0];
                        polygon_last[1][0] = object_data_trans_bottom[j][1];
                        polygon_last[2][0] = object_data_trans_bottom[j][2];
                        /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "4" << std::endl;
                        }*/
                    }
                    else if (object_data_trans_bottom[j][0] < 1 && object_data_trans_bottom[j - 1][0] >= 1) {
                        //std::cout << "5" << std::endl;
                        float m = (float)(object_data_trans_bottom[j][1] - object_data_trans_bottom[j - 1][1]) / (float)(object_data_trans_bottom[j][0] - object_data_trans_bottom[j - 1][0]);
                        float b = object_data_trans_bottom[j][1] - (m * object_data_trans_bottom[j][0]);
                        float y = m * 1 + b;
                        float z = (y - object_data_trans_bottom[j - 1][1]) / (object_data_trans_bottom[j][1] - object_data_trans_bottom[j - 1][1]) * (object_data_trans_bottom[j][2] - object_data_trans_bottom[j - 1][2]) + object_data_trans_bottom[j - 1][2];
                        //std::cout << m << " " << b << " " << y << std::endl;
                        polygon_last[0][0] = 1;
                        polygon_last[1][0] = y;
                        polygon_last[2][0] = z;
                        object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_right.push_back(object_data_trans_right_tmp);
                        object_data_trans_right_tmp.clear();
                        polygon_last[0][0] = object_data_trans_bottom[j][0];
                        polygon_last[1][0] = object_data_trans_bottom[j][1];
                        polygon_last[2][0] = object_data_trans_bottom[j][2];
                        object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_right.push_back(object_data_trans_right_tmp);
                        object_data_trans_right_tmp.clear();
                        /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                            std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "5" << std::endl;
                        }*/
                    }
                }
                object_data_trans_bottom.clear();
                //last_to_first_right
                //std::cout << object_data_trans_right.size() << std::endl;
                //std::cout << polygon_first.getX() << " " << std::atof(ch[2].c_str()) << " " << polygon_last.getX() << std::endl;
                if (polygon_first[0][0] < 1 && polygon_last[0][0] < 1) {
                    //std::cout << "6" << std::endl;
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_right.push_back(object_data_trans_right_tmp);
                    object_data_trans_right_tmp.clear();
                    /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                        std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << "hi+++++++++++++++++" << std::endl;
                    }*/
                }
                else if (polygon_first[0][0] >= 1 && polygon_last[0][0] < 1) {
                    //std::cout << "7" << std::endl;
                    float m = (float)(polygon_first[0][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                    float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                    float y = m * 1 + b;
                    float z = (y - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                    polygon_last[0][0] = 1;
                    polygon_last[1][0] = y;
                    polygon_last[2][0] = z;
                    object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_right.push_back(object_data_trans_right_tmp);
                    object_data_trans_right_tmp.clear();
                }
                else if (polygon_first[0][0] >= 1 && polygon_last[0][0] >= 1) {
                    //std::cout << "8" << std::endl;
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                        std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "8" << std::endl;
                    }*/
                }
                else if (polygon_first[0][0] < 1 && polygon_last[0][0] >= 1) {
                    //std::cout << "9" << std::endl;
                    float m = (float)(polygon_first[1][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                    float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                    float y = m * 1 + b;
                    float z = (y - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                    polygon_last[0][0] = 1;
                    polygon_last[1][0] = y;
                    polygon_last[2][0] = z;
                    object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_right.push_back(object_data_trans_right_tmp);
                    object_data_trans_right_tmp.clear();
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    object_data_trans_right_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_right_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_right.push_back(object_data_trans_right_tmp);
                    object_data_trans_right_tmp.clear();
                }
                //std::cout << object_data_trans_right.size() << std::endl;
                /*for (int v = 0; v < object_data_trans_right.size(); v++) {
                    std::cout << object_data_trans_right[v].getX() << " " << object_data_trans_right[v].getY() << " " << "right" << std::endl;
                }*/
                /*std::cout << std::endl;
                std::cout << object_data_trans_right.size() << std::endl;*/

                for (int j = 0; j < object_data_trans_right.size(); j++) {
                    //std::cout << "hi" << std:: endl;
                    if (j == 0) {
                        polygon_first[0][0] = object_data_trans_right[0][0];
                        polygon_first[1][0] = object_data_trans_right[0][1];
                        polygon_first[2][0] = object_data_trans_right[0][2];
                        polygon_last[0][0] = object_data_trans_right[0][0];
                        polygon_last[1][0] = object_data_trans_right[0][1];
                        polygon_last[2][0] = object_data_trans_right[0][2];
                        //std::cout << "hi2" << std::endl;
                        continue;
                    }
                    //top
                    if (object_data_trans_right[j][1] <= 1 && object_data_trans_right[j - 1][1] <= 1) {
                        polygon_last[0][0] = object_data_trans_right[j][0];
                        polygon_last[1][0] = object_data_trans_right[j][1];
                        polygon_last[2][0] = object_data_trans_right[j][2];
                        object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_top.push_back(object_data_trans_top_tmp);
                        object_data_trans_top_tmp.clear();
                    }
                    else if (object_data_trans_right[j][1] > 1 && object_data_trans_right[j - 1][1] <= 1) {
                        if (object_data_trans_right[j][0] == object_data_trans_right[j - 1][0]) {
                            float z = (-1 - object_data_trans_right[j - 1][1]) / (object_data_trans_right[j][1] - object_data_trans_right[j - 1][1]) * (object_data_trans_right[j][2] - object_data_trans_right[j - 1][2]) + object_data_trans_right[j - 1][2];
                            polygon_last[0][0] = object_data_trans_right[j][0];
                            polygon_last[1][0] = 1;
                            polygon_last[2][0] = z;
                            object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_top.push_back(object_data_trans_top_tmp);
                            object_data_trans_top_tmp.clear();
                        }
                        else {
                            float m = (float)(object_data_trans_right[j][1] - object_data_trans_right[j - 1][1]) / (float)(object_data_trans_right[j][0] - object_data_trans_right[j - 1][0]);
                            float b = object_data_trans_right[j][1] - (m * object_data_trans_right[j][0]);
                            float x = (1 - b) / m;
                            float z = (1 - object_data_trans_right[j - 1][1]) / (object_data_trans_right[j][1] - object_data_trans_right[j - 1][1]) * (object_data_trans_right[j][2] - object_data_trans_right[j - 1][2]) + object_data_trans_right[j - 1][2];
                            polygon_last[0][0] = x;
                            polygon_last[1][0] = 1;
                            polygon_last[2][0] = z;
                            object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_top.push_back(object_data_trans_top_tmp);
                            object_data_trans_top_tmp.clear();
                        }
                    }
                    else if (object_data_trans_right[j][1] > 1 && object_data_trans_right[j - 1][1] > 1) {
                        //std::cout << "456789" << std::endl;
                        polygon_last[0][0] = object_data_trans_right[j][0];
                        polygon_last[1][0] = object_data_trans_right[j][1];
                        polygon_last[2][0] = object_data_trans_right[j][2];
                    }
                    else if (object_data_trans_right[j][1] <= 1 && object_data_trans_right[j - 1][1] > 1) {
                        if (object_data_trans_right[j][0] == object_data_trans_right[j - 1][0]) {
                            float z = (1 - object_data_trans_right[j - 1][1]) / (object_data_trans_right[j][1] - object_data_trans_right[j - 1][1]) * (object_data_trans_right[j][2] - object_data_trans_right[j - 1][2]) + object_data_trans_right[j - 1][2];
                            polygon_last[0][0] = object_data_trans_right[j][0];
                            polygon_last[1][0] = 1;
                            polygon_last[2][0] = z;
                            object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_top.push_back(object_data_trans_top_tmp);
                            object_data_trans_top_tmp.clear();
                            polygon_last[0][0] = object_data_trans_right[j][0];
                            polygon_last[1][0] = object_data_trans_right[j][1];
                            polygon_last[2][0] = object_data_trans_right[j][2];
                            object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_top.push_back(object_data_trans_top_tmp);
                            object_data_trans_top_tmp.clear();
                        }
                        else {
                            float m = (float)(object_data_trans_right[j][1] - object_data_trans_right[j - 1][1]) / (float)(object_data_trans_right[j][0] - object_data_trans_right[j - 1][0]);
                            float b = object_data_trans_right[j][1] - (m * object_data_trans_right[j][0]);
                            float x = (1 - b) / m;
                            float z = (1 - object_data_trans_right[j - 1][1]) / (object_data_trans_right[j][1] - object_data_trans_right[j - 1][1]) * (object_data_trans_right[j][2] - object_data_trans_right[j - 1][2]) + object_data_trans_right[j - 1][2];
                            polygon_last[0][0] = x;
                            polygon_last[1][0] = 1;
                            polygon_last[2][0] = z;
                            object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_top.push_back(object_data_trans_top_tmp);
                            object_data_trans_top_tmp.clear();
                            polygon_last[0][0] = object_data_trans_right[j][0];
                            polygon_last[1][0] = object_data_trans_right[j][1];
                            polygon_last[2][0] = object_data_trans_right[j][2];
                            object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                            object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                            object_data_trans_top.push_back(object_data_trans_top_tmp);
                            object_data_trans_top_tmp.clear();
                        }
                    }
                }
                object_data_trans_right.clear();
                //last_to_first_top
                if (polygon_first[1][0] <= 1 && polygon_last[1][0] <= 1) {
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                    object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                    object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                    object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                    object_data_trans_top.push_back(object_data_trans_top_tmp);
                    object_data_trans_top_tmp.clear();
                }
                else if (polygon_first[1][0] > 1 && polygon_last[1][0] <= 1) {
                    if (polygon_first[0][0] == polygon_last[0][0]) {
                        float z = (1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = 1;
                        polygon_last[2][0] = z;
                        object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_top.push_back(object_data_trans_top_tmp);
                        object_data_trans_top_tmp.clear();
                    }
                    else {
                        float m = (float)(polygon_first[1][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                        float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                        float x = (1 - b) / m;
                        float z = (1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = x;
                        polygon_last[1][0] = 1;
                        polygon_last[2][0] = z;
                        object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_top.push_back(object_data_trans_top_tmp);
                        object_data_trans_top_tmp.clear();
                    }
                }
                else if (polygon_first[1][0] > 1 && polygon_last[1][0] > 1) {
                    polygon_last[0][0] = polygon_first[0][0];
                    polygon_last[1][0] = polygon_first[1][0];
                    polygon_last[2][0] = polygon_first[2][0];
                }
                else if (polygon_first[1][0] <= 1 && polygon_last[1][0] > 1) {
                    if (polygon_first[0][0] == polygon_last[0][0]) {
                        float z = (1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = 1;
                        polygon_last[2][0] = z;
                        object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_top.push_back(object_data_trans_top_tmp);
                        object_data_trans_top_tmp.clear();
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = polygon_first[1][0];
                        polygon_last[2][0] = polygon_first[2][0];
                        object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_top.push_back(object_data_trans_top_tmp);
                        object_data_trans_top_tmp.clear();
                    }
                    else {
                        float m = (float)(polygon_first[1][0] - polygon_last[1][0]) / (float)(polygon_first[0][0] - polygon_last[0][0]);
                        float b = polygon_first[1][0] - (m * polygon_first[0][0]);
                        float x = (1 - b) / m;
                        float z = (1 - polygon_last[1][0]) / (polygon_first[1][0] - polygon_last[1][0]) * (polygon_first[2][0] - polygon_last[2][0]) + polygon_last[2][0];
                        polygon_last[0][0] = x;
                        polygon_last[1][0] = 1;
                        polygon_last[2][0] = z;
                        object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_top.push_back(object_data_trans_top_tmp);
                        object_data_trans_top_tmp.clear();
                        polygon_last[0][0] = polygon_first[0][0];
                        polygon_last[1][0] = polygon_first[1][0];
                        polygon_last[2][0] = polygon_first[2][0];
                        object_data_trans_top_tmp.push_back(polygon_last[0][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[1][0]);
                        object_data_trans_top_tmp.push_back(polygon_last[2][0]);
                        object_data_trans_top.push_back(object_data_trans_top_tmp);
                        object_data_trans_top_tmp.clear();
                    }
                }
                object_image_space.push_back(object_data_trans_top);
                object_data_trans_top.clear();
            }
            clip.clear();
           
            Point lb(left, bottom);
            Point rb(right, bottom);
            Point rt(right, top);
            Point lt(left, top);
            RGB.setRGB(1, 1, 1);
            Line(lb, rb).drawLine(RGB);
            Line(rb, rt).drawLine(RGB);
            Line(rt, lt).drawLine(RGB);
            Line(lt, lb).drawLine(RGB);
            Point obj(0, 0);
            
            std::vector<std::vector<float>> z_buffer;
            std::vector<float> z_buffer_tmp;
            std::vector<std::vector<Color>> c_buffer;
            std::vector<Color> c_buffer_tmp;
            int top = window_height / 2 + window_height / 2 * vyt;
            int bottom = window_height / 2 + window_height / 2 * vyb;
            int left = window_height / 2 + window_height / 2 * vxl;
            int right = window_height / 2 + window_height / 2 * vxr;
            std::wcout << "vyt: " << top << std::endl;
            std::wcout << "vyb: " << bottom << std::endl;
            std::wcout << "vxl: " << left << std::endl;
            std::wcout << "vxr: " << right << std::endl;
            std::cout << vxl << " " << vxr << " " << vyb << " " << vyt << std::endl;
            std::wcout << window_height << " " << window_width << std::endl;

            for (int i = 0; i < window_height; i++) {
                for (int j = 0; j < window_width; j++) {
                    z_buffer_tmp.push_back(2);
                    RGB.setRGB(0, 0, 0);
                    c_buffer_tmp.push_back(RGB);
                    /*if ((i > top || i < bottom) && (j > right || j < left)) {
                        RGB.setRGB(0, 0, 0);
                        c_buffer_tmp.push_back(RGB);
                    }*/
                    if (i <= top && i >= bottom && j <= right && j >= left) {
                        RGB.setRGB(Br, Bg, Bb);
                        c_buffer_tmp.push_back(RGB);
                    }
                }
                z_buffer.push_back(z_buffer_tmp);
                c_buffer.push_back(c_buffer_tmp);
            }
            for (int i = 0; i < window_height; i++) {
                for (int j = 0; j < window_width; j++) {

                    if (i <= top && i >= bottom && j <= right && j >= left) {
                        RGB.setRGB(Br, Bg, Bb);
                        c_buffer[i][j] = RGB;
                    }
                }
            }

            
            /*for (int i = 0; i < object_image_space.size(); i++) {
                for (int j = 0; j < object_image_space[i].size(); j++) {
                    obj.setXY(object_image_space[i][j].getX(), object_image_space[i][j].getY());
                    object_data.push_back(obj);
                }
                object_screen_space.push_back(object_data);
                object_data.clear();
            }*/
            //glReadPixels(0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
            //glDrawPixels(window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
            //std::cout << clip.size() << std::endl;
            /*for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    std::cout << WMV[i][j] << " ";
                }
                std::cout << std::endl;
            }*/
            //std::cout << "size : " << object_data_trans_top.size() << std::endl;
            /*for (int m = 0; m < 4; m++) {
                for (int n = 0; n < 4; n++) {
                    std::cout << WMV[m][n] << " ";
                }
                std::cout << std::endl;
            }*/
            //viewport
            for (int i = 0; i < object_image_space.size(); i++) {
                for (int j = 0; j < object_image_space[i].size(); j++) { //triangle
                    //std::cout << object_image_space[i].size() << std::endl;
                    float point[3][1] = { {1}, {1}, {1} };
                    float point_trans[3][1] = { {0}, {0}, {0} };
                    //std::cout << object_data_trans_top[k].getX() << " " << object_data_trans_top[k].getY() << std::endl;
                    point[0][0] = object_image_space[i][j][0];
                    point[1][0] = object_image_space[i][j][1];
                    /*for (int s = 0; s < 3; s++) {
                        for (int t = 0; t < 1; t++) {
                            std::cout << point[s][t];
                        }
                        std::cout << std::endl;
                    }*/
                    for (int row = 0; row < 3; row++) {
                        for (int col = 0; col < 1; col++) {
                            for (int inner = 0; inner < 3; inner++) {
                                point_trans[row][col] += WMV[row][inner] * point[inner][col];
                            }
                        }
                    }
                    object_image_space[i][j][0] = int(std::floor(point_trans[0][0] + 0.5));
                    object_image_space[i][j][1] = int(std::floor(point_trans[1][0] + 0.5));
                }
            }
            /*for (int i = 0; i < object_image_space.size(); i++) {
                for (int j = 0; j < object_image_space[i].size(); j++) {
                    std::cout << object_image_space[i][j][0] << " " << object_image_space[i][j][1] << " " << object_image_space[i][j][2] << " |||| ";
                }
                std::cout << std::endl;
            }*/
            //shading
            z_value.clear();
            float x = 0;
            float y = 0;
            float z = 0;
            float first_point[3][1] = {{-1}, {-1}, {-1}};
            float last_point[3][1] = { {-1}, {-1}, {-1} };;
            
            
            for (int i = 0; i < object_image_space.size(); i++) {
                for (int k = 0; k < window_height; k++) {
                    y = k;
                    int po = 0;
                    for (int j = 0; j < object_image_space[i].size(); j++) { //triangle
                        if (j == 0) {
                            continue;
                        }
                        if ((object_image_space[i][j][1] >= y && object_image_space[i][j - 1][1] <= y) || (object_image_space[i][j][1] <= y && object_image_space[i][j - 1][1] >= y)) {
                            //std::cout << y << std::endl;
                            x = (y - object_image_space[i][j][1]) / (object_image_space[i][j - 1][1] - object_image_space[i][j][1]) * (object_image_space[i][j - 1][0] - object_image_space[i][j][0]) + object_image_space[i][j][0];
                            z = (y - object_image_space[i][j][1]) / (object_image_space[i][j - 1][1] - object_image_space[i][j][1]) * (object_image_space[i][j - 1][2] - object_image_space[i][j][2]) + object_image_space[i][j][2];
                            //std::cout << x << std::endl;
                            if (object_image_space[i][j][1] == object_image_space[i][j - 1][1]) {
                                if (object_image_space[i][j][0] < object_image_space[i][j - 1][0]) {
                                    
                                    for (int t = object_image_space[i][j][0]; t <= object_image_space[i][j - 1][0]; t++) {
                                        z = (t - object_image_space[i][j - 1][0]) / (object_image_space[i][j][0] - object_image_space[i][j - 1][0]) * (object_image_space[i][j][2] - object_image_space[i][j - 1][2]) + object_image_space[i][j - 1][2];
                                        z_value_co.push_back(t);
                                        z_value_co.push_back(y);
                                        z_value_co.push_back(z);
                                        z_value_tmp.push_back(z_value_co);
                                        z_value_co.clear();
                                    }
                                }
                                else {
                                    for (int t = object_image_space[i][j - 1][0]; t <= object_image_space[i][j][0]; t++) {
                                        z = (t - object_image_space[i][j - 1][0]) / (object_image_space[i][j][0] - object_image_space[i][j - 1][0]) * (object_image_space[i][j][2] - object_image_space[i][j - 1][2]) + object_image_space[i][j - 1][2];
                                        z_value_co.push_back(t);
                                        z_value_co.push_back(y);
                                        z_value_co.push_back(z);
                                        z_value_tmp.push_back(z_value_co);
                                        z_value_co.clear();
                                    }
                                }
                                continue;
                            }
                            po++;
                            if (po == 1) {
                                first_point[0][0] = x;
                                first_point[1][0] = y;
                                first_point[2][0] = z;
                            }
                            else if (po == 2) {
                                last_point[0][0] = x;
                                last_point[1][0] = y;
                                last_point[2][0] = z;
                            }
                        }
                        //std::cout << "hi1" << std::endl;
                    }
                    if ((object_image_space[i][0][1] >= y && object_image_space[i][object_image_space[i].size() - 1][1] <= y) || (object_image_space[i][0][1] <= y && object_image_space[i][object_image_space[i].size() - 1][1] >= y)) {
                        x = (y - object_image_space[i][0][1]) / (object_image_space[i][object_image_space[i].size() - 1][1] - object_image_space[i][0][1]) * (object_image_space[i][object_image_space[i].size() - 1][0] - object_image_space[i][0][0]) + object_image_space[i][0][0];
                        z = (y - object_image_space[i][0][1]) / (object_image_space[i][object_image_space[i].size() - 1][1] - object_image_space[i][0][1]) * (object_image_space[i][object_image_space[i].size() - 1][2] - object_image_space[i][0][2]) + object_image_space[i][0][2];
                        if (object_image_space[i][0][1] == object_image_space[i][object_image_space[i].size() - 1][1]) {
                            if (object_image_space[i][0][0] < object_image_space[i][object_image_space[i].size() - 1][0]) {
                                for (int t = object_image_space[i][0][0]; t <= object_image_space[i][object_image_space[i].size() - 1][0]; t++) {
                                    z = (t - object_image_space[i][object_image_space[i].size() - 1][0]) / (object_image_space[i][0][0] - object_image_space[i][object_image_space[i].size() - 1][0]) * (object_image_space[i][0][2] - object_image_space[i][object_image_space[i].size() - 1][2]) + object_image_space[i][object_image_space[i].size() - 1][2];
                                    z_value_co.push_back(t);
                                    z_value_co.push_back(y);
                                    z_value_co.push_back(z);
                                    z_value_tmp.push_back(z_value_co);
                                    z_value_co.clear();
                                }
                            }
                            else {
                                for (int t = object_image_space[i][object_image_space[i].size() - 1][0]; t <= object_image_space[i][0][0]; t++) {
                                    z = (t - object_image_space[i][object_image_space[i].size() - 1][0]) / (object_image_space[i][0][0] - object_image_space[i][object_image_space[i].size() - 1][0]) * (object_image_space[i][0][2] - object_image_space[i][object_image_space[i].size() - 1][2]) + object_image_space[i][object_image_space[i].size() - 1][2];
                                    z_value_co.push_back(t);
                                    z_value_co.push_back(y);
                                    z_value_co.push_back(z);
                                    z_value_tmp.push_back(z_value_co);
                                    z_value_co.clear();
                                }
                            }
                            continue;
                        }
                        po++;
                        if (po == 1) {
                            first_point[0][0] = x;
                            first_point[1][0] = y;
                            first_point[2][0] = z;
                        }
                        else if (po == 2) {
                            last_point[0][0] = x;
                            last_point[1][0] = y;
                            last_point[2][0] = z;
                        }
                    }
                    if (po == 0) {
                        continue;
                    }
                    //std::cout << object_image_space[i][0][1] << std::endl;
                    //std::cout << object_image_space[i][object_image_space[i].size() - 1][1] << std::endl;
                    
                    //std::cout << "hi2" << std::endl;
                    if (first_point[0][0] < last_point[0][0]) {
                        for (int s = first_point[0][0]; s <= last_point[0][0]; s++) {
                            z = (s - first_point[0][0]) / (last_point[0][0] - first_point[0][0]) * (last_point[2][0] - first_point[2][0]) + first_point[2][0];
                            //std::cout << z << std::endl;
                            z_value_co.push_back(s);
                            z_value_co.push_back(y);
                            z_value_co.push_back(z);
                            z_value_tmp.push_back(z_value_co);
                            z_value_co.clear();
                        }
                    }
                    else if ((first_point[0][0] == last_point[0][0]) && first_point[0][0] != -1) {
                        z_value_co.push_back(first_point[0][0]);
                        z_value_co.push_back(y);
                        z_value_co.push_back(z);
                        z_value_tmp.push_back(z_value_co);
                        //std::cout << z_value_co[0] << " " << z_value_co[1] << " " << z_value_co[2] << std::endl;
                        z_value_co.clear();
                    }
                    else {
                        for (int s = last_point[0][0]; s <= first_point[0][0]; s++) {
                            z = (s - last_point[0][0]) / (first_point[0][0] - last_point[0][0]) * (first_point[2][0] - last_point[2][0]) + last_point[2][0];
                            z_value_co.push_back(s);
                            z_value_co.push_back(y);
                            z_value_co.push_back(z);
                            z_value_tmp.push_back(z_value_co);
                            z_value_co.clear();
                        }
                    }
                    //std::cout << "hi3" << std::endl;
                    
                }
                z_value.push_back(z_value_tmp);
                z_value_tmp.clear();
            }
            /*for (int t = 0; t < z_value[0].size(); t++) {
                std::cout << z_value[0][t][0] << " " << z_value[0][t][1] << " " << z_value[0][t][2] << std::endl;
            }
            
            std::cout << z_value.size() << std::endl;
            for (int m = 0; m < I.size(); m++) {
                std::cout << I[m][0] << " " << I[m][1] << " " << I[m][2] << std::endl;
            }
            std::cout << I.size() << std::endl;
            std::cout << z_buffer.size() << std::endl;
            std::cout << z_buffer[0].size() << std::endl;*/
            for (int i = 0; i < z_value.size(); i++) {
                for (int j = 0; j < z_value[i].size(); j++) { //triangle
                    if (z_value[i][j][2] < z_buffer[z_value[i][j][1]][z_value[i][j][0]]) {
                        z_buffer[int(std::floor(z_value[i][j][1] + 0.5))][int(std::floor(z_value[i][j][0] + 0.5))] = z_value[i][j][2];
                        c_buffer[int(std::floor(z_value[i][j][1] + 0.5))][int(std::floor(z_value[i][j][0] + 0.5))].setRGB(I[i][0], I[i][1], I[i][2]);
                    }
                    //std::cout << z_value[i][j][2] << std::endl;
                }
            }
            z_value.clear();
            for (int i = 1; i < window_height; i++) {
                for (int j = 1; j < window_width; j++) {
                    Point draw(j, i);
                    draw.drawPoint(c_buffer[i][j]);
                }
            }
            I.clear();
            object_image_space.clear();
            glFlush();
            system("pause");
            clear();
            glFlush();
            //object_screen_space.clear();
            clip.clear();
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
    glReadPixels(0, 0, window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
}


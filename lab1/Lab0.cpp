#include <GL/glut.h>
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <iostream>

int window;
void display();
void mouse(int bin, int state, int x, int y);
void keyDown(unsigned char key, int x, int y);
void mouseMove(int x, int y);
void clear();
char key = 0;
bool dot = FALSE;

class Color {
public:
    float r;
    float g;
    float b;
    Color() {this->Ran();}
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
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    int getX();
    int getY();
    void drawPoint(Color);
    void setXY(int, int);
};

void Point::setXY(int a, int b) {
    x = a;
    y = b;
}
int Point::getX() { return x; }
int Point::getY() { return y; }
void Point::drawPoint(Color rgb) {
    glPointSize(3);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(rgb.getR(), rgb.getG(), rgb.getB());
    glBegin(GL_POINTS);
    glVertex2i(x, 800 - y);
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
        int baseStore[2];
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

        int b = baseStore[0];

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

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    system("pause");
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);  //窗口位置
    gluOrtho2D(0, 800, 0, 800);
    window = glutCreateWindow("Lab1!");
    glutSwapBuffers();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyDown);
    gluOrtho2D(0, 800, 0, 800);
    glutMainLoop();
    return 0;
}

GLubyte window_pixels[3 * 800 * 800];
GLubyte tmp_window_pixels[3 * 800 * 800];
Color rgb;
Point line_first(-1, -1);
Point polygon_first(-1, -1);
Point polygon_last(-1, -1);
Point circle_center(-1, -1);
Point mouse_connect(-1, -1);
int click = 0;
bool polygon_count_point = FALSE;
bool redo_first = FALSE;
bool last_p = FALSE;
int arr[2] = { -1 };
int arr_first[2] = { -1 };


void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClear(GL_COLOR_BUFFER_BIT);  
    glFlush();
}

void mouse(int bin, int state, int x, int y) {
    Point point(x, y);
    if (state == GLUT_DOWN) {
        glReadPixels(0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
        if (bin == GLUT_LEFT_BUTTON) {
            if (key == 'd') {
                dot = 1;
                rgb.Ran();
                point.drawPoint(rgb);
            }
            else if (key == 'l') {
                if (click == 0) {
                    line_first.setXY(x, y);
                    rgb.Ran();
                    click++;
                }
                else if (click == 1) {
                    Line(line_first, point).drawLine(rgb);
                    click = 0;
                }
            }
            else if (key == 'o') {
                if (circle_center.getX() == -1 && circle_center.getY() == -1) {
                    circle_center.setXY(x, y);
                    rgb.Ran();
                }
                else {
                    Circle(circle_center, point).drawCircle(rgb);
                    circle_center.setXY(-1, -1);
                }
            }
            else if (key == 'p') {
                if (polygon_first.getX() == -1 && polygon_first.getY() == -1) {
                    polygon_first.setXY(x, y);
                    last_p = FALSE;
                    arr_first[0] = x;
                    arr_first[1] = y;
                    rgb.Ran();
                }
                else if (polygon_count_point == FALSE) {
                    Line(polygon_first, point).drawLine(rgb);
                    polygon_count_point = TRUE;
                    polygon_last.setXY(x, y);
                    arr[0] = x;
                    arr[1] = y;
                    redo_first = TRUE;
                }
                else {
                    Line(polygon_last, point).drawLine(rgb);
                    arr[0] = polygon_last.getX();
                    arr[1] = polygon_last.getY();
                    polygon_last.setXY(x, y);
                    redo_first = FALSE;
                }
            }
            else if (key == 'c') {
                std::cout << "clear";
            }
        }
        else if (bin == GLUT_RIGHT_BUTTON) {
            if (key == 'p') {
                Line(polygon_last, polygon_first).drawLine(rgb);
                arr[0] = polygon_last.getX();
                arr[1] = polygon_last.getY();
                polygon_first.setXY(-1, -1);
                polygon_last.setXY(-1, -1);
                polygon_count_point = FALSE;
                last_p = TRUE;
            }
        }
    }
    else if (state == GLUT_UP) {
        mouse_connect.setXY(-1, -1);
        if (bin == GLUT_LEFT_BUTTON) {
            if (key == 'd') {
                dot = 0;
            }
        }
    }
}

void keyDown(unsigned char key1, int x, int y) {
    if (key1 == 'q') {
        glutDestroyWindow(window);
    }
    else if (key1 == 'u') {
        glReadPixels(0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, tmp_window_pixels);
        glDrawPixels(800, 800, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
        glFinish();
        std::copy(tmp_window_pixels, tmp_window_pixels + 3 * 800 * 800, window_pixels);
        if (key == 'p') {
            if (redo_first == TRUE) {
                polygon_count_point = FALSE;
                redo_first = FALSE;
                polygon_first.setXY(-1, -1);
            }
            else {
                polygon_last.setXY(arr[0], arr[1]);
                polygon_count_point = TRUE;
                if (last_p == TRUE) {
                    polygon_first.setXY(arr_first[0], arr_first[1]);
                }
            }
        }
    }
    else if (key1 == 'c') {
        clear();
    }
    else {
        if (key1 == key) {
            return;
        }
        key = key1;
    }
}

void mouseMove(int x, int y) {
    if (dot == 1) {
        Point point_move(-1, -1);
        Point(x, y).drawPoint(rgb);
        if (mouse_connect.getX() == -1 && mouse_connect.getY() == -1) {
            mouse_connect.setXY(x, y);
        }
        else {
            point_move.setXY(x, y);
            Line(mouse_connect, point_move).drawLine(rgb);
            mouse_connect.setXY(x, y);
        }
    }
}

void clear() {
    glReadPixels(0, 0, 800, 800, GL_RGB, GL_UNSIGNED_BYTE, window_pixels);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
}









#include<windows.h>
#include <GL/glut.h>
#include<iostream>
#include<vector>
#include<map>
#include <fstream>
#include <fstream>
using namespace std;

GLint height;
GLint width;
int Height(int y)
{
    return height - y;
}
int Heightt(int y)
{
    return 100 - y;
}

struct Point {
    GLint x;
    GLint y;
    GLfloat r;
    GLfloat g;
    GLfloat b;
};
map<int, int> shapeAtX, shapeAtY;
int psize, linewidth, shapeCount;
GLfloat red, green, blue;
Point p;
vector<Point> Points;
vector<Point> undoPoints;
vector<Point> tmpP;
vector<int> shapeType;

int mousePosX = 0, mousePosY = 0;

void RenderText(float x, float y, const char* text)
{    
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    }
}
void RenderTextColor(float x, float y, const char* text,char c)
{
    if(c=='r')glColor3f(1, 0, 0);
    else if(c=='g')glColor3f(0, 0.7, 0);
    else if(c=='b')glColor3f(0, 0, 1);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    }
}
void RenderTextNumber(float x, float y, int num, int color=0)
{
    vector<char> n;
    while (num) {
        char c = (num % 10) + '0';
        n.push_back(c);
        num /= 10;
    }
    if(color==0)glColor3f(0.9, 0.9, 0);
    else glColor3f(0, 0, 0);
    glRasterPos2f(x+3, Height(y)-15);
    for (int i = n.size()-1; i>=0; i--) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, n[i]);
    }
}
void ShapeNumberings()
{
    for (int i = 0; i < shapeCount; i++) {
        RenderTextNumber(Points[i * 2].x, Points[i * 2].y, i + 1);
    }
}
void ShapeSizeShow()
{
    int x = tmpP[0].x;
    int y = tmpP[0].y;
    RenderText(300, Heightt(18), "  Size ");

    RenderText(300, Heightt(32), "[ W: ");
    RenderTextNumber(340, height-100+18, fabs(mousePosX-x) ,1);
    RenderText(370, Heightt(32), " ]");

    RenderText(300, Heightt(46), "[ H: ");
    RenderTextNumber(340, height-100+32, fabs(mousePosY - y) ,1 );
    RenderText(370, Heightt(46), " ]");
}
void instructions()
{
    RenderTextColor(10, Heightt(18), "b:",'r');
    RenderTextColor(10, Heightt(37), "n:",'g');
    RenderTextColor(10, Heightt(56), "m:",'b');
    RenderText(35, Heightt(75), "[Color]");

    RenderText(120, Heightt(18), "  z: Undo");
    RenderText(120, Heightt(32), "  r: Redo");
    RenderText(120, Heightt(46), "RMB: Cancel");
    RenderText(120, Heightt(60), "  d: Delete all");
    RenderText(120, Heightt(75), "  s: Save Coordinates");

}
void CurrentColor() {
    
    glPointSize(14);
    glBegin(GL_POINTS);
    glColor3f(red, 0, 0); glVertex2d(40, Heightt(17) );
    glColor3f(0, green, 0); glVertex2d(40, Heightt(35) );
    glColor3f(0, 0, blue); glVertex2d(40, Heightt(53) );
    glEnd();

    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2i(50, Heightt(10) );
    glVertex2i(100, Heightt(10) );
    glVertex2i(100, Heightt(60) );
    glVertex2i(50, Heightt(60) );
    glEnd();
}

void canvasBorder() {
    glLineWidth((float)2);
    glColor3f(0, 0, 0);

    glBegin(GL_LINES);
    glVertex2i(0, Height(0));
    glVertex2i(width, Height(0));
    glEnd();


    glBegin(GL_LINES);
    glVertex2i(width, Height(height - 100));
    glVertex2i(0, Height(height - 100));
    glEnd();

}

void GuidLine()
{
    if (shapeAtX[mousePosX]>0) {
        glLineWidth((float)1);
        glColor3f(0, 0, 0);

        glBegin(GL_LINES);
        glVertex2i(mousePosX, Height(0));
        glVertex2i(mousePosX, Height(height-100));
        glEnd();
    }
    if (shapeAtY[mousePosY]>0) {
        glLineWidth((float)1);
        glColor3f(0, 0, 0);

        glBegin(GL_LINES);
        glVertex2i(0, Height(mousePosY)  );
        glVertex2i(width, Height(mousePosY)  );
        glEnd();
    }
}
void draw() {

    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < Points.size(); i+=2) {
        glBegin(GL_QUADS);
        //color of the rectangle/ component
        glColor3f(Points[i].r, Points[i].g, Points[i].b);

        //first point
        glVertex2i(Points[i].x, Height(Points[i].y) );
        //second point
        glVertex2i(Points[i + 1].x, Height(Points[i].y) );
        //third point
        glVertex2i(Points[i+1].x, Height(Points[i+1].y) );
        //fourth point
        glVertex2i(Points[i].x, Height(Points[i + 1].y) );

        glEnd();
    }

    if (tmpP.size() > 0) {
        glBegin(GL_QUADS);
        //color of the rectangle/ component
        glColor3f(0.9, 0.9, 0.9);

        //first point
        glVertex2i(tmpP[0].x, Height( tmpP[0].y) );
        //second point
        glVertex2i(mousePosX, Height( tmpP[0].y) );
        //third point
        glVertex2i(mousePosX, Height( mousePosY) );
        //fourth point
        glVertex2i(tmpP[0].x, Height( mousePosY) );

        glEnd();
        ShapeSizeShow();
    }
    GuidLine();
    ShapeNumberings();
    CurrentColor();
    canvasBorder();
    instructions();
    glFlush();
}

void Redo()
{
    int sz = undoPoints.size();
    if (sz >= 2) {
        Points.push_back(undoPoints[sz - 1]);
        shapeAtX[undoPoints[sz - 1].x]++;
        shapeAtY[undoPoints[sz - 1].y]++;
        undoPoints.pop_back();

        Points.push_back(undoPoints[sz - 2]);
        shapeAtX[undoPoints[sz - 2].x]++;
        shapeAtY[undoPoints[sz - 2].y]++;
        undoPoints.pop_back();

        shapeCount++;
        tmpP.clear();
        draw(); 

    }
}
void AllClear() {
    int sz = Points.size();
    for (int i = sz - 1; i >= 0; i--)undoPoints.push_back(Points[i]);
    Points.clear();
    tmpP.clear();
    shapeCount = 0;
    shapeAtX.clear();
    shapeAtY.clear();

    draw();
}
void tmpClear() {
    tmpP.clear();
    draw();
}
void Undo() {
    int sz = Points.size();
    if (sz > 0) {
        shapeAtX[Points[sz-1].x ]-- ;
        shapeAtY[Points[sz-1].y ]-- ;
        undoPoints.push_back(Points[sz-1]);
        Points.pop_back();

        shapeAtX[Points[sz - 2].x]--;
        shapeAtY[Points[sz - 2].y]--;
        undoPoints.push_back(Points[sz - 2]);
        Points.pop_back();

        shapeCount--;
        draw();
    }

}
void SetColor(unsigned char key) {
    if (key == 'b') { red = red + 0.2; if (red > 1.01)red = 0; }
    else if (key == 'n') { green = green + 0.2; if (green > 1.01)green = 0; }
    else { blue = blue + 0.2; if (blue > 1.01)blue = 0; }
    draw();
}
void Save() {
    string filename = "1";
    cout << "File name To save : ";
    //cin>>filename;
    cout << "(Wait!!!)Saving into File ...\n\n\n";
    ofstream file;
    file.open(filename+".txt");
    if (file) {
        for (int i = 0; i < Points.size(); i+=2) {
            //file<<Points[i].r<<" ";
            //file<<Points[i].g<<" ";
            //file<<Points[i].b<<" ";
            //file<<Points[i].x<<" ";
 
            file <<(int)(i/2)+1 << "    ";
            file << Points[i].x;
            file << " ";
            file << Points[i].y;
            file << " ";
            file << Points[i + 1].x - Points[i].x;
            file << " ";
            file << Points[i + 1].y - Points[i].y;
            file << "\n";

            cout << (int)(i / 2) + 1 << "    ";
            cout << Points[i].x;
            cout << " ";
            cout << Points[i].y;
            cout << " ";
            cout << Points[i + 1].x - Points[i].x;
            cout << " ";
            cout << Points[i + 1].y - Points[i].y;
            cout << "\n";
        }
        file.close();
    }
    else if (!file)cout << "FILE not found\n";

    cout << "\nFile Saved. ";
    cout << "Check project folder to find the file : "<<filename<<".txt\n";

}
static void key(unsigned char key, int x, int y)
{
    if (key == 's')Save(); // detect s
    else if (key == 'z')Undo(); // detect z
    else if (key == 'r')Redo();
    else if (key == 'd')AllClear();
    else if (key == 'q')exit(EXIT_SUCCESS);
    else if (key == 'b' || key == 'n' || key == 'm')SetColor(key);
}

void myMouseFunc(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        tmpClear();
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        p.r = red; p.g = green; p.b = blue;
        p.x = x;
        p.y = y;
        //cout << x << " - " << y << "\n";
        tmpP.push_back(p);

        if (tmpP.size() >= 2) {
            if(undoPoints.size()>0)undoPoints.clear();
            if (tmpP[0].x > tmpP[1].x)swap(tmpP[0].x, tmpP[1].x);
            if (tmpP[0].y > tmpP[1].y)swap(tmpP[0].y, tmpP[1].y);
            for (int i = 0; i < 2; i++) {
                p.r = tmpP[i].r;
                p.g = tmpP[i].g;
                p.b = tmpP[i].b;
                p.x = tmpP[i].x;
                p.y = tmpP[i].y;
                Points.push_back(p);

                shapeAtX[tmpP[i].x]++;
                shapeAtY[tmpP[i].y]++;
            }
            shapeCount++;
            tmpP.clear();
        }
        draw();
    }
}
void myMouseMovement(int x, int y)
{
    mousePosX = x;
    mousePosY = y;
    draw();
}
void init() {
    red = 0; green = 0; blue = 0; psize = 3; linewidth = 2; shapeCount = 0;
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    gluOrtho2D(0.0f, width, 0.0f, height);
}

int main(int argc, char* argv[])
{
    int x, y;
    cout << "Enter UI window size [Width, Height] : ";
    cin >> x >> y;
    cout << "Initialized Window.  Start Drawing your UI: \n";
    width = x; height = y+100;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(10, 10);

    glutCreateWindow("UI Design Helper to find x y dx dy");
    glutDisplayFunc(draw);
    glutKeyboardFunc(key);
    glutMouseFunc(myMouseFunc);
    glutPassiveMotionFunc(myMouseMovement);
    init();

    glutMainLoop();

    return EXIT_SUCCESS;
}

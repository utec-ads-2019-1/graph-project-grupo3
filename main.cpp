

using namespace std;

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <string>
#include <math.h>
#include <iostream>
#include "graph.h"
#include "read.h"
#else
#include <GL/glut.h>
#include <iostream>
#endif

const float PI = 3.141592653;
void drawCircle(float segments, float radius, float sx, float sy);
void draw_line(float x1, float y1, float x2, float y2);
void draw_text(char *text, float x, float y);
void reshape(int w, int h);
void Draw();

int main(int argc, char **argv)
{

  Graph<int, int> g(false);
  string file;
  cout << "Ingresa nombre de file::";
  cin >> file;
  g.readFile(file);
  g.print();
  //Read<int,int> read(file);
  //g=read.getGraph();

  /*FOR OPENGL*/

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(1200, 800);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("A Binary search tree");

  glutDisplayFunc(Draw);
  glutIdleFunc(Draw);
  glutMainLoop();

  return EXIT_SUCCESS;
}

void Draw()
{
  glClearColor(1.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0, 10, -30);
  glColor3f(1, 1, 1);
  //drawNode(node1,node2, 0, 0, 0);
  glutSwapBuffers();
}

void drawCircle(float segments, float radius, float sx, float sy)
{
  float theta = 2 * 3.1415926 / segments;
  float tan_factor = tanf(theta);
  float radial_factor = cosf(theta);
  float x = radius;
  float y = 0;

  int cache_pt = 0;
  double cache_x;
  double cache_y;

  glBegin(GL_LINES);
  for (int ii = 0; ii < segments; ii++)
  {
    if (!cache_pt)
    {
      cache_x = x + sx;
      cache_y = y + sy;
      cache_pt = 1;
    }
    else
    {
      //glVertex2f(cache_x,cache_y);
      glVertex2f(x + sx, y + sy);
      cache_x = x + sx;
      cache_y = y + sy;
    }
    float tx = -y;
    float ty = x;
    x += tx * tan_factor;
    y += ty * tan_factor;
    x *= radial_factor;
    y *= radial_factor;
  }
  glEnd();
}

void draw_line(float x1, float y1, float x2, float y2)
{
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}
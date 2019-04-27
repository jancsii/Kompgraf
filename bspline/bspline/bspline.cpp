#include "stdafx.h"
#include "bevgrafmath2017.h"
#include "GL/glut.h"
#include <vector>

GLsizei winWidth = 800;
GLsizei winHeight = 600;

std::vector<vec2> points = {};
std::vector<vec2> splinePoints = {};

GLint dragged = -1;
GLint pickRad = 4;

mat24 G;


void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	//glColor3f(0.0, 0.0, 0.0);
	glPointSize(7.0);
	glLineWidth(2.0);
}

void control()
{
	glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

void spline()
{
	mat4 M = { -1, 3, -3, 1,
				3, -6, 0, 4,
				-3, 3, 3, 1,
				1, 0, 0, 0
			};
	
	M = (1.0 / 6.0) * M;

	vec2 segedS = 2 * points[0] - points[1];
	vec2 S = (1.0 / 6.0) * (segedS + 4 * points[0] + points[1]);
	vec2 segedE = 2 * points[points.size() - 1] - points[points.size() - 2];
	vec2 E = (1.0 / 6.0) * (points[points.size() - 2] + 4 * points[points.size() - 1] + segedE);

	splinePoints.push_back(segedS);
	for (int i = 0; i < points.size(); i++)
	{
		splinePoints.push_back(points[i]);
	}
	splinePoints.push_back(segedE);

	for (int i = 0; i < points.size() - 1; i++)
	{
		G = { splinePoints[i], splinePoints[i + 1], splinePoints[i + 2], splinePoints[i + 3] };

		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 0.0, 0.0);
		for (float t = 0.0; t <= 1.0; t += 0.001)
		{
			vec4 T = { t * t * t, t * t, t, 1.0 };
			vec2 point = G * M * T;
			glVertex2f(point.x, point.y);
		}
		glEnd();

		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 1.0);
		for (float t = 0.0; t <= 1.0; t += 0.001)
		{
			vec4 T = { t * t * t, t * t, t, 1.0 };
			vec2 point = G * M * T;
			if (t == 0.0 && i >= 1)
				glVertex2f(point.x, point.y);
		}
		glEnd();
	}
	splinePoints.clear();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	control();

	if (points.size() > 3)
		spline();

	glFlush();
	//glutSwapBuffers();
}

void processMouse(int button, int state, int x, int y)
{
	vec2 point = vec2(x, winHeight - y - 1);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		points.push_back(point);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < points.size(); i++)
		{
			if (dist(points[i], point) < 10)
			{
				points.erase(points.begin() + i);
				break;
			}
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		dragged = 0;
		
			for (int i = 0; i < points.size(); i++)
			{
				if (dist(points[i], point) < 10)
				{
					dragged = i;
				}
			}
			point = vec2(x, winHeight - y - 1);
			points[dragged] = point;
		
	}

	glutPostRedisplay();
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0)
	{
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
		if (points[dragged].x + pickRad >= winWidth)
			points[dragged].x = winWidth - pickRad;
		if (points[dragged].x - pickRad <= 0)
			points[dragged].x = 0 + pickRad;
		if (points[dragged].y + pickRad >= winHeight)
			points[dragged].y = winHeight - pickRad;
		if (points[dragged].y - pickRad <= 0)
			points[dragged].y = 0 + pickRad;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("B-spline");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();

	return 0;
}


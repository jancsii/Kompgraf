#include "stdafx.h"
#include <GL/glut.h>
#include "./bevgrafmath2017.h"
#include <math.h>
#include <vector>

GLsizei winWidth = 800;
GLsizei winHeight = 600;

int dragged = -1;

float beta0 = 0.0;

std::vector<vec2> points = {};
std::vector<vec2> points1 = {};
std::vector<vec2> points2 = {};
std::vector<vec2> points3 = {};
std::vector<vec2> points4 = {};

bool first = true;
bool second = true;
bool third = true;
bool fourth = true;

void init() {

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.f, winWidth, 0.f, winHeight);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(7.0);
}

void control() {

	if (points.size() > 3) {

		glLineWidth(1.0);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);

		for (int i = 0; i < points.size(); i++) {

			glVertex2f(points[i].x, points[i].y);
		}

		glVertex2f(points[0].x, points[0].y);
		glVertex2f(points[points.size() - 1].x, points[points.size() - 1].y);
		glEnd();
	}

	glPointSize(10.0);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++) {

		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

void grid() {

	glColor3f(0.8f, 0.8f, 0.8f);
	glLineWidth(1.0);
	int delta = 20;
	for (int i = 0; i <= winWidth; i += delta) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= winWidth; j += delta) {
			glVertex2f(i, j);
		}
		glEnd();
	}

	for (int i = 0; i <= winHeight; i += delta) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= winWidth; j += delta) {
			glVertex2f(j, i);
		}
		glEnd();
	}
}

double beta(int k) {

	if (k == 0) 
		return beta0;
	else if (k == 1) 
		return sqrt(2 + beta0);
	else if (k == 2) 
		return sqrt(2 + beta(k - 1));
	else if (k == 3) 
		return sqrt(2 + beta(k - 1));
	else if (k == 4) 
		return sqrt(2 + beta(k - 1));
	else 
		return 0;
}

double gamma(int k) {

	return -1.0 / (3.0 * (1.0 - pow(beta(k), 2)) * (1.0 + beta(k)));
}

double alpha(int k, int i) {

	if (i == 0)
		return (1.0 / 60.0) * ((-90 * gamma(k + 1)) - 1);
	else if (i == 1)
		return (1.0 / 60.0) * ((90 * gamma(k + 1)) + 43);
	else if (i == 2)
		return (1.0 / 60.0) * ((90 * gamma(k + 1)) + 17);
	else if (i == 3)
		return (1.0 / 60.0) * ((-90 * gamma(k + 1)) + 1);
	else 
		return 0.0;
}

void calculate() {

	int k = 1;
	auto n = int(points.size());
	for (int j = 1; j <= n; ++j) {
		points1.push_back(points[j % n]);
		points1.push_back(
			alpha(k - 1, 0) * points[(j - 1) % n] + alpha(k - 1, 1) * points[j % n] +
			alpha(k - 1, 2) * points[(j + 1) % n] + alpha(k - 1, 3) * points[(j + 2) % n]);
		points1.push_back(
			alpha(k - 1, 3) * points[(j - 1) % n] + alpha(k - 1, 2) * points[j % n] +
			alpha(k - 1, 1) * points[(j + 1) % n] + alpha(k - 1, 0) * points[(j + 2) % n]);
	}

	k = 2;
	n = int(points1.size());
	for (int j = 1; j <= n; ++j) {
		points2.push_back(points1[j % n]);
		points2.push_back(
			alpha(k - 1, 0) * points1[(j - 1) % n] + alpha(k - 1, 1) * points1[j % n] +
			alpha(k - 1, 2) * points1[(j + 1) % n] + alpha(k - 1, 3) * points1[(j + 2) % n]);
		points2.push_back(
			alpha(k - 1, 3) * points1[(j - 1) % n] + alpha(k - 1, 2) * points1[j % n] +
			alpha(k - 1, 1) * points1[(j + 1) % n] + alpha(k - 1, 0) * points1[(j + 2) % n]);
	}

	k = 3;
	n = int(points2.size());
	for (int j = 1; j <= n; ++j) {
		points3.push_back(points2[j % n]);
		points3.push_back(
			alpha(k - 1, 0) * points2[(j - 1) % n] + alpha(k - 1, 1) * points2[j % n] +
			alpha(k - 1, 2) * points2[(j + 1) % n] + alpha(k - 1, 3) * points2[(j + 2) % n]);
		points3.push_back(
			alpha(k - 1, 3) * points2[(j - 1) % n] + alpha(k - 1, 2) * points2[j % n] +
			alpha(k - 1, 1) * points2[(j + 1) % n] + alpha(k - 1, 0) * points2[(j + 2) % n]);
	}

	k = 4;
	n = int(points3.size());
	for (int j = 1; j <= n; ++j) {
		points4.push_back(points3[j % n]);
		points4.push_back(
			alpha(k - 1, 0) * points3[(j - 1) % n] + alpha(k - 1, 1) * points3[j % n] +
			alpha(k - 1, 2) * points3[(j + 1) % n] + alpha(k - 1, 3) * points3[(j + 2) % n]);
		points4.push_back(
			alpha(k - 1, 3) * points3[(j - 1) % n] + alpha(k - 1, 2) * points3[j % n] +
			alpha(k - 1, 1) * points3[(j + 1) % n] + alpha(k - 1, 0) * points3[(j + 2) % n]);
	}
}

void draw() {

	points1.clear();
	points2.clear();
	points3.clear();
	points4.clear();

	calculate();

	if (first) {
		glLineWidth(4.0);
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < points1.size(); i++) {
			glVertex2d(points1[i].x, points1[i].y);
		}
		glEnd();
	}

	if (second) {
		glLineWidth(4.0);
		glColor3f(1.0f, 0.5f, 0.5f);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < points2.size(); i++) {
			glVertex2d(points2[i].x, points2[i].y);
		}
		glEnd();
	}

	if (third) {
		glLineWidth(4.0);
		glColor3f(0.5f, 1.0f, 0.5f);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < points3.size(); i++) {
			glVertex2d(points3[i].x, points3[i].y);
		}
		glEnd();
	}

	if (fourth) {
		glLineWidth(4.0);
		glColor3f(0.5f, 0.5f, 1.0f);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < points4.size(); i++) {
			glVertex2d(points4[i].x, points4[i].y);
		}
		glEnd();
	}
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	grid();
	control();
	if (points.size() > 3) {
		draw();
	}
	glFlush();
}

GLint getActivePointv2(std::vector<vec2> p, GLint size, GLint sens, GLint x, GLint y) {
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	vec2 p = vec2(xMouse, winHeight - yMouse - 1);
	GLint i, j;


	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		if ((i = getActivePointv2(points, points.size(), 8, xMouse, winHeight - yMouse)) != -1)

			dragged = i;
		else {
			points.push_back(p);
		}

		if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
			dragged = -1;
		if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) {
			points.clear();
		}

		glutPostRedisplay();
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) {
	GLint i, j;

	if (dragged >= 0) {
		points[dragged].x = xMouse;
		points[dragged].y = winHeight - yMouse;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'q':
		if (first) {
			first = false;
		}
		else {
			first = true;
		}

		break;

	case 'w':
		if (second) {
			second = false;
		}
		else {
			second = true;
		}

		break;

	case 'e':
		if (third) {
			third = false;
		}
		else {
			third = true;
		}

		break;

	case 'r':
		if (fourth) {
			fourth = false;
		}
		else {
			fourth = true;
		}

		break;

	case 't':
		int temp;
		printf("Give me a tenson number: \n");
		scanf_s("%d", &temp);

		if (temp == -1 || temp < -2) {
			printf("Invalid number! \n");
			temp = 0.0;
			printf("Tension = 0!\n");
			break;
		}
		else {
			printf("Valid number! \n");
			beta0 = temp;
			break;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Subdivision");
	init();
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}


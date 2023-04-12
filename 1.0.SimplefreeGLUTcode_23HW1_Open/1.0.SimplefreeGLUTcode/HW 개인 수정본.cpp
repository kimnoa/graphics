#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define TO_RADIAN		0.017453292519943296 

//int rightbuttonpressed = 0;
int leftbuttonpressed = 0, center_selected = 0, shiftkeypressed = 0; //문제 (e)
int altpressed = 0, rightpressed = 0; //문제 (f), (g)
int ctrlpressed = 0; //문제 (g)

int mirror_x = 0;//문제 (h) x축 대칭
int mirror_y = 0;//y축 대칭
int mirror_xy = 0;//원점 대칭

float rot = 0; //(h) 회전 각도


float r, g, b; // Background color
float px, py, qx, qy; // Line (px, py) --> (qx, qy)
float R = 0.00f, s = 0.00f, h = 0.00f; //문제 (d)
float angle = 0.00f; //문제 (d)
float x_fix = 0.00f, y_fix = 0.00f; //문제 (f)
int n_object_points = 6;
float object[6][2], object_center_x, object_center_y;
float rotation_angle_in_degree;
int window_width, window_height;



void draw_axes() {
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.975f, 0.025f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.975f, -0.025f);
	glVertex2f(1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);
	glVertex2f(0.025f, 0.975f);
	glVertex2f(0.0f, 1.0f);
	glVertex2f(-0.025f, 0.975f);
	glVertex2f(0.0f, 1.0f);
	glEnd();
	glLineWidth(1.0f);

	glPointSize(7.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPointSize(1.0f);
}
void draw_line(float px, float py, float qx, float qy) {
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(px, py);
	glVertex2f(qx, qy);
	glEnd();
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(px, py);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(qx, qy);
	glEnd();
	glPointSize(1.0f);
}

void draw_object(void) {
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(object[i][0], object[i][1]);
	glEnd();
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(object[i][0], object[i][1]);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(object_center_x, object_center_y);
	glEnd();
	glPointSize(1.0f);
}

void reflection_x(void) { //x축 대칭
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(object[i][0], -object[i][1]);
	glEnd();
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(object[i][0], -object[i][1]);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(object_center_x, -object_center_y);
	glEnd();
	glPointSize(1.0f);
}

void reflection_y(void) {
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(-object[i][0], object[i][1]);
	glEnd();
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(-object[i][0], object[i][1]);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(-object_center_x, object_center_y);
	glEnd();
	glPointSize(1.0f);
}

void reflection_xy(void) {
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(-object[i][0], -object[i][1]);
	glEnd();
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(-object[i][0], -object[i][1]);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(-object_center_x, -object_center_y);
	glEnd();
	glPointSize(1.0f);
}

void rotation(void) { //문제 (h) 원점을 중심으로 한 회전
	float temp_x = 0.00f, temp_y = 0.00f;
	for (int i = 0; i < n_object_points; i++)
	{
		temp_x = object[i][0];
		temp_y = object[i][1];

		object[i][0] = temp_x * cos(rot) - temp_y * sin(rot);
		object[i][1] = temp_x * sin(rot) + temp_y * cos(rot);

	}
	temp_x = object_center_x;
	temp_y = object_center_y;

	object_center_x = temp_x * cos(rot) - temp_y * sin(rot);
	object_center_y = temp_x * sin(rot) + temp_y * cos(rot);

}

void wheel(int wheel,int direction, int x, int y) //문제 (d)
{
	R = sqrtf(pow((px - qx), 2) + pow((py - qy), 2)); //두 점 사이의 거리
	s = qx - px; // 두 점 사이의 x 좌표 차이
	h = qy - py; // 두 점 사이의 y 좌표 차이
	if (h < 0)
		s = -s;
	angle = acosf(s / R); //라디안 값 반환
	float d_qx = 0.00f, d_qy = 0.00f; //qx와 qy의 변화량

	if (direction >0) //마우스를 휠을 올릴 때
	{
		angle+=rotation_angle_in_degree*0.1;
		d_qx = R * cos(angle);
		d_qy = R * sin(angle);
		if (h < 0)
		{
			d_qx = -d_qx;
			d_qy = -d_qy;
		}
		qx = px + d_qx; //변화량을 저장
		qy = py +d_qy;
	}
	else { //마우스를 휠을 내릴 때
		
		angle -= rotation_angle_in_degree * 0.1;
		d_qx = R * cos(angle);
		d_qy = R * sin(angle);
		if (h < 0)
		{
			d_qx = -d_qx;
			d_qy = -d_qy;
		}
		qx = px + d_qx; //변화량을 저장
		qy = py + d_qy;
	}

	//printf("%.2f %.2f %.2f %.2f %.2f \n", qx, qy, d_qx, d_qy, angle);
	glutPostRedisplay(); //화면에 출력
}


void display(void) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	draw_axes();
	draw_line(px, py, qx, qy);
	draw_object();
	if (mirror_x)
		reflection_x();
	if (mirror_y)
		reflection_y();
	if (mirror_xy)
		reflection_xy();
	rotation();
	glutPostRedisplay();
	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		r = 1.0f; g = b = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case 'g':
		g = 1.0f; r = b = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case 'b':
		b = 1.0f; r = g = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case 's':
		r = 250.0f / 255.0f, g = 128.0f / 255.0f, b = 114.0f / 255.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;

	case 49: //숫자 1키
	{
		if (mirror_x)
			mirror_x = 0;
		else
			mirror_x = 1;
		break;
	}

	case 50: //숫자 2키
	{
		if (mirror_y)
			mirror_y = 0;
		else
			mirror_y = 1;
		break;
	}

	case 51: //숫자 3키
	{
		if (mirror_xy)
			mirror_xy = 0;
		else
			mirror_xy = 1;
		break;
	}
	}
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		r -= 0.1f;
		if (r < 0.0f) r = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		r += 0.1f;
		if (r > 1.0f) r = 1.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		g -= 0.1f;
		if (g < 0.0f) g = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		g += 0.1f;
		if (g > 1.0f) g = 1.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;

	case GLUT_KEY_SHIFT_L: //왼쪽 쉬프트 키를 누를 때
	{
		shiftkeypressed = 1;
		break;
	}

	case GLUT_KEY_ALT_L: //왼쪽 alt 키를 누를 때
	{
		altpressed = 1;
		break;
	}

	case GLUT_KEY_CTRL_L: //왼쪽 ctrl 키를 누를 때
	{
		ctrlpressed = 1;
		break;
	}

	}
}

void specialup(int key, int x, int y) { //shift, alt, ctrl key를 떼었을 때의 동작
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:
	{
		shiftkeypressed = 0; //점 이동 정지
		break;
	}

	case GLUT_KEY_ALT_L:
	{
		altpressed = 0; //다각형 이동 정지
		break;
	}

	case GLUT_KEY_CTRL_L:
	{
		ctrlpressed = 0; //스케일 정지
		break;
	}

	default:
		break;
	}

}


int prevx, prevy;
void mousepress(int button, int state, int x, int y) {
	prevx = x; prevy = y;
	float w = (float)window_width, h = (float)window_height;
	float mx = (- w + prevx * 2) / 500.0f; //창의 크기를 변화시켜도 좌표계에서 일정 값을 가지도록 조정(정규화)
	float my = (h - prevy * 2) / 500.0f;
	if ((button == GLUT_LEFT_BUTTON) && state == GLUT_DOWN)
	{
		leftbuttonpressed = 1;
	}
	else
		leftbuttonpressed = 0; //없을 시, 오른쪽 마우스로도 점 이동 가능

	float range = 0.02f; //점 주변까지 클릭 허용
	if (mx >= px - range && mx <= px + range && my >= py-range && my <= py+range)
	{
		center_selected = 1;
	}
	else
		center_selected = 0;

	if ((button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN)
	{
		rightpressed = 1;
		x_fix = mx, y_fix = my;
	}
	else
	{
		rightpressed = 0; //없을 시, 왼쪽 마우스로도 이동 가능
		rot = 0.00f;
	}


}

void mousemove(int x, int y) {
	float w = (float)window_width, h = (float)window_height;
	float mx = (-w + x * 2) / 500.0f; 
	float my = (h - y * 2) / 500.0f;
	float d_x = mx - x_fix, d_y = my - y_fix;

	//점 이동
	if (leftbuttonpressed && center_selected && shiftkeypressed)
	{
		px = mx;
		py = my;
	}

	//다각형 이동
	if (rightpressed && altpressed)
	{
		// 마우스 이동 측정 = d_x, d_y
		for (int i = 0; i < n_object_points; i++)
		{
			object[i][0] += d_x;
			object[i][1] += d_y;
		}
		object_center_x += d_x;
		object_center_y += d_y;
	}

	//스케일
	if (ctrlpressed && rightpressed)
	{
		if (d_x < -0.00f)
		{
			for (int i = 0; i < n_object_points; i++)
			{
				object[i][0] = (object_center_x + 9 * object[i][0]) / 10;
				object[i][1] = (object_center_y + 9 * object[i][1]) / 10;
			}
		}
		else if (d_x > 0.00f) {
			for (int i = 0; i < n_object_points; i++)
			{
				object[i][0] = (-object_center_x + 10 * object[i][0]) / 9;
				object[i][1] = (-object_center_y + 10 * object[i][1]) / 9;
				
			}
		}
	}

	if (shiftkeypressed && rightpressed && (d_y != 0.0f)) //마우스 y변위만큼 빠르게 회전
	{
		rot += 0.01f * d_y;
		//printf("%.4f \n", rot);
	}

	x_fix = mx; //마우스 기준점 재설정
	y_fix = my;
}


void reshape(int width, int height) {
	// DO NOT MODIFY THIS FUNCTION!!!
	window_width = width, window_height = height;
	glViewport(0.0f, 0.0f, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-window_width / 500.0f, window_width / 500.0f, -window_height / 500.0f, window_height / 500.0f, -1.0f, 1.0f);

	glutPostRedisplay();
}


void close(void) {
	fprintf(stdout, "\n^^^ The control is at the close callback function now.\n\n");
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mousepress);
	glutMouseWheelFunc(wheel);
	glutMotionFunc(mousemove);
	glutReshapeFunc(reshape);
	glutCloseFunc(close);

	glutSpecialUpFunc(specialup); //키보드에서 special key를 떼었을 때 동작하는 함수
}

void initialize_renderer(void) {
	register_callbacks();
	r = 250.0f / 255.0f, g = 128.0f / 255.0f, b = 114.0f / 255.0f; // Background color = Salmon
	px = -0.75f, py = -0.30f, qx = -0.55f, qy = -0.30f; //(b)
	rotation_angle_in_degree = 1.0f; // 1 degree

	float sq_cx = -0.65f, sq_cy = +0.30f, sq_side = 0.20f;
	object[0][0] = sq_cx + sq_side;
	object[0][1] = sq_cy - sq_side;
	object[1][0] = sq_cx + 2 * sq_side;
	object[1][1] = sq_cy + sq_side;
	object[2][0] = sq_cx + sq_side;
	object[2][1] = sq_cy + sq_side;
	object[3][0] = sq_cx ;
	object[3][1] = sq_cy + 1.5 * sq_side;
	object[4][0] = sq_cx - 1.5 * sq_side;
	object[4][1] = sq_cy + 1.5 * sq_side;
	object[5][0] = sq_cx - 1.5 * sq_side;
	object[5][1] = sq_cy - sq_side;
	object_center_x = object_center_y = 0.0f;
	for (int i = 0; i < n_object_points; i++) { //전역 변수로 점 개수는 6으로 설정됨
		object_center_x += object[i][0];
		object_center_y += object[i][1];
	}
	object_center_x /= n_object_points;
	object_center_y /= n_object_points;

}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = TRUE;
	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char* program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 4
void main(int argc, char* argv[]) {
	char program_name[64] = "Sogang CSE4170 Simple 2D Transformations";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'r', 'g', 'b', 's', 'q'",
		"    - Special keys used: LEFT, RIGHT, UP, DOWN",
		"    - Mouse used: SHIFT/L-click and move, ALT/R-click and move, CTRL/R-click and move",
		"    - Wheel used: up and down scroll"
		"    - Other operations: window size change"
	};

	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); // <-- Be sure to use this profile for this example code!
	//	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_RGBA);

	glutInitWindowSize(750, 750);
	glutInitWindowPosition(500, 200);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	// glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT); // default
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutMainLoop();
	fprintf(stdout, "^^^ The control is at the end of main function now.\n\n");
}
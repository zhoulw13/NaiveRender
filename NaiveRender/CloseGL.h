#pragma once

#include <sstream>
#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include "Transform.h"

typedef struct Point {
	int x, y;

	Point() {};
	Point(int y, int x) :x(x), y(y) {};
};

enum MOVE_EVENT {
	TRANSITION_LEFT,
	TRANSITION_RIGHT,
	TRANSITION_UP,
	TRANSITION_DOWN,
	TRANSITION_IN,
	TRANSITION_OUT,
	SCALE_IN,
	SCALE_OUT,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	ROTATE_UP,
	ROTATE_DOWN
};


class CloseGL {
private:
	glm::vec3 eye, up, face;
	vector<glm::vec3> v_lst;
	vector<glm::vec3> f_lst;

	glm::mat3 k_camera;
	Transform *t;
	glm::mat3 rot;
	glm::vec3 tra;
public:
	unsigned char *data;

public:
	CloseGL();
	bool readvals(stringstream &ss, const int num, float *values);
	void readfile(const char *filename);

	unsigned char *get_data() { return data; };
	void reset_data();
	void reset_transform();
	void set_pixel(int, int, const unsigned char *);
	void set_segment(int, int, int, const unsigned char *);
	void set_triangle(Point, Point, Point, const unsigned char *);
	int intersect(Point, Point, int);

	Point projection(glm::vec3);
	void camera_move(MOVE_EVENT);
	void render();
};
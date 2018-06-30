#pragma once
using namespace std;

#include <glm/glm.hpp>
#include "Transform.h"
#include "ReadObj.h"
#include "RayTracing.h"

typedef struct Point {
	int x, y;
	int id;

	Point() {};
	Point(int y, int x) :x(x), y(y) {};
};

enum MOVE_EVENT {
	TRANSLATE_LEFT,
	TRANSLATE_RIGHT,
	TRANSLATE_UP,
	TRANSLATE_DOWN,
	TRANSLATE_IN,
	TRANSLATE_OUT,
	SCALE_IN,
	SCALE_OUT,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	ROTATE_UP,
	ROTATE_DOWN
};

enum COLOR_SCHEME {
	SEGMENT,
	FACE
};

enum PROJECTION_METHOD {
	PERSPECTIVE,
	ORTHOGONAL
};


class CloseGL {
private:
	glm::vec3 eye, up, front;
	vector<ReadObj *> obj;
	RayTracing *ray_tracing;

	glm::mat3 k_camera;
	Transform *t;
	glm::mat3 rot;
	glm::vec3 tra;
	glm::mat3 sca;

	COLOR_SCHEME color_scheme;
	PROJECTION_METHOD projection_method;
	bool ray_tracing_flag;

	float scale;

	int texture_h, texture_w;

public:
	unsigned char *data;
	unsigned char *texture;

public:
	CloseGL();

	unsigned char *get_data() { return data; };
	void readfile(const char *);
	void loadtexture(unsigned char *, int, int);
	void reset_data();
	void reset_transform();
	void reset_camera();
	void set_pixel(int, int, const unsigned char *);
	void set_segment(int, int, int, const unsigned char *);
	void set_segment(Point, Point, const unsigned char *);
	void set_triangle(int, int, const unsigned char *);
	bool in_range(Point);
	int intersect(Point, Point, int);

	Point projection(glm::vec3);
	void camera_move(MOVE_EVENT);
	void render();

	void set_color_scheme(COLOR_SCHEME);
	void set_projection(PROJECTION_METHOD);
	void set_ray_tracing();
};
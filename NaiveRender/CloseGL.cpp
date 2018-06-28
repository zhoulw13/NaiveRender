#include "CloseGL.h"
#include "constants.h"

#include <fstream>
#include <iostream>
#include <algorithm>

void print(glm::vec3 p) {
	cout << "v " << p[0] << " " << p[1] << " " << p[2] << endl;
}

void print(glm::vec4 p) {
	cout << "v " << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << endl;
}

void print(glm::mat3 m) {
	print(m[0]);
	print(m[1]);
	print(m[2]);
}

void print(Point p) {
	cout << "p " << p.y << " " << p.x << endl;
}

bool CloseGL::readvals(stringstream &ss, const int num, float *values) {
	for (int i = 0; i < num; i++) {
		ss >> values[i];
		if (ss.fail())
			return false;
	}
	return true;
}

void CloseGL::readfile(const char *filename) {
	v_lst.clear();
	f_lst.clear();
	reset_data();

	string str, cmd;
	float values[3];
	ifstream in;
	in.open(filename);
	if (in.is_open()) {
		getline(in, str);
		while (in) {
			if (str.find_first_not_of(" \t\r\n") == string::npos || str[0] == '#') {
				getline(in, str);
				continue;
			}

			stringstream ss(str);
			ss >> cmd;
			if (cmd == "v") {
				bool valid = readvals(ss, 3, values);
				if (valid)
					v_lst.push_back(glm::vec3(values[0], values[1], values[2]));
			}
			else if (cmd == "f") {
				bool valid = readvals(ss, 3, values);
				if (valid)
					f_lst.push_back(glm::vec3(values[0]-1, values[1]-1, values[2]-1));
			}
			else {
				cout << "??" << endl;
			}
			getline(in, str);
		}
	}

	radius = 0;
	int length = v_lst.size();
	for (int i = 0; i < length; i++) {
		if (glm::length(v_lst[i]) > radius) {
			radius = glm::length(v_lst[i]);
		}
	}
	reset_camera();

	cout << "Load Over" << endl;
	cout << "Radius: " << radius << "\n";
	cout << "vertex num: " << v_lst.size() << "\n";
	cout << "front  num: " << f_lst.size() << "\n";
}

CloseGL::CloseGL() {
	data = new unsigned char[window_width * window_height * 3];
	t = new Transform();
	color_scheme = SEGMENT;
	projection_method = PERSPECTIVE;
	radius = 0;

	reset_data();
	reset_transform();
}

void CloseGL::reset_transform() {
	eye = eye_init;
	up = glm::normalize(up_init);
	front = -glm::normalize(eye_init);
	scale = 1;

	rot = glm::mat3(1.0f);
	tra = eye;
	sca = glm::mat3(1.0f);
}

void CloseGL::reset_camera() {
	if (projection_method == PERSPECTIVE) {
		k_camera = glm::mat3(
			window_height / 2 /  radius * glm::length(eye), 0, window_width / 2,
			0, window_height / 2 / radius * glm::length(eye), window_height / 2,
			0, 0, 1);
	}
	else if (projection_method == ORTHOGONAL) {
		k_camera = glm::mat3(
			window_height / 2 / radius, 0, window_width / 2,
			0, window_height / 2 / radius, window_height / 2,
			0, 0, 1);
	}
}

void CloseGL::reset_data() {
	for (int i = 0; i < window_height; i++) {
		for (int j = 0; j < window_width; j++) {
			int ind = (i * window_width + j) * 3;
			data[ind] = bg_color[0];
			data[ind + 1] = bg_color[1];
			data[ind + 2] = bg_color[2];
		}
	}
}

void CloseGL::set_pixel(int i, int j, const unsigned char *color) {
	if (in_range(Point(i, j))) {
		int ind = (i * window_width + j) * 3;
		data[ind] = color[0];
		data[ind + 1] = color[1];
		data[ind + 2] = color[2];
	}
}

bool CloseGL::in_range(Point p) {
	if (p.y < 0 || p.y >= window_height || p.x < 0 || p.x >= window_width)
		return false;
	return true;
}

//horizontal segment
void CloseGL::set_segment(int i, int s, int e, const unsigned char *color) {
	if (!in_range(Point(i, s)) && !in_range(Point(i, e)))
		return;
	int sum = s + e;
	s = min(s, e);
	e = sum - s;
	for (int k = s; k <= e; k++)
		set_pixel(i, k, color);
}

//arbitrary segment
void CloseGL::set_segment(Point p1, Point p2, const unsigned char *color) {
	if (!in_range(p1) && !in_range(p2))
		return;
	float k, e;
	int x, y, dx, dy, sig;

	if (p1.x == p2.x) {
		for (int i = min(p1.y, p2.y); i <= max(p1.y, p2.y); i++)
			set_pixel(i, p1.x, color);
		return;
	}

	k = float(p2.y - p1.y) / (p2.x - p1.x);
	if ((abs(k) < 1 && p1.x > p2.x)
		|| (abs(k) >= 1 && p1.y > p2.y)) {
		swap(p1, p2);
	}

	dx = p2.x - p1.x;
	dy = p2.y - p1.y;

	sig = (k >= 0) ? 1 : -1;
	e = -1 * sig*0.5;
	x = p1.x;
	y = p1.y;
	if (abs(k) < 1) {
		for (int i = 0; i < dx; i++) {
			set_pixel(y, x, color);
			x++;
			e += k;
			if (sig*e >= 0) {
				y += sig;
				e -= sig;
			}
		}
	}
	else {
		k = 1 / k;
		for (int i = 0; i < dy; i++) {
			set_pixel(y, x, color);
			y++;
			e += k;
			if (sig*e >= 0) {
				x += sig;
				e -= sig;
			}
		}
	}
}

void CloseGL::set_triangle(Point p1, Point p2, Point p3, const unsigned char *color) {
	if (p1.x == INT_MAX || p2.x == INT_MAX || p3.x == INT_MAX)
		return;
	if (color_scheme == FACE) {
		if (!in_range(p1) && !in_range(p2) && !in_range(p3))
			return;
		Point sorted[3];
		sorted[0] = p1;
		sorted[1] = p2;
		sorted[2] = p3;
		sort(sorted, sorted + 3, [](Point a, Point b) {return a.y < b.y; });

		int s, e;
		if (sorted[0].y != sorted[1].y) {
			for (int i = sorted[0].y; i <= sorted[1].y; i++) {
				s = intersect(sorted[0], sorted[1], i);
				e = intersect(sorted[0], sorted[2], i);
				set_segment(i, s, e, color);
			}
		}
		if (sorted[2].y != sorted[1].y) {
			for (int i = sorted[2].y; i >= sorted[1].y; i--) {
				s = intersect(sorted[2], sorted[1], i);
				e = intersect(sorted[2], sorted[0], i);
				set_segment(i, s, e, color);
			}
		}
	}
	else if (color_scheme == SEGMENT) {
		set_segment(p1, p2, color);
		set_segment(p1, p3, color);
		set_segment(p3, p2, color);
	}
}

int CloseGL::intersect(Point p1, Point p2, int y) {
	return (double)(y - p1.y) / (double)(p2.y - p1.y) * (p2.x - p1.x) + p1.x;
}

Point CloseGL::projection(glm::vec3 p) {
	glm::vec4 ph = vec4(p*sca, 1);
	glm::mat4x3 x(rot);
	x[3] = -rot * tra;
	glm::vec3 p_ = x * ph * k_camera;
	if (p_[2] < z_near || p_[2] > z_far)
		return Point(INT_MAX, -1);
	if (projection_method == PERSPECTIVE)
		return Point(p_[1] / p_[2], p_[0] / p_[2]);
	else if (projection_method == ORTHOGONAL) {
		p_ = x * ph;
		p_[2] = -1;
		p_ = p_ * k_camera;
		return Point(-p_[1], -p_[0]);
	}
}

void CloseGL::camera_move(MOVE_EVENT event) {
	if (event == TRANSLATE_LEFT)
		tra += t->Translation(front, up, glm::vec3(-1, 0, 0));
	else if (event == TRANSLATE_RIGHT)
		tra += t->Translation(front, up, glm::vec3(1, 0, 0));
	else if (event == TRANSLATE_UP)
		tra += t->Translation(front, up, glm::vec3(0, 1, 0));
	else if (event == TRANSLATE_DOWN)
		tra += t->Translation(front, up, glm::vec3(0, -1, 0));
	else if (event == TRANSLATE_IN)
		tra += t->Translation(front, up, glm::vec3(0, 0, 1));
	else if (event == TRANSLATE_OUT)
		tra += t->Translation(front, up, glm::vec3(0, 0, -1));
	else if (event == ROTATE_LEFT)
		rot = rot * t->Rotation(front, up, -1, Z_ROTATE);
	else if (event == ROTATE_RIGHT)
		rot = rot * t->Rotation(front, up, 1, Z_ROTATE);
	else if (event == ROTATE_UP)
		rot = rot * t->Rotation(front, up, 1, X_ROTATE);
	else if (event == ROTATE_DOWN)
		rot = rot * t->Rotation(front, up, -1, X_ROTATE);
	else if (event == SCALE_OUT)
		sca = t->Scale(scale, 1);
	else if (event == SCALE_IN)
		sca = t->Scale(scale, -1);
}

void CloseGL::render() {
	int l = f_lst.size();
	reset_data();
	for (int i = 0; i < l; i++) {
		set_triangle(projection(v_lst[f_lst[i][0]]), projection(v_lst[f_lst[i][1]]), projection(v_lst[f_lst[i][2]]), default_color);
	}
}

void CloseGL::set_color_scheme(COLOR_SCHEME cs) {
	color_scheme = cs;
}

void CloseGL::set_projection(PROJECTION_METHOD pm) {
	projection_method = pm;
	reset_camera();
}

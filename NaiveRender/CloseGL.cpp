#include "CloseGL.h"
#include "constants.h"

#include <fstream>
#include <iostream>
#include <algorithm>


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
	cout << "Load Over" << endl;
	cout << "vertex num: " << v_lst.size() << "\n";
	cout << "face   num: " << f_lst.size() << "\n";
}

CloseGL::CloseGL() {
	eye = eye_init;
	up = up_init;
	data = new unsigned char[window_width * window_height * 3];
	reset_data();

	k_camera[0] = glm::vec3(100*5, 0, window_width/2);
	k_camera[1] = glm::vec3(0, 100*5, window_height/2);
	k_camera[2] = glm::vec3(0, 0, 1);
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
	if (i < 0 || i >= window_height || j < 0 || j >= window_width)
		return;
	int ind = (i * window_width + j) * 3;
	data[ind] = color[0];
	data[ind + 1] = color[1];
	data[ind + 2] = color[2];
}

void CloseGL::set_segment(int i, int s, int e, const unsigned char *color) {
	int sum = s + e;
	s = min(s, e);
	e = sum - s;
	for (int k = s; k <= e; k++)
		set_pixel(i, k, color);
}

void CloseGL::set_triangle(Point p1, Point p2, Point p3, const unsigned char *color) {
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

int CloseGL::intersect(Point p1, Point p2, int y) {
	return (double)(y - p1.y) / (double)(p2.y - p1.y) * (p2.x - p1.x) + p1.x;
}

void print(glm::vec3 p) {
	cout << "v " << p[0] << " " << p[1] << " " << p[2] << endl;
}

void print(Point p) {
	cout << "p " << p.y << " " << p.x << endl;
}

Point CloseGL::projection(glm::vec3 p) {
	glm::vec3 p_ = (p-eye) * k_camera;
	return Point((double)p_[1]/(double)p_[2], (double)p_[0]/(double)p_[2]);
}

void CloseGL::render() {
	int l = f_lst.size();
	for (int i = 0; i < l; i++) {
		set_triangle(projection(v_lst[f_lst[i][0]]), projection(v_lst[f_lst[i][1]]), projection(v_lst[f_lst[i][2]]), default_color);
	}
}

#include "ReadObj.h"

#include <fstream>
#include <iostream>

bool ReadObj::readvals(stringstream &ss, const int num, float *values) {
	char c;
	ss >> values[0];
	for (int i = 1; i < num; i++) {
		ss >> std::noskipws >> c >> values[i];
		if (ss.fail())
			return false;
	}
	return true;
}

void ReadObj::readfile(const char *filename) {
	v_lst.clear();
	f_lst.clear();

	string str, cmd;
	char *buffer = new char[100];
	float values[9];
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
			else if (cmd == "vn") {
				bool valid = readvals(ss, 3, values);
				if (valid)
					vn_lst.push_back(glm::vec3(values[0], values[1], values[2]));
			}
			else if (cmd == "vt") {
				bool valid = readvals(ss, 2, values);
				if (valid)
					vt_lst.push_back(glm::vec2(values[0], values[1]));
			}
			else if (cmd == "f") {
				if (vn_lst.size() > 0 && vt_lst.size() > 0) {
					bool valid = readvals(ss, 9, values);
					if (valid) {
						f_lst.push_back(glm::vec3(values[0] - 1, values[3] - 1, values[6] - 1));
						ft_lst.push_back(glm::vec3(values[1] - 1, values[4] - 1, values[7] - 1));
						fn_lst.push_back(glm::vec3(values[2] - 1, values[5] - 1, values[8] - 1));
					}
				}
				else if (vn_lst.size() > 0) {
					bool valid = readvals(ss, 6, values);
					if (valid) {
						f_lst.push_back(glm::vec3(values[0] - 1, values[2] - 1, values[4] - 1));
						ft_lst.push_back(glm::vec3(values[1] - 1, values[3] - 1, values[5] - 1));
					}
				}
				else {
					bool valid = readvals(ss, 3, values);
					if (valid)
						f_lst.push_back(glm::vec3(values[0] - 1, values[1] - 1, values[2] - 1));
				}

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

	cout << "Load Over" << endl;
	cout << "Radius: " << radius << "\n";
	cout << "vertex num: " << v_lst.size() << "\n";
	cout << "face   num: " << f_lst.size() << "\n";
}

ReadObj::ReadObj() {
}


ReadObj::~ReadObj() {
}

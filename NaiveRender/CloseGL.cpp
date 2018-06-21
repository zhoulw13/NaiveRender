#include "CloseGL.h"

#include <fstream>
#include <iostream>

bool CloseGL::readvals(stringstream &ss, const int num, float *values) {
	for (int i = 0; i < num; i++) {
		ss >> values[i];
		if (ss.fail())
			return false;
	}
	return true;
}

void CloseGL::readfile(const char *filename) {
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
					f_lst.push_back(face(values[0], values[1], values[2]));
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

void CloseGL::render() {

}
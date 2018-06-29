#pragma once

#include "glm/glm.hpp"

#include <sstream>
#include <vector>
using namespace std;

class ReadObj
{
public:
	vector<glm::vec3> v_lst;
	vector<glm::vec3> vn_lst;
	vector<glm::vec2> vt_lst;
	vector<glm::vec3> f_lst;
	vector<glm::vec3> fn_lst;
	vector<glm::vec3> ft_lst;
	float radius;

public:
	bool readvals(stringstream &ss, const int num, float *values);
	void readfile(const char *filename);

public:
	ReadObj();
	~ReadObj();
};


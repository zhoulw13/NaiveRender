#pragma once

#include "glm/glm.hpp"
#include <vector>
using namespace std;

struct HitInfo {
	glm::vec3 normal;
	glm::vec3 hitpos;
	glm::mat3 cc;
};

class RayTracing
{
private:
	glm::vec3 w, u, v;
	vector<vector<glm::vec3>*> f_lst;
	vector<vector<glm::vec3>*> v_lst;

	glm::mat4 rt;
	glm::mat3 sca;
	glm::mat4 rt_inv;
	glm::mat3 sca_inv;

public:
	RayTracing();
	~RayTracing();

	void addData(vector<glm::vec3> *f, vector<glm::vec3> *v);
	void setAxis(glm::vec3 &, glm::vec3 &);
	void setTransform(glm::mat3 &, glm::vec3 &, glm::mat3 &);
	glm::vec3 transform(glm::vec3 &);
	glm::vec3 inverse_transform(glm::vec3 &);

	glm::vec3 rayThruPixel(float , float );
	glm::vec3 intersection(glm::vec3 &, glm::vec3 &, HitInfo &);
	float intersect(glm::vec3 &, glm::vec3 &, int, int, HitInfo &);
	glm::vec3 findColor(glm::vec3 &, glm::vec3 &, HitInfo &, int);

	float det(glm::vec3 &, glm::vec3 &, glm::vec3 &);
};


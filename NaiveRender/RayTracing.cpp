#include "RayTracing.h"
#include "Constants.h"

#include <iostream>
#include <algorithm>


const float eps = 1e-6;
const float eps2 = 1e-3;
int sgn(float x) { return x > eps ? 1 : x < -eps ? 1 : 0; };

RayTracing::RayTracing() {

}

void RayTracing::setAxis(glm::vec3 &front, glm::vec3 &up) {
	w = glm::normalize(front);
	u = glm::normalize(glm::cross(w, up));
	v = glm::cross(w, u);
}

void RayTracing::addData(vector<glm::vec3> *f, vector<glm::vec3> *v) {
	f_lst.push_back(f);
	v_lst.push_back(v);
}

void RayTracing::setTransform(glm::mat3 &rot, glm::vec3 &tra, glm::mat3 &sca) {
	rt = glm::mat4(rot);
	rt[3] = glm::vec4(-rot * tra, 1);

	rt_inv = glm::transpose(glm::inverse(rt));
	sca_inv = glm::transpose(glm::inverse(sca));

	this->sca = sca;
}

RayTracing::~RayTracing() {
}

glm::vec3 RayTracing::rayThruPixel(float i, float j) {
	float alpha = tan(glm::radians(fovy / 2)) * (j - (window_width / 2)) / (window_width / 2) * window_width / window_height;
	float beta = tan(glm::radians(fovy / 2)) * (window_height / 2 - i) / (window_height / 2);
	glm::vec3 ret = glm::normalize(alpha*u + beta * v - w);
	return ret;
}

glm::vec3 RayTracing::intersection(glm::vec3 &origin, glm::vec3 &ray, HitInfo &info) {
	float min_dist = INT_MAX;

	int f_index = 0;
	int o_index = 0;

	HitInfo tmp;

	int os = f_lst.size();
	for (int i = 0; i < os; i++) {
		int fs = f_lst[i]->size();
		for (int j = 0; j < fs; j++) {
			float t = intersect(origin, ray, i, j, tmp);
			if (t > 0 && t < min_dist) {
				min_dist = t;
				f_index = j;
				o_index = i;
				info = tmp;
			}
		}
	}

	return glm::vec3(min_dist, f_index, o_index);
}

float RayTracing::intersect(glm::vec3 &origin, glm::vec3 &ray, int oi, int fi, HitInfo &info) {
	float re = INT_MAX;

	glm::vec3 tri = (*f_lst[oi])[fi];
	glm::vec3 a = transform((*v_lst[oi])[tri[0]]);
	glm::vec3 b = transform((*v_lst[oi])[tri[1]]);
	glm::vec3 c = transform((*v_lst[oi])[tri[2]]);

	glm::vec3 n = glm::normalize(glm::cross(c - a, b - a));
	if (glm::dot(n, ray) > 0)
		info.normal = -n;
	else
		info.normal = n;

	if (sgn(glm::dot(n, ray)) == 0)
		return INT_MAX;

	glm::vec3 e1 = a - b;
	glm::vec3 e2 = a - c;
	glm::vec3 s = a - origin;
	float x = det(ray, e1, e2);
	float beta = det(ray, s, e2)/x;
	float gamma = det(ray, e1, s)/x;

	// check validation
	if (beta < 0 || beta > 1 || gamma < 0 || gamma > 1 || (beta + gamma) > 1) {
		return INT_MAX;
	}
	else {
		re = det(s, e1, e2) / x;
		info.hitpos = origin + re * ray;
		glm::mat3 tmp = glm::mat3(
			a[0], b[0], c[0],
			a[1], b[1], c[1],
			1, 1, 1
		);
		glm::transpose(glm::inverse(tmp)) * info.hitpos;

	}

	return re;
}

glm::vec3 RayTracing::findColor(glm::vec3 &ray, glm::vec3 &hit, HitInfo &info, int depth) {
	if (hit[0] == INT_MAX || depth > max_depth) {
		return glm::vec3(0.0);
	}

	glm::vec3 color = ambient[(int)hit[2]];

	glm::vec3 real_hitpos = inverse_transform(info.hitpos);
	glm::vec3 eye_dir = -ray;
	glm::vec3 hire_normal = rt_inv * glm::vec4(-info.normal, 0.0);
	hire_normal = glm::normalize(hire_normal);
	
	//shadow
	glm::vec3 light = glm::normalize(point_light - real_hitpos);
	glm::vec3 hitpos = real_hitpos + light * eps2;
	HitInfo recur_info;
	glm::vec3 shadow = intersection(hitpos, light, recur_info);

	//distance
	float r = glm::length(point_light - real_hitpos);
	if (r < shadow[0]) {
		glm::vec3 halfvec = glm::normalize(eye_dir + light);

		float dDot = max(glm::dot(light, hire_normal), float(0.0));
		float hDot = pow(max(glm::dot(hire_normal, halfvec), float(0.0)), shininess);

		float atten_const = attenuation[0] + attenuation[1] * r + attenuation[2] * pow(r, 2);
		color += diffuse * point_light_color * dDot / atten_const;
		color += specular * point_light_color * hDot / atten_const;
	}

	//recursive ray tracing
	glm::vec3 ray2 = ray - 2 * glm::dot(ray, hire_normal) * hire_normal;
	glm::vec3 origin2 = real_hitpos + ray2 * eps2;
	HitInfo info2;
	glm::vec3 hit2 = intersection(origin2, ray2, info2);
	color += specular * findColor(ray2, hit2, info2, depth + 1);

	return color;
}

glm::vec3 RayTracing::transform(glm::vec3 &pos) {
	glm::vec4 p = rt * glm::vec4(pos*sca, 1) ;
	return -p;
}

glm::vec3 RayTracing::inverse_transform(glm::vec3 &pos) {
	glm::vec3 p = rt_inv * glm::vec4(-pos, 1);
	return p * sca_inv;
}

float RayTracing::det(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c) {
	glm::mat3 x = glm::mat3(a, b, c);
	return glm::determinant(x);
}
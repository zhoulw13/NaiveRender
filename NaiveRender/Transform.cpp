#include "Transform.h"

vec3 Transform::Transition(vec3 &eye, vec3& up, vec3 direction) {
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);

	mat3 x(u, v, w);
	return scale * direction * x;
	eye += direction * scale;
}

mat3 Transform::Rotate(float degree, vec3 axis) {
	mat3 ret;
	vec3 a = glm::normalize(axis);
	float sine = sin(glm::radians(degree)), cosine = cos(glm::radians(degree));
	float x = a.x, y = a.y, z = a.z;

	mat3 aat = mat3(x*x, y*x, z*x,
		x*y, y*y, z*y,
		x*z, y*z, z*z);

	mat3 aStar = mat3(0, -z, y,
		z, 0, -x,
		-y, x, 0);

	ret = glm::mat3(cosine) + (1 - cosine) * aat + sine * aStar; 
	return ret;
}

void Transform::Rotation(vec3 &eye, vec3 &up, int direction, ROTATE_TYPE type) {
	
	/*if (type == Z_ROTATE) {
		eye = eye * Rotate(direction * scale, up);
	}
	else if (type == X_ROTATE) {
		mat3 rot = Rotate(direction * scale, glm::cross(eye, up));
		eye = eye * rot;
		up = up * rot;
	}*/
}

mat4 Transform::LookAt(vec3 &eye, vec3 &up) {
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	mat4 r = mat4(u.x, u.y, u.z, 0,
		v.x, v.y, v.z, 0,
		w.x, w.y, w.z, 0,
		0, 0, 0, 1.0f);

	mat4 t = mat4(1, 0, 0, -glm::dot(u, eye),
		0, 1, 0, -glm::dot(v, eye),
		0, 0, 1, -glm::dot(w, eye),
		0, 0, 0, 1.0f);

	return glm::transpose(r*t);
}

Transform::Transform() {
	scale = 1;
}


Transform::~Transform() {
}

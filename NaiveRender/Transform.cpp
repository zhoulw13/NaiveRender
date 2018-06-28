#include "Transform.h"

vec3 Transform::Translation(vec3 &front, vec3& up, vec3 direction) {
	vec3 w = glm::normalize(front);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);

	mat3 x(u, v, w);
	return scale * x * direction;
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

mat3 Transform::Rotation(vec3 &front, vec3 &up, int direction, ROTATE_TYPE type) {
	mat3 ret;
	if (type == Z_ROTATE) {
		ret = Rotate(direction * scale, up);
		front = front * ret;
	}
	else if (type == X_ROTATE) {
		ret = Rotate(direction * scale, glm::cross(front, up));
		front = front * ret;
		up = up * ret;
	}
	return ret;
}

mat3 Transform::Scale(float &scale, int direction) {
	if (direction > 0)
		scale *= 1.1;
	else
		scale /= 1.1;
	return mat3(
		scale, 0, 0,
		0, scale, 0,
		0, 0, scale
	);
}

Transform::Transform() {
	scale = 1;
}


Transform::~Transform() {
}

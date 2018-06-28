#pragma once

#include <glm/glm.hpp>
const float pi = 3.141592654;

typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ;
typedef glm::vec3 vec3 ;
typedef glm::vec4 vec4 ;

enum ROTATE_TYPE {
	Z_ROTATE,
	X_ROTATE
};

class Transform {
private:
	float scale;
public:
	Transform();
	~Transform();

	vec3 Translation(vec3 &eye, vec3& up, vec3 direction);
	mat3 Rotation(vec3 &eye, vec3 &up, int direction, ROTATE_TYPE);
	mat3 Scale(float &scale, int direction);

	mat3 Rotate(float degree, vec3 axis);
};


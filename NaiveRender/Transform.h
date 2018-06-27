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

	vec3 Transition(vec3 &eye, vec3& up, vec3 direction);
	void Rotation(vec3 &eye, vec3 &up, int direction, ROTATE_TYPE);
	mat3 Rotate(float degree, vec3 axis);
	mat4 LookAt(vec3 &eye, vec3 &up);

};


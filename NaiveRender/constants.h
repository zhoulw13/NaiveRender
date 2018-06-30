#include <glm/glm.hpp>

const glm::vec3 eye_init(0.0, 0.0, 5.0);
const glm::vec3 up_init(0.0, 1.0, 0.0);
const float z_near = -99;
const float z_far = -0.1;
const float fovy = 30.0;

const int window_width = 640;
const int window_height = 480;
const unsigned char bg_color[3] = {0, 0, 0};

const int max_depth = 3;
const float shininess = 20;
const glm::vec3 diffuse(0.4, 0.4, 0.4);
const glm::vec3 ambient[2] = { glm::vec3(1.0, 0, 0),  glm::vec3(1.0, 0, 1.0) };
const glm::vec3 specular(0.4, 0.4, 0.4);

const float attenuation[3] = { 1, 0.001, 0.001 };

const glm::vec3 point_light(5.0, 5.0, 5.0);
const glm::vec3 point_light_color(1.0, 1.0, 1.0);
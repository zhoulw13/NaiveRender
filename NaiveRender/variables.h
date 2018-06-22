#include <glm/glm.hpp>

#ifdef MAIN
glm::vec3 eyeinit(5.0, 0.0, 0.0);
glm::vec3 upinit(0.0, 0.0, 1.0);
glm::vec3 lookinit(-1.0, 0.0, 0.0);
#else
extern glm::vec3 eyeinit;
extern glm::vec3 upinit;
extern glm::vec3 lookinit;
#endif


//EXTERN glm::vec3 eyeinit(5.0, 0.0, 0.0);
//EXTERN glm::vec3 upinit(0.0, 0.0, 1.0);
//EXTERN glm::vec3 lookinit(-1.0, 0.0, 0.0);
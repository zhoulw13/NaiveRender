#include <sstream>
#include <vector>
using namespace std;

#include "variables.h"

class CloseGL {
private:
	glm::vec3 eye, up;
	vector<glm::vec3> v_lst;
	vector<glm::vec3> f_lst;
public:
	CloseGL();
	bool readvals(stringstream &ss, const int num, float *values);
	void readfile(const char *filename);
public:
	void render();
};
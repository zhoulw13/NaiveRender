#include <sstream>
#include <vector>
using namespace std;

#include <glm/glm.hpp>

struct face {
	int v1, v2, v3;

	inline face(int a, int b, int c) : v1(a), v2(b), v3(c) {};
};

class CloseGL {
private:
	glm::vec3 eye, up;
	vector<glm::vec3> v_lst;
	vector<face> f_lst;
public:
	bool readvals(stringstream &ss, const int num, float *values);
	void readfile(const char *filename);
public:
	void render();
};
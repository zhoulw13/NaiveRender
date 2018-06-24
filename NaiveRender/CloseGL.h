#include <sstream>
#include <vector>
using namespace std;

#include <glm/glm.hpp>

typedef struct Point {
	int x, y;

	Point() {};
	Point(int y, int x) :x(x), y(y) {};
};


class CloseGL {
private:
	glm::vec3 eye, up;
	vector<glm::vec3> v_lst;
	vector<glm::vec3> f_lst;

	glm::mat3x3 k_camera;
public:
	unsigned char *data;

public:
	CloseGL();
	bool readvals(stringstream &ss, const int num, float *values);
	void readfile(const char *filename);

	unsigned char *get_data() { return data; };
	void reset_data();
	void set_pixel(int, int, const unsigned char *);
	void set_segment(int, int, int, const unsigned char *);
	void set_triangle(Point, Point, Point, const unsigned char *);
	int intersect(Point, Point, int);

	Point projection(glm::vec3);
	void render();
};
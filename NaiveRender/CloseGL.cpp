#include "CloseGL.h"
#include "constants.h"

#include <iostream>
#include <algorithm>
#include <omp.h>

void print(glm::vec3 p) {
	cout << "v " << p[0] << " " << p[1] << " " << p[2] << endl;
}

void print(glm::vec4 p) {
	cout << "v " << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << endl;
}

void print(glm::mat3 m) {
	print(m[0]);
	print(m[1]);
	print(m[2]);
}

void print(glm::mat4 m) {
	print(m[0]);
	print(m[1]);
	print(m[2]);
	print(m[3]);
}

void print(glm::mat2x3 m) {
	print(m[0]);
	print(m[1]);
}

void print(Point p) {
	cout << "p " << p.y << " " << p.x << endl;
}

CloseGL::CloseGL() {
	data = new unsigned char[window_width * window_height * 3];
	t = new Transform();
	color_scheme = SEGMENT;
	projection_method = PERSPECTIVE;
	ray_tracing = new RayTracing();
	ray_tracing_flag = false;
	texture = NULL;

	reset_data();
	reset_transform();
}

void CloseGL::readfile(const char *filename) {
	ReadObj *newobj = new ReadObj();
	newobj->readfile(filename);
	
	int l = obj.size();
	for (int i = 0; i < l; i++)
		obj[i]->shift(glm::vec3(-1.5, 0, 0));
	obj.push_back(newobj);

	reset_data();
	texture = NULL;
	ray_tracing_flag = false;
	reset_camera();
	reset_transform();
}

void CloseGL::loadtexture(unsigned char *t, int h, int w) {
	if (obj.size() > 0 && obj[0]->vt_lst.size() > 0 && obj[0]->ft_lst.size() > 0) {
		texture = t;
		texture_h = h;
		texture_w = w;
	}
}

void CloseGL::reset_transform() {
	eye = eye_init;
	up = glm::normalize(up_init);
	front = -glm::normalize(eye_init);
	scale = 1;

	rot = glm::mat3(1.0f);
	tra = eye;
	sca = glm::mat3(1.0f);
	ray_tracing->setTransform(rot, tra, sca);
}

void CloseGL::reset_camera() {
	if (projection_method == PERSPECTIVE) {
		k_camera = glm::mat3(
			window_height / 2 * glm::length(eye), 0, window_width / 2,
			0, window_height / 2 * glm::length(eye), window_height / 2,
			0, 0, 1);
	}
	else if (projection_method == ORTHOGONAL) {
		k_camera = glm::mat3(
			window_height / 2, 0, window_width / 2,
			0, window_height / 2, window_height / 2,
			0, 0, 1);
	}
}

void CloseGL::reset_data() {
	for (int i = 0; i < window_height; i++) {
		for (int j = 0; j < window_width; j++) {
			set_pixel(i, j, bg_color);
		}
	}
}

void CloseGL::set_pixel(int i, int j, const unsigned char *color) {
	if (in_range(Point(i, j))) {
		int ind = (i * window_width + j) * 3;
		data[ind] = color[0];
		data[ind + 1] = color[1];
		data[ind + 2] = color[2];
	}
}

bool CloseGL::in_range(Point p) {
	if (p.y < 0 || p.y >= window_height || p.x < 0 || p.x >= window_width)
		return false;
	return true;
}

//horizontal segment
void CloseGL::set_segment(int i, int s, int e, glm::mat3 &cc, glm::mat2x3 &tx, const unsigned char *color) {
	if (!in_range(Point(i, s)) && !in_range(Point(i, e)))
		return;
	int sum = s + e;
	s = min(s, e);
	e = sum - s;
	if (color_scheme == SEGMENT || texture == NULL) {
		for (int k = s; k <= e; k++)
			set_pixel(i, k, color);
	}
	else {
		glm::vec3 combine;
		for (int k = s; k <= e; k++) {
			combine = glm::vec3(k, i, 1) * cc;
			glm::vec2 tmp = combine * tx;
			//print(tx);
			//print(cc);
			//cout << "haha " << k << " " << i << endl;
			//print(combine);
			//cout << endl;

			//cout << tmp[0] << " " << tmp[1] << endl;
			if (combine[0] < 0 || combine[1] < 0 || combine[2] < 0)
				return;
			//cout << endl;
			int ti = tmp[1] * texture_h;
			int tj = tmp[0] * texture_w;
			//cout << ti << " " << tj << endl;
			set_pixel(i, k, &texture[(ti * texture_w + tj) * 3]);
		}
	}	
}

//arbitrary segment
void CloseGL::set_segment(Point p1, Point p2, const unsigned char *color) {
	if (!in_range(p1) && !in_range(p2))
		return;
	float k, e;
	int x, y, dx, dy, sig;

	if (p1.x == p2.x) {
		for (int i = min(p1.y, p2.y); i <= max(p1.y, p2.y); i++)
			set_pixel(i, p1.x, color);
		return;
	}

	k = float(p2.y - p1.y) / (p2.x - p1.x);
	if ((abs(k) < 1 && p1.x > p2.x)
		|| (abs(k) >= 1 && p1.y > p2.y)) {
		swap(p1, p2);
	}

	dx = p2.x - p1.x;
	dy = p2.y - p1.y;

	sig = (k >= 0) ? 1 : -1;
	e = -1 * sig*0.5;
	x = p1.x;
	y = p1.y;
	if (abs(k) < 1) {
		for (int i = 0; i < dx; i++) {
			set_pixel(y, x, color);
			x++;
			e += k;
			if (sig*e >= 0) {
				y += sig;
				e -= sig;
			}
		}
	}
	else {
		k = 1 / k;
		for (int i = 0; i < dy; i++) {
			set_pixel(y, x, color);
			y++;
			e += k;
			if (sig*e >= 0) {
				x += sig;
				e -= sig;
			}
		}
	}
}

void CloseGL::set_triangle(int o_id, int f_id, const unsigned char *color) {
	Point p[3];

	p[0] = projection(obj[o_id]->v_lst[obj[o_id]->f_lst[f_id][0]]);
	p[1] = projection(obj[o_id]->v_lst[obj[o_id]->f_lst[f_id][1]]);
	p[2] = projection(obj[o_id]->v_lst[obj[o_id]->f_lst[f_id][2]]);

	if (p[0].x == INT_MAX || p[1].x == INT_MAX || p[2].x == INT_MAX)
		return;

	if ((p[0].x - p[1].x) * (p[2].y - p[1].y) == (p[0].y - p[1].y) * (p[2].x - p[1].x))
		return;

	if (color_scheme == FACE) {
		if (!in_range(p[0]) && !in_range(p[1]) && !in_range(p[2]))
			return;

		p[0].id = 0;
		p[1].id = 1;
		p[2].id = 2;
		sort(p, p + 3, [](Point a, Point b) {return a.y < b.y; });
		
		glm::mat3 cc = glm::mat3(
			p[0].x, p[1].x, p[2].x,
			p[0].y, p[1].y, p[2].y,
			1, 1, 1
		);

		glm::mat2x3 tx;
		if (texture != NULL) {
			//print(cc);
			glm::vec2 vt1 = obj[o_id]->vt_lst[obj[o_id]->ft_lst[f_id][p[0].id]];
			glm::vec2 vt2 = obj[o_id]->vt_lst[obj[o_id]->ft_lst[f_id][p[1].id]];
			glm::vec2 vt3 = obj[o_id]->vt_lst[obj[o_id]->ft_lst[f_id][p[2].id]];
			tx = glm::mat2x3(
				vt1[0], vt2[0], vt3[0],
				vt1[1], vt2[1], vt3[1]
			);
		}

		cc = glm::inverse(cc);

		int s, e;
		if (p[0].y != p[1].y) {
			for (int i = p[0].y; i <= p[1].y; i++) {
				s = intersect(p[0], p[1], i);
				e = intersect(p[0], p[2], i);
				set_segment(i, s, e, cc, tx, color);
			}
		}
		if (p[2].y != p[1].y) {
			for (int i = p[2].y; i >= p[1].y; i--) {
				s = intersect(p[2], p[1], i);
				e = intersect(p[2], p[0], i);
				set_segment(i, s, e, cc, tx, color);
			}
		}
	}
	else if (color_scheme == SEGMENT) {
		set_segment(p[0], p[1], color);
		set_segment(p[0], p[2], color);
		set_segment(p[2], p[1], color);
	}
}

int CloseGL::intersect(Point p1, Point p2, int y) {
	return (double)(y - p1.y) / (double)(p2.y - p1.y) * (p2.x - p1.x) + p1.x;
}

Point CloseGL::projection(glm::vec3 p) {
	glm::vec4 ph = vec4(p*sca, 1);
	glm::mat4x3 x(rot);
	x[3] = -rot * tra;
	glm::vec3 p_ = x * ph * k_camera;
	if (p_[2] < z_near || p_[2] > z_far)
		return Point(INT_MAX, -1);
	if (projection_method == PERSPECTIVE)
		return Point(p_[1] / p_[2], p_[0] / p_[2]);
	else if (projection_method == ORTHOGONAL) {
		p_ = x * ph;
		p_[2] = -1;
		p_ = p_ * k_camera;
		return Point(-p_[1], -p_[0]);
	}
}

void CloseGL::camera_move(MOVE_EVENT event) {
	if (event == TRANSLATE_LEFT)
		tra += t->Translation(front, up, glm::vec3(-1, 0, 0));
	else if (event == TRANSLATE_RIGHT)
		tra += t->Translation(front, up, glm::vec3(1, 0, 0));
	else if (event == TRANSLATE_UP)
		tra += t->Translation(front, up, glm::vec3(0, 1, 0));
	else if (event == TRANSLATE_DOWN)
		tra += t->Translation(front, up, glm::vec3(0, -1, 0));
	else if (event == TRANSLATE_IN)
		tra += t->Translation(front, up, glm::vec3(0, 0, 1));
	else if (event == TRANSLATE_OUT)
		tra += t->Translation(front, up, glm::vec3(0, 0, -1));
	else if (event == ROTATE_LEFT)
		rot = rot * t->Rotation(front, up, -1, Z_ROTATE);
	else if (event == ROTATE_RIGHT)
		rot = rot * t->Rotation(front, up, 1, Z_ROTATE);
	else if (event == ROTATE_UP)
		rot = rot * t->Rotation(front, up, 1, X_ROTATE);
	else if (event == ROTATE_DOWN)
		rot = rot * t->Rotation(front, up, -1, X_ROTATE);
	else if (event == SCALE_OUT)
		sca = t->Scale(scale, 1);
	else if (event == SCALE_IN)
		sca = t->Scale(scale, -1);
	ray_tracing->setTransform(rot, tra, sca);
}

void CloseGL::render() {
	reset_data();
	if (!ray_tracing_flag) {
		int l = obj.size();
		for (int i = 0; i < l; i++) {

			unsigned char *color_ = new unsigned char[3];
			color_[0] = ambient[i][0] * 255;
			color_[1] = ambient[i][1] * 255;
			color_[2] = ambient[i][2] * 255;

			int d = obj[i]->f_lst.size();
			for (int j = 0; j < d; j++) {
				set_triangle(i, j, color_);
			}
		}
	}
	else {
		#pragma omp parallel for
		for (int i = 0; i < window_height; i++) {
			#pragma omp parallel for
			for (int j = 0; j < window_width; j++) {
				HitInfo info;
				glm::vec3 ray = ray_tracing->rayThruPixel(float(i) + 0.5, float(j) + 0.5);
				glm::vec3 hit = ray_tracing->intersection(vec3(0,0,0), ray, info);
				glm::vec3 color = ray_tracing->findColor(ray, hit, info, 1);
				unsigned char *color_ = new unsigned char[3];
				color_[0] = min(color[0], 1.0f) * 255;
				color_[1] = min(color[1], 1.0f) * 255;
				color_[2] = min(color[2], 1.0f) * 255;

				set_pixel(i, j, color_);
			}
		}
	}
}

void CloseGL::set_color_scheme(COLOR_SCHEME cs) {
	color_scheme = cs;
}

void CloseGL::set_projection(PROJECTION_METHOD pm) {
	projection_method = pm;
	reset_camera();
}

void CloseGL::set_ray_tracing() {
	ray_tracing_flag = true;
	ray_tracing->setAxis(front, up);
	ray_tracing->setTransform(rot, tra, sca);

	glm::mat4 rt = glm::mat4(rot);
	rt[3] = glm::vec4(-rot * tra, 1);
	print(rt);

	int l = obj.size();
	for (int i = 0; i < l; i++) {
		ray_tracing->addData(&obj[i]->f_lst, &obj[i]->v_lst);
	}
}

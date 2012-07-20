#ifndef __FSKLOADER_H_
#define __FSKLOADER_H_
#include <vector>
#include <cstdio>
#include "boost/multi_array.hpp"

typedef boost::multi_array<int, 2> ivec2d_type;
typedef ivec2d_type::index ivec2_index_type;

typedef struct _MODEL {
	std::vector<float> textures;
	ivec2d_type matlist;
	std::vector<float> materials;
	std::vector<float> verts;
	std::vector<float> texcoords;
	std::vector<float> triangles;
	int vert_offset;
}Model;

typedef struct _RGBA {
	int r,g,b,a;
}rgba_type;

class FSKloader {
	public:
		FSKloader(std::string);
		virtual ~FSKloader();

		Model* getModel();

	private:
		std::string filepath;
		Model* model;
		rgba_type rgba(int);
		bool check_if_MX_model(std::string);
	protected:
};

#endif

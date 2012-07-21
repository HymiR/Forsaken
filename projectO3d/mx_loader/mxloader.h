#ifndef __MXLOADER_H_
#define __MXLOADER_H_
#include <vector>
#include <cstdio>
#include "boost/multi_array.hpp"

typedef boost::multi_array<int, 2> ivec2d_type;
typedef ivec2d_type::index ivec2_index_type;

typedef struct _VERTEX {
	float x,y,z;
}Vertex;

typedef struct _TEXCOORDS {
	float tu, tv;
}Texcoords;

typedef struct _MATERIAL {
}Material;

typedef struct _MODEL {
	std::vector<float> textures;
	ivec2d_type matlist;
	std::vector<Material> materials;
	std::vector<Vertex> verts;
	std::vector<Texcoords> texcoords;
	std::vector<float> triangles;
	int vert_offset;
}Model;

typedef struct _RGBA {
	int r,g,b,a;
}rgba_type;

typedef struct _BYTES {
	short h;
	int i;
	unsigned int I;
	float v[3];
	std::string s;
}Bytes;

class MXloader {
	public:
		MXloader();
		virtual ~MXloader();

		Model* getModel();
		void load_new_model(std::string);
	
	private:
		FILE* modelfile;
		std::string filepath;
		Model* model;
		rgba_type rgba(int);
		bool check_if_MX_model(std::string);
		void readModel();
		void remove_old_model();
		Bytes getBytes(std::string);
	protected:
};

#endif

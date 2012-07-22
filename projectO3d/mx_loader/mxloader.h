#ifndef __MXLOADER_H_
#define __MXLOADER_H_
#include <vector>
#include <cstdio>
#include "boost/multi_array.hpp"

typedef boost::multi_array<unsigned int, 2> ivec2d_type;
typedef ivec2d_type::index ivec2_index_type;

typedef struct _VERTEX {
	float x,y,z;
}Vertex;

typedef struct _TEXCOORDS {
	float tu, tv;
}Texcoords;

typedef struct _TRIANGLE {
}Triangle;

typedef struct _TEXTURE {
	int width;
	int height;
	int flags;
	char *data; // maybe replace this with an Ogre texture type
}Texture;

typedef struct _MATERIAL { // maybe this struct can also be replaced with an Ogre type
	int flags;
	union
	{
		/* Simple colored material with alpha and specular value. */
		struct
		{
			float color[3];
			float specular[3];
			float alpha;
		} simple;
		/* Detailed material specification. */
		struct
		{
			float ambient[3];
			float diffuse[3];
			float specular[3];
			float emission[3];
			float shininess;
			float alpha;
		} full;
	};
}Material;

typedef struct _MODEL {
	std::vector<Texture> textures;
	ivec2d_type matlist;
	std::vector<Material> materials;
	std::vector<Vertex> verts;
	std::vector<Texcoords> texcoords;
	std::vector<Triangle> triangles;
	int vert_offset;
}Model;

typedef struct _RGBA {
	unsigned char r,g,b,a;
}rgba_type;

typedef struct _BYTES {
	std::vector<float> f;
	std::vector<short> h;
	std::vector<int> i;
	std::vector<unsigned int> I;
	std::vector<Vertex> v;
	std::vector<std::string> s;
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
		rgba_type rgba(unsigned int);
		Material getmat(unsigned int, unsigned int);
		void addmat(unsigned int, unsigned int, Material);
		Material material(unsigned int, unsigned int);
		Material gfx_material(int, rgba_type, rgba_type, int);
		bool check_if_MX_model(std::string);
		void readModel();
		void remove_old_model();
		Bytes getBytes(std::string);
	protected:
};

#endif

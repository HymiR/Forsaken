#ifndef __AUXILIARY_H_
#define __AUXILIARY_H_

typedef struct _VERTEX {
	float x,y,z;
}Vertex;

typedef struct _TEXCOORDS {
	float tu, tv;
}Texcoords;

typedef struct _TEXTURE {
	int width;
	int height;
	int flags;
	std::string file;
	char *data; // maybe replace this with an Ogre texture type
}Texture;

typedef struct _TRIANGLE {
	short v0, v1, v2;
	Vertex normal;
}Triangle;

typedef struct _RGBA {
	unsigned char r,g,b,a;
}rgba_type;

typedef struct _MATERIAL {
	rgba_type color, specular;
}Material;

typedef struct _BYTES {
	std::vector<float> f;
	std::vector<short> h;
	std::vector<int> i;
	std::vector<unsigned int> I;
	std::vector<Vertex> v;
	std::vector<std::string> s;
}Bytes;

#endif

#ifndef __MXCONVERTER_H_
#define __MXCONVERTER_H_
#include <vector>
#include <cstdio>
#include "auxiliary.h"

typedef struct _MODEL {
	std::vector<Texture> textures;
	std::vector<Material> materials;
	std::vector<Vertex> verts;
	std::vector<Texcoords> texcoords;
	std::vector<Triangle> triangles;
	int vert_offset;
}Model;


class MXconverter {
	public:
		MXconverter();
		virtual ~MXconverter();

		Model* getModel();
		void load_new_model(std::string);
	
	private:
		FILE* modelfile;
		std::string filepath;
		Model* model;
		rgba_type rgba(unsigned int);
		bool check_if_MX_model();
		void readModel();
		void remove_old_model();
		Bytes getBytes(std::string);

		template<class T>
		void reverse_byte_order(T&, size_t);
	protected:
};

#endif

#include <iostream>
#include <string>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "include/mxconverter.h"

MXconverter::MXconverter()
{
	this->modelfile = NULL;
	this->model = NULL;
}

MXconverter::~MXconverter()
{
	remove_old_model();
}

void MXconverter::load_new_model(std::string filepath)
{
	remove_old_model();
	this->filepath = filepath;
	this->model = new Model();

	if(check_if_MX_model()) {
		std::cout << "We have a valid mx file\n";
		this->modelfile = fopen(filepath.c_str(), "rb");
	} else {
		std::cout << "We don't have a valid mx file\n";
		return;
	}
	this->modelfile = fopen(this->filepath.c_str(), "rb");
	readModel();
	fclose(this->modelfile);
	this->modelfile = NULL;
}

void MXconverter::remove_old_model()
{
	if(this->model){
		delete this->model;
		this->model = NULL;
	}

	if(this->modelfile) {
		fclose(this->modelfile);
		this->modelfile = NULL;
	}

	this->filepath = "";
}

rgba_type MXconverter::rgba(unsigned int n)
{
	rgba_type rgba_s;
	rgba_s.b = (unsigned char)(n & 0xff) / 255;
	n = n >> 8;
	rgba_s.g = (unsigned char)(n & 0xff) / 255;
	n = n >> 8;
	rgba_s.r = (unsigned char)(n & 0xff) / 255;
	n = n >> 8;
	rgba_s.a = (unsigned char)(n & 0xff) / 255;
	n = n >> 8;
	return rgba_s;
}


bool MXconverter::check_if_MX_model()
{
	int X = 0;
	int Y = 0x50524a58; // the magic number
	this->modelfile = fopen(this->filepath.c_str(),"rb");
	if (this->modelfile != NULL) {
		Bytes by = getBytes("ii");
		X = by.i[0];
		int ver = by.i[1];
		std::cout << "Version: " << std::hex << ver/0x1000000 << std::endl;
		fclose(this->modelfile);
		this->modelfile = NULL;
		if(X == Y)
			return true;
	} else {
		std::cout << "Unable to open model file\n";
	}

	return false;
}

Model* MXconverter::getModel()
{
	return this->model;
}

/**
 * This function will read in the model
 * file and put the data in our Model
 * structure.
 */
void MXconverter::readModel()
{
	size_t groupsize, execsize, vertexsize, texgroupsize, trianglesize, texfilesize = 0;
	getBytes("ii"); // jump over magic number
	texfilesize = (size_t)getBytes("h").h[0];
	printf("we have %d textures\n", texfilesize);
	for(size_t txti = 0; txti < texfilesize; txti++) { // for all texture file names
		Bytes bytxt = getBytes("z");
		Texture tex;
		std::string fb = bytxt.s[txti];
		boost::algorithm::to_lower(fb); // our filenames are all lowercase
		tex.file = fb;
		this->model->textures.push_back(tex);
		std::cout << "Texture file: " << tex.file << "\n";
	}
	groupsize = (size_t)getBytes("h").h[0];
	printf("we have %d groups\n", groupsize);
	for(size_t i = 0; i < groupsize; i++) { // for each group
		int verts_in_this_group = 0;
		execsize = (size_t)getBytes("h").h[0];
		printf("we have %d exec lists\n", execsize);
		for(size_t j = 0; j < execsize; j++) { // for each execlist
			printf("Exectype is: %x\n", getBytes("i").i[0]); // we don't need the exec_type
			vertexsize = (size_t)getBytes("h").h[0];
			printf("we have %d vertices in group %d\n", vertexsize, i);
			for(size_t k = 0; k < vertexsize; k++) { // for each vertex
				// here we read the bytes
				Bytes byv = getBytes("vIIIff"); // the first 'I' value is crap, we can throw it away
				this->model->verts.push_back(byv.v[0]);
				Material mat;
				mat.color = rgba(byv.I[1]); mat.specular = rgba(byv.I[2]);
				this->model->materials.push_back(mat);
				Texcoords t;
				t.tu = byv.f[0]; t.tv = byv.f[1];
				this->model->texcoords.push_back(t);
				verts_in_this_group++;
			}

			texgroupsize = (size_t)getBytes("h").h[0];
			printf("we have %d texture groups\n", texgroupsize);
			for(size_t tg = 0; tg < texgroupsize; tg++) { // for each texture group
				Bytes byt = getBytes("hhhh");
				trianglesize = (size_t)getBytes("h").h[0];
				printf("we have %d triangles in texture group %d\n", trianglesize, tg);
				for(size_t tgt = 0; tgt < trianglesize; tgt++) { // for each triangle
					Bytes bytv = getBytes("hhhhv"); // the 4th short can be omitted
					Triangle tri;
					tri.v0 = bytv.h[0]; tri.v1 = bytv.h[1]; tri.v2 = bytv.h[2];
					tri.normal = bytv.v[0];
					this->model->triangles.push_back(tri);
				}
			}
		}
		this->model->vert_offset += verts_in_this_group;
	}
	printf("read the model\n");
}

/**
 * This function accepts a string containing a
 * mask which tells the function how much bytes it
 * should read and what format they have.
 *
 * Format characters:
 *   f = 4-byte floating-point value (float)
 *   h = signed 2-byte value (short)
 *   i = signed 4-byte value (int)
 *   I = unsigned 4-byte value (int)
 *   v = vector (3 floats totaling 12 bytes)
 *   z = null-terminated string
 */
Bytes MXconverter::getBytes(std::string bytemask)
{
	if(this->modelfile == NULL) {
		std::cout << "Error: no modelfile loaded while trying to read bytes!\n";
		exit(1);
	}

	Bytes by;
	float f = 0.0f;
	short h = 0;
	int i = 0;
	unsigned int I = 0;
	float buff_v[3] = {0.0f, 0.0f, 0.0f};
	Vertex v = {0.0f, 0.0f, 0.0f};
	std::string s;
	unsigned int pseudofloat = 0;

	for(size_t it = 0; it < bytemask.length(); it++) {
		if(bytemask[it] == 'f') {
			fread(&pseudofloat, 1, 4, this->modelfile);
			reverse_byte_order(pseudofloat, sizeof(pseudofloat));
			f = *(float*)&pseudofloat;
			by.f.push_back(f);
		} else if(bytemask[it] == 'h') { // read signed 2-byte (short)
			fread(&h, 1, 2, this->modelfile);
			by.h.push_back(h);
		} else if(bytemask[it] == 'i') { // read signed 4-byte (int)
			fread(&i, 1, 4, this->modelfile);
			reverse_byte_order(i, sizeof(i));
			by.i.push_back(i);
		} else if(bytemask[it] == 'I') { // read unsigned 4-byte (uint)
			fread(&I, 1, 4, this->modelfile);
			reverse_byte_order(I, sizeof(I));
			by.I.push_back(I);
		} else if(bytemask[it] == 'v') { // read 12-byte (3*4-byte, float)
			for(int j = 0; j < 3; j++) {
				fread(&pseudofloat, 1, 4, this->modelfile);
				reverse_byte_order(pseudofloat, sizeof(pseudofloat));
				buff_v[j] = *(float*)&pseudofloat;
			}
			v.x = buff_v[0]; v.y = buff_v[1]; v.z = buff_v[2];
			by.v.push_back(v);
		} else if(bytemask[it] == 'z') { // read null-terminated string
			char c = -1;
			while(c != '\0') {
				fread(&c, 1, 1, this->modelfile);
				s += c;
			}
			//fread(&c, 1, 1, this->modelfile); // jump 1 byte
			by.s.push_back(s);
		} else {
			std::cout << "Wrong format character!\n";
		}
	}
	return by;
}

/**
 * Reverse the endianess of the given value (bytes), numbytes
 * is the size of the value in bytes
 */
template<class T>
void MXconverter::reverse_byte_order(T& bytes, size_t numbytes)
{
	unsigned char buffbyte = 0;
	T buffbytes = 0;
	if(numbytes > 1) {
		for(size_t i = 0; i < numbytes; i++) {
			buffbyte = (unsigned char)bytes & 0xff;
			bytes = bytes >> 8;
			buffbytes |= buffbyte;
			if(i < numbytes-1)
				buffbytes = buffbytes << 8;
		}
		bytes = buffbytes;
	}
}

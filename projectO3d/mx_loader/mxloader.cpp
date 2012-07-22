#include <iostream>
#include <cstring>
#include "mxloader.h"

MXloader::MXloader()
{
	this->modelfile = NULL;
	this->model = NULL;
}

MXloader::~MXloader()
{
	remove_old_model();
}

void MXloader::load_new_model(std::string filepath)
{
	remove_old_model();
	this->filepath = filepath;

	if(check_if_MX_model(filepath)) {
		std::cout << "We have a valid mx file\n";
		this->modelfile = fopen(filepath.c_str(), "rb");
	} else {
		std::cout << "We don't have a valid mx file\n";
	}
}

void MXloader::remove_old_model()
{
	if(this->model){
		delete this->model;
		this->model = NULL;
	}

	if(this->modelfile) {
		fclose(this->modelfile);
		this->modelfile = NULL;
	}
}

rgba_type MXloader::rgba(int n)
{
	rgba_type rgba_s;
	rgba_s.b = (n & 0xff) / 255;
	n = n >> 8;
	rgba_s.g = (n & 0xff) / 255;
	n = n >> 8;
	rgba_s.r = (n & 0xff) / 255;
	n = n >> 8;
	rgba_s.a = (n & 0xff) / 255;
	n = n >> 8;
	return rgba_s;
}

bool MXloader::check_if_MX_model(std::string file)
{
	FILE* fp = NULL;
	unsigned char X[4] = {0};
	unsigned char Y[4] = {0x50, 0x52, 0x4a, 0x58}; // the magic number
	fp = fopen(file.c_str(),"rb");
	if (fp != NULL) {
		fread(&X, 4, 1, fp); // we only want the 4 first bytes
		fclose(fp);
		if(memcmp(Y, X, 4) == 0)
			return true;
	} else {
		std::cout << "Unable to open model file\n";
	}

	return false;
}

Model* MXloader::getModel()
{
	return this->model;
}

/**
 * This function will read in the model
 * file and put the data in out Model
 * structure.
 */
void MXloader::readModel()
{
	for(size_t i = 1; i < (size_t)getBytes("h").h[0]; i++) { // for each group
		int verts_in_this_group = 0;
		for(size_t j = 1; j < (size_t)getBytes("h").h[0]; j++) { // for each execlist
			getBytes("i"); // we don't need the exec_type
			for(size_t k = 1; k < (size_t)getBytes("h").h[0]; k++) { // for each vertex
				// here we read the bytes
				Bytes by = getBytes("vIIIff");
				/*
				Vertex v;
				for(int vi = 0; vi < 3; vi++)
					v[vi] = by.v[vi];
				this->model->verts.push_back(v);
				//TODO: get, calculate, push_back Materials
				Texcoords t;
				*/
			}
			for(;;) { // for each texture group
				for(;;) { // for each triangle
				
				}
			}
		}
	}
}

/**
 * This function accepts a string containing a
 * mask which tells the function how much bytes it
 * should read and what format they have.
 */
Bytes MXloader::getBytes(std::string bytemask)
{
	Bytes by;
	float f = 0.0f;
	short h = 0;
	int i = 0;
	unsigned int I = 0;
	float buff_v[3] = {0.0f, 0.0f, 0.0f};
	Vertex v = {0.0f, 0.0f, 0.0f};
	std::string s;

	for(size_t i = 0; i < bytemask.length(); i++) {
		if(bytemask[i] == 'f') {
			fread(&f, 1, 4, this->modelfile);
			by.f.push_back(f);
		} else if(bytemask[i] == 'h') { // read signed 2-byte (short)
			fread(&h, 1, 2, this->modelfile);
			by.h.push_back(h);
		} else if(bytemask[i] == 'i') { // read signed 4-byte (int)
			fread(&i, 1, 4, this->modelfile);
			by.i.push_back(i);
		} else if(bytemask[i] == 'I') { // read unsigned 4-byte (uint)
			fread(&I, 1, 4, this->modelfile);
			by.I.push_back(I);
		} else if(bytemask[i] == 'v') { // read 12-byte (3*4-byte, float)
			for(int j = 0; j < 3; j++) {
				fread(&buff_v[j], 1, 4, this->modelfile);
			}
			v.x = buff_v[0]; v.y = buff_v[1]; v.z = buff_v[2];
			by.v.push_back(v);
		} else if(bytemask[i] == 'z') { // read null-terminated string
			char c = 0;
			while(c != '\0') {
				fread(&c, 1, 1, this->modelfile);
				s += c;
			}
			by.s.push_back(s);
		} else {
			std::cout << "Wrong format character!\n";
		}
	}
	return by;
}


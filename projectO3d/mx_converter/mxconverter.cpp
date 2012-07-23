#include <iostream>
#include <cstring>
#include "mxconverter.h"

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

	if(check_if_MX_model(filepath)) {
		std::cout << "We have a valid mx file\n";
		this->modelfile = fopen(filepath.c_str(), "rb");
	} else {
		std::cout << "We don't have a valid mx file\n";
	}
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

Material MXconverter::getmat(unsigned int color, unsigned int specular)
{
	Material mat;
	// TODO: implement this
	return mat;
}

void MXconverter::addmat(unsigned int color, unsigned int specular, Material mat)
{
	// TODO: implement this
}

Material MXconverter::material(unsigned int color, unsigned int specular)
{
	Material mat;
	rgba_type rgba_col = rgba(color);
	rgba_type rgba_spe = rgba(specular | (255 << 24));
	mat = getmat(color, specular);
	//if(mat != NULL)
	//	return mat;

	mat = gfx_material(0, rgba_col, rgba_spe, 1);

	addmat(color, specular, mat);

	return mat;
}

Material MXconverter::gfx_material(int flags, rgba_type color, rgba_type specular, int alpha)
{
	Material mat;
	// TODO: implement this
	return mat;
}

Triangle MXconverter::face(short vx, short vy, short vz, Vertex normal, Texture texture)
{
	Triangle tri;
	short v0 = vx + this->model->vert_offset;
	short v1 = vy + this->model->vert_offset;
	short v2 = vz + this->model->vert_offset;

	tri.fvert[0].index = v0; tri.fvert[0].txc = this->model->texcoords[v0]; this->model->materials[v0];
	tri.fvert[1].index = v1; tri.fvert[0].txc = this->model->texcoords[v1]; this->model->materials[v1];
	tri.fvert[2].index = v2; tri.fvert[0].txc = this->model->texcoords[v2]; this->model->materials[v2];
	tri.normal = normal;
	tri.texture = texture;

	return tri;
}

bool MXconverter::check_if_MX_model(std::string file)
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
	for(size_t txti = 0; txti < (size_t)getBytes("h").h[0]; txti++) { // for all texture file names
		Bytes bytxt = getBytes("z");
		// TODO: implement texture function and call it here (put result into model)
	}

	for(size_t i = 0; i < (size_t)getBytes("h").h[0]; i++) { // for each group
		int verts_in_this_group = 0;
		for(size_t j = 0; j < (size_t)getBytes("h").h[0]; j++) { // for each execlist
			getBytes("i"); // we don't need the exec_type
			for(size_t k = 0; k < (size_t)getBytes("h").h[0]; k++) { // for each vertex
				// here we read the bytes
				Bytes byv = getBytes("vIIIff"); // the first 'I' value is crap, we can throw it away
				this->model->verts.push_back(byv.v[0]);
				this->model->materials.push_back(material(byv.I[1], byv.I[2]));
				Texcoords t;
				t.tu = byv.f[0]; t.tv = byv.f[1];
				this->model->texcoords.push_back(t);
				verts_in_this_group++;
			}
			for(size_t tg = 0; tg < (size_t)getBytes("h").h[0]; tg++) { // for each texture group
				Bytes byt = getBytes("hhhh");
				for(size_t tgt = 0; tgt < (size_t)getBytes("h").h[0]; tgt++) { // for each triangle
					Bytes bytv = getBytes("hhhhv"); // the 4th short can be omitted
					this->model->triangles.push_back(face(bytv.h[0], bytv.h[1], bytv.h[2], bytv.v[0], this->model->textures[byt.h[3]]));
				}
			}
		}
		this->model->vert_offset += verts_in_this_group;
	}
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


#include <iostream>
#include <cstring>
#include "mxloader.h"

MXloader::MXloader( std::string filepath )
{
	this->filepath = filepath;
	if(check_if_MX_model(this->filepath)) {
		std::cout << "We have a valid mx file\n";
	} else {
		std::cout << "We don't have a valid mx file\n";
	}
}

MXloader::~MXloader() {
	if(this->model){
		delete this->model;
		this->model = NULL;
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

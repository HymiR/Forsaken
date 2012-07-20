#include "fskloader.h"

FSKloader::FSKloader( std::string filepath )
{
	this->filepath = filepath;
}

FSKloader::~FSKloader() {
	if(this->model){
		delete this->model;
		this->model = NULL;
	}
}

rgba_type FSKloader::rgba(int n)
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

bool FSKloader::check_if_MX_model(std::string file)
{

	return false;
}

Model* FSKloader::getModel()
{

	return this->model;
}

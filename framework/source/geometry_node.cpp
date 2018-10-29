#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "geometry_node.hpp"

	geometry_node::geometry_node():node(){}	
	geometry_node::geometry_node(std::string name):node{name}{}
	geometry_node::~geometry_node(){}

	//get-set for geometry model
	model* geometry_node::getGeometry(){return geometry;}
	void geometry_node::setGeometry(model& newgeometry){
		geometry = &newgeometry;
	}


#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include <map>
#include <vector>
#include <glbinding/gl/gl.h>

// use gl definitions from glbinding 
using namespace gl;

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


class geometry_node: public node{
 public:
	geometry_node();	
	geometry_node(std::string name);
	~geometry_node();

	//get-set for geometry model
	model* getGeometry() const;
	void setGeometry(model& newgeometry);

 protected:
	model* geometry;
};
#endif
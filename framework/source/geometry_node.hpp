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


class geometry_node:node{
 public:
	geometry_node();
	~geometry_node();
	//return perspective
	bool getPerspective();
	//get-set if enabled
	bool getEnabled();
	void setEnabled(bool b);
	//get-set PojectionMatrix
	glm::fmat4 getProjectionMatrix();
	void setProjectionMatrix(glm::fmat4 newprojection);
	
 protected:
 	bool isPerspective;
 	bool isEnabled
 	glm::fmat4 projectionMatrix;
};
#endif
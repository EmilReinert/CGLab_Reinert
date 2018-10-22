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
#include "node.hpp"

	node::node(){}
	node::~node(){}
	//get-set parent node
	node node::getParent()const{}
	void node::setParent(node const& newparent){}
	//returns child with specific name
	node node::getChild(std::string child_name){}
	//returns vector of all children
	std::vector<node> node::getChildren(){}
	//getters
	std::string node::getName(){}
	std::string node::getPath(){}
	int node::getDepth(){}
	//get-set for Transformation
	glm::fmat4 node::getLocalTransform(){}
	glm::fmat4 node::getWorldTransform(){}
	void node::setLocalTransform(glm::fmat4 trans_mat){}
	void node::setWorldTransform(glm::fmat4 trans_mat){}
	//adding children
	void node::addChild(node const& newchild){}
	//removing children
	void node::removeChild(std::string name){}


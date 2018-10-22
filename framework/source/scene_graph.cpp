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
#include "scene_graph.hpp"
	scene_graph::scene_graph(){}
	scene_graph::~scene_graph(){}
	//get-set string name of scene graph
	std::string scene_graph::getName(){}
	void scene_graph::setName(std::string newname){}
	//get-set root node
	node scene_graph::getRoot() const{}
	void scene_graph::setRoot(node const& newroot){}
	//prints names of all children in graph
	void scene_graph::printGraph(){}
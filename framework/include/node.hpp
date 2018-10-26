#ifndef NODE_HPP
#define NODE_HPP

#include <map>
#include <vector>
#include "window_handler.hpp"
#include "structs.hpp"
#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>

// use gl definitions from glbinding 
using namespace gl;

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class node{
 public:
	node();
	node(std::string name);
	virtual ~node();
	//get-set parent node
	node* getParent()const;
	void setParent(node& newparent);
	//returns child with specific name
	node* getChild(std::string child_name);
	//returns vector of all children
	std::vector<node*> getChildren();
	//getters
	std::string getName();
	std::string getPath();
	int getDepth();
	//get-set for Transformation
	glm::fmat4 getLocalTransform();
	glm::fmat4 getWorldTransform();
	void setLocalTransform(glm::fmat4 trans_mat);
	void setWorldTransform(glm::fmat4 trans_mat);
	//adding children
	void addChild(node& newchild);
	//removing children
	void removeChild(std::string name);
	//recursive breath tree analysis
	void printGraph();

 	std::vector<node*> children;
 protected: 	
 	node* parent; 	
 	std::string name;
 	std::string path;
 	int depth=0;
 	glm::fmat4 localTransform;
 	glm::fmat4 worldTransform;

};
#endif
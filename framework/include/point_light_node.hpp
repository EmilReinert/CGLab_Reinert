#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

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


class point_light_node: public node{
 public:
	point_light_node();	
	point_light_node(std::string name);
	~point_light_node();

	//get-set for color and intensity
	glm::vec3 getColor() const;
	void setColor(glm::vec3 c);
	float getIntensity() const;
	void setIntensity(float f);
	//prints attributes of lightnode
	void print();

 protected:
	glm::vec3 color;
	float intensity;
};
#endif
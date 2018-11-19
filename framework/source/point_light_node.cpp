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

#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "point_light_node.hpp"

	point_light_node::point_light_node():node(){}	
	point_light_node::point_light_node(std::string name):node{name}{}
	point_light_node::~point_light_node(){}

	//get-set for color and intensity
	glm::vec3 point_light_node::getColor() const{
		return color;
	}
	void point_light_node::setColor(glm::vec3 c){
		color = c;
	}
	float point_light_node::getIntensity() const{
		return intensity;
	}
	void point_light_node::setIntensity(float f){
		intensity = f;
	}
	//prints attributes of lightnode
	void point_light_node::print(){
		std::cout<<"Light "<<name<<": Color = "<<glm::to_string(color)<<", Intensity = "<<intensity<<std::endl;
	}
	


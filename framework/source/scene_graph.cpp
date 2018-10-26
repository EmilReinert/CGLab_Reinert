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
	scene_graph::scene_graph(std::string name):m_name{name}{}
	scene_graph::~scene_graph(){}


	//get-set string name of scene graph
	std::string scene_graph::getName(){return m_name;}
	void scene_graph::setName(std::string newname){
		m_name = newname;
	}
	//get-set root node
	node* scene_graph::getRoot()const{return m_root_node;}
	void scene_graph::setRoot(node& newroot){
		m_root_node = &newroot;
	}
	//prints names of all children in graph
	void scene_graph::printGraph(){
		if(m_root_node!=NULL){
			printf("----------------printing Graph----------------\n");
			std::cout<<m_root_node->getName()<<std::endl;
			m_root_node->printGraph();
			printf("----------------------------------------------\n");
		}
		else{
			std::cout<<"there is no graph to print"<<std::endl;
		}
	}
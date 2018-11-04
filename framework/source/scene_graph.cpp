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
	scene_graph::scene_graph(){rootSetup();}
	scene_graph::scene_graph(std::string name):m_name{name}{rootSetup();}
	scene_graph::~scene_graph(){}


	//get-set string name of scene graph
	std::string scene_graph::getName(){return m_name;}
	void scene_graph::setName(std::string newname){
		m_name = newname;
	}
	//get-set root node
	node* scene_graph::getRoot()const{return m_root_node;}
	void scene_graph::setRoot(node & newroot){
		m_root_node = &newroot;
	}
	//prints names of all children in graph
	void scene_graph::printGraph()const{
		if(m_root_node!=NULL){
			std::cout<<"----------------printing: "<< m_root_node->getName()<< "----------------\n";
			m_root_node->printGraph();
			printf("----------------------------------------------\n");
		}
		else{
			std::cout<<"there is no graph to print"<<std::endl;
		}
	}
	void scene_graph::rootSetup(){

		//creating singualar nodes
	  node system = node{"solarSystem"};

	  geometry_node sun = geometry_node{"sun"};
	  
	  geometry_node mercury = geometry_node{"mercury"};
	  mercury.setDistance(2.0f);
	  mercury.setSize(0.3f);
	  //mercury.speed = ;
	  geometry_node venus = geometry_node{"venus"};
	  venus.setDistance(3.0f);
	  venus.setSize(0.4f);
	  //venus.speed = ;
	  geometry_node earth = geometry_node{"earth"};
	  earth.setDistance(4.0f);
	  earth.setSize(0.5f);
	  //earth.speed = ;
	  geometry_node mars = geometry_node{"mars"};
	  mars.setDistance(5.0f);
	  mars.setSize(0.4f);
	  //mars.speed = ;
	  geometry_node jupiter = geometry_node{"jupiter"};
	  jupiter.setDistance(6.0f);
	  jupiter.setSize(2.0f);
	  //jupiter.speed = ;
	  geometry_node saturn = geometry_node{"saturn"};
	  saturn.setDistance(7.0f);
	  saturn.setSize(1.0f);
	  //saturn.speed = ;
	  geometry_node uranus = geometry_node{"uranus"};
	  uranus.setDistance(8.0f);
	  uranus.setSize(1.5f);
	  //uranus.speed = ;
	  geometry_node neptune = geometry_node{"neptune"};
	  neptune.setDistance(9.0f);
	  neptune.setSize(1.2f);
	  //neptune.speed = ;
	  geometry_node pluto = geometry_node{"pluto"};
	  pluto.setDistance(10.0f);
	  pluto.setSize(0.2f);
	  //pluto.speed = ;


	  geometry_node j_moon_1 = geometry_node{"jupiterMoon1"}; 
	  geometry_node j_moon_2 = geometry_node{"jupiterMoon2"};
	  geometry_node e_moon = geometry_node{"earthMoon"};
	  /////////creating hirarchie//////////////
	  system.addChild(sun);
	  system.addChild(mercury);
	  system.addChild(venus);
	  system.addChild(earth);
	  system.addChild(mars);
	  system.addChild(jupiter);
	  system.addChild(saturn);
	  system.addChild(uranus);
	  system.addChild(neptune);
	  system.addChild(pluto);
	  jupiter.addChild(j_moon_1); 
	  jupiter.addChild(j_moon_2);
	  earth.addChild(e_moon);

	  system.printGraph();
	  setRoot(system);
	  std::cout<<"Scene set up"<<std::endl;


	}
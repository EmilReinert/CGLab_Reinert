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

	node::node():
		localTransform{glm::fmat4()}
		,worldTransform{glm::fmat4()}{}
	node::node(std::string name):
		name{name}
		,localTransform{glm::fmat4()}
		,worldTransform{glm::fmat4()}{}
	node::~node(){}

//get-set parent node
	node* node::getParent()const{return parent;}

	void node::setParent(node & newparent){
		//sets new parent		
		parent = &newparent;
		//assigns child to new parent
		newparent.children.push_back(this);
	}

//returns child with specific name
	node* node::getChild(std::string child_name){
		for (node* child: children){
			if(!child->getName().compare(child_name)){
				return child;
			}
		}
		printf("no child with given name\n");
		return NULL;
	}
//returns vector of all children
	std::vector<node*> node::getChildren(){return children;}
//getters
	std::string node::getName()const{return name;}
	//path calculated by looking at earlier parents	
	std::string node::getPath(){
		//calculating and assigning path
		std::cout<<"pathcheck";
		if(parent!=NULL)			
			path = parent->getPath()+" "+name;	
		else
			path = name;
		return path;
	}
	//depth calculated by looking at earlier parents
	int node::getDepth(){
		//calculating and assigning depth
		if(parent!=NULL)
			depth = parent->getDepth()+1;
		else
			depth = 0;
		//returnign
		return depth;
	}

//get-set for Transformation
	//return local transfomation matrix
	glm::fmat4 node::getLocalTransform(){return localTransform;}
	//calculates and assigns world transfomation first
	glm::fmat4 node::getWorldTransform(){
		//calculate worldTransfromation
		std::cout<<"hi";
		glm::fmat4 worldholder;
		if(parent== NULL){
			std::cout<<"hi1";
			return worldTransform = getLocalTransform();
		}
		else{
			std::cout<<"hi2";
			return worldTransform = getLocalTransform()*parent->getWorldTransform();
		}
		//assign worldtransformation
		std::cout<<"hi3";
		//return 
		return worldTransform;
	}
	//setters for transformation
	void node::setLocalTransform(glm::fmat4 trans_mat){
		localTransform = trans_mat;
	}
	void node::setWorldTransform(glm::fmat4 trans_mat){
		worldTransform = trans_mat;
	}

//adding children
	void node::addChild(node& newchild){
		//calling setparent on the new child
		newchild.setParent(*this);
	}
//removing children
	void node::removeChild(std::string name){
		int counter =0;
		for (node* child: children){
			if(!child->getName().compare(name)){
				children.erase(children.begin()+counter);
				return;
			}
			counter++;
		}
		printf("no child with given name\n");
	}
//recursive breath tree analysis
	void node::printGraph()const{
		//test if it has children
		if(!children.empty()){
			for(node* child: children){
				std::cout<<child->getName()<<" ";
			}
			std::cout<<std::endl;
			for(node* child: children){
				child->printGraph();
			}
		}
		else
			return;
	}


	//some rendering getters and setters
	float node::getDistance()const{return distance;}
	float node::getSize()const{return size;}
	float node::getRotation()const{return rotation;}
	float node::getSpeed()const{return speed;}

	void node::setDistance(float dis){distance = dis;}
	void node::setSize(float siz){size = siz;}
	void node::setRotation(float rot){rotation = rot;}
	void node::setSpeed(float spe){speed = spe;}


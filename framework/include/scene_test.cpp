#ifndef SCENE_TEST_CPP
#define SCENE_TEST_CPP
#include "scene_graph.hpp"
#include "glm/ext.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	printf("=================TESTS=================\n");
/*
//test if node heredity works
	camera_node mainCam = camera_node{"camera"};
	cout<<mainCam.getName()<<endl;
*/

//test if scenegraph assignments and grapth works
	scene_graph scene = scene_graph{"scene"};
  //creating singualar nodes
  node system = node{"solarSystem"};

  geometry_node sun = geometry_node{"sun"};

  geometry_node mercury = geometry_node{"mercury"};
  mercury.setLocalTransform(glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 2.0f}));
  geometry_node venus = geometry_node{"venus"};
  geometry_node earth = geometry_node{"earth"};
  geometry_node mars = geometry_node{"mars"};
  geometry_node jupiter = geometry_node{"jupiter"};
  geometry_node saturn = geometry_node{"saturn"};
  geometry_node uranus = geometry_node{"uranus"};
  geometry_node neptune = geometry_node{"neptune"};
  geometry_node pluto = geometry_node{"pluto"};


  geometry_node j_moon_1 = geometry_node{"jupiterMoon1"}; 
  geometry_node j_moon_2 = geometry_node{"jupiterMoon2"};
  geometry_node e_moon = geometry_node{"earthMoon"};
  /////////creating hirarchie//////////////
  scene.setRoot(system);
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
  scene.printGraph();


  node* root = scene.getRoot();
	//cout<<e_moon.getPath()<<endl;	
	//cout<<e_moon.getDepth()<<endl;	
	//cout<<e_moon.getParent()->getName()<<endl;cout<<e_moon.getParent()->getParent()->getName()<<endl;

//test if worlmatrix calculation works
	glm::fmat4 testMax = glm::translate(glm::fmat4{}, glm::fvec3{4.0f, 4.0f, 4.0f});
	system.setLocalTransform(testMax);
	cout<<to_string(testMax)<<endl;
	cout<<to_string(e_moon.getWorldTransform())<<endl;

//further matrix calculation work

  scene_graph scene1 = scene_graph("scene1");
  vector<node*> children = scene1.getRoot()->getChildren();
  int count = 0;
  for(node* child: children){
    cout<<count++;
  }

}

#endif
#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"

#include <glm/gtc/type_precision.hpp>

#include <map>
#include <vector>

class scene_graph
{
 public:
	scene_graph();	
	scene_graph(std::string name);
	~scene_graph();

	//get-set string name of scene graph
	std::string getName();
	void setName(std::string newname);
	//get-set root node
	node* getRoot() const;
	void setRoot(node & newroot);
	//prints names of all children in graph
	void printGraph();

 protected:
 	std::string m_name;
 	node* m_root_node;
};
#endif
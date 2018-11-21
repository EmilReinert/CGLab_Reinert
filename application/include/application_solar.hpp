#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"

#include "scene_graph.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;
  //actual render for planet loop//for each planet
  void update_planet(node* const Planet, float count)const;

  //actuall render for planet loop//for each planet
  void drawStars()const;

  //////SCENE-SETUP here!! //////////////
  void sceneSetup();
  void setScene(scene_graph & scene);

  //retruns pointer of scene
  scene_graph* getScene()const;

 protected:
  void initializeShaderPrograms();

  //intializing of scene objects
    //for models
    void initializeGeometry();
    //initialiting of stars
    void initializeStars();
    //initialiting of stars
    void initializeOrbits();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();


  // scene graph as member
  scene_graph *m_scene_graph;

  //render mode
  int render_mode = 1;

  // cpu representation of models
  model_object planet_object;
  // colors of planetes
  std::vector<glm::vec3> planet_colors;
  model_object star_object;
  model_object orbit_object;

  //holder of special scene objects
  // holds a vector of the planets
  std::vector<node*> m_planets;  
  // holds random floats for stars
  std::vector<float> stars;

  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;


};

#endif
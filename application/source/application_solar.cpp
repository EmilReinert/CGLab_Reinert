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
#include "node.hpp"
#include <vector>
using namespace std;


ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{} 
 ,star_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  sceneSetup();
  initializeGeometry();
  initializeStars();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &star_object.element_BO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
}

///SCENE SETUP

void ApplicationSolar::setScene(scene_graph & scene){
  m_scene_graph = &scene;  
  node* root = scene.getRoot();
  m_planets = root->getChildren();
  std::cout<<"scene is set - ready to render!"<<std::endl;
}

scene_graph* ApplicationSolar::getScene()const{
  return m_scene_graph;
}

void ApplicationSolar::sceneSetup(){
  scene_graph scene = scene_graph{"scene"};
  setScene(scene);
}

//stars rendering loop
void ApplicationSolar::drawStars()const{
    // bind shader to upload uniforms
    glUseProgram(m_shaders.at("star").handle);

    // bind the VAO to draw
    glBindVertexArray(star_object.vertex_AO);
    //size of stars
    glPointSize(1);
    glDrawArrays(star_object.draw_mode, 0, int(stars.size()));
    //std::cout<<"drawing stars";
}

//planets rendering loop
void ApplicationSolar::update_planet(node* const Planet, float count)const{
    // bind shader to upload uniforms
    glUseProgram(m_shaders.at("planet").handle);

    //rotation
    glm::fmat4 model_matrix2 = glm::rotate(glm::fmat4{}, (1/(count+1))*float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});

    //in case the current planet is the sun (first element)
    if(count==0.0){
      //radius
      model_matrix2 = glm::translate(model_matrix2*model_matrix2, count*glm::fvec3{0.0f, 0.0f, -1.0f});
    
      //scale 
      model_matrix2 = glm::scale(model_matrix2, 1.5f * glm::fvec3{1.0f,1.0f,1.0f});
    }
    //other planets
    else
    {
      //radius
      model_matrix2 = glm::translate(model_matrix2, (count + 5.0f) * glm::fvec3{0.0f, 0.0f, -1.0f});
    
      //scale 
      model_matrix2 = glm::scale(model_matrix2, 0.3f * glm::fvec3{ (1/ count), (1/ count), (1/ count) });
    }

    glm::fmat4 model_matrix2cop = model_matrix2;
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix2));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix2 = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix2);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix2));

    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

/// adding some moons just for fun
    //rotation
    glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, (((int)count%3)+1)*(100.0f/(count+100))*float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});


    //radius
    model_matrix = glm::translate(model_matrix2*model_matrix, glm::fvec3{0.0f, 0.0f, -3.0f});
  
    //scale 
    model_matrix = glm::scale(model_matrix, 0.1f * glm::fvec3{ count,count,count });
  


    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix));

    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
    

  /// ACTUAL PLANET UPDATER 
  /// values get all crazy, so i take custom ones. as seen above

    /*std::cout<<"speed: "<<Planet->getSpeed()<<", distance: "<<Planet->getDistance()<<", size: "<<Planet->getSize()<<".\n";
    // bind shader to upload uniforms
    glUseProgram(m_shaders.at("planet").handle);

    //rotation
    glm::fmat4 model_matrix2 = glm::rotate(glm::fmat4{}, Planet->getSpeed()*float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});

    //radius
    model_matrix2 = glm::translate(model_matrix2*model_matrix2, Planet->getDistance()*glm::fvec3{0.0f, 0.0f, -1.0f});
    //scale 
    model_matrix2 = glm::scale(model_matrix2,glm::fvec3{Planet->getSize(),Planet->getSize(),Planet->getSize()});
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix2));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix2 = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix2);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix2));

    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);*/

}

void ApplicationSolar::render() const {

  float count = 0.0f;
  //loops and counts planets
  for (auto planet : m_planets ){
    //actual planted rendering
    update_planet(planet, count);
    count+=1.0f;;
  }
  //render stars
  drawStars();
}


void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  updateUniformLocations();
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});

  // request uniform locations for shader program
  //for planets
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
  //for stars
  m_shaders.at("star").u_locs["ModelViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;  
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());

}
// load models
void ApplicationSolar::initializeStars() {

  //filling empty star container
  for(int i=0; i<100000; i++){
    stars.push_back(float((rand()%200))-100.0f);
  }
  // generate vertex array object
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer 
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stars.size(), stars.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  
  // first attribute is 3 floats with no offset & stride  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 3);

   // generate generic buffer
  glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(stars.size());

}


///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadView();
  }
  if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, -0.0f});
    uploadView();
  }
  if (key == GLFW_KEY_E  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.0f});
    uploadView();
  }
  if (key == GLFW_KEY_R  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, -0.0f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling

  m_view_transform = glm::rotate(m_view_transform, float(pos_y)/300, glm::fvec3{-1.0f, 0.0f, 0.0f});
  m_view_transform = glm::rotate(m_view_transform, float(pos_x)/300, glm::fvec3{0.0f, -1.0f, 0.0f});
  uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
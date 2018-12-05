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

const char *modes[] = {"", "planet1", "planet2"};

ApplicationSolar::ApplicationSolar(std::string const &resource_path)
    : Application{resource_path}, planet_object{}, star_object{}, orbit_object{}, m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}, m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeTextures();
  sceneSetup();
  initializeGeometry();
  initializeStars();
  initializeOrbits();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar()
{
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &star_object.element_BO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
}

///SCENE SETUP

void ApplicationSolar::setScene(scene_graph &scene)
{
  m_scene_graph = &scene;
  node *root = scene.getRoot();
  m_planets = root->getChildren();
  std::cout << "scene is set - ready to render!" << std::endl;
}

scene_graph *ApplicationSolar::getScene() const
{
  return m_scene_graph;
}

void ApplicationSolar::sceneSetup()
{
  scene_graph scene = scene_graph{"scene"};
  setScene(scene);
}

//stars rendering loop
void ApplicationSolar::drawStars() const
{
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
void ApplicationSolar::update_planet(node *const Planet, float count) const
{
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at(modes[render_mode]).handle);

  //planet color
  glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
  color = planet_colors[(uint)count];
  //rotation
  glm::fmat4 model_matrix2 = glm::rotate(glm::fmat4{}, (1 / (count + 1)) * float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});

  //in case the current planet is the sun (first element)
  if (count == 0.0)
  {
    color.x = 1.0f;
    color.y = 0.8f;
    color.z = 0.3f;
    //radius
    model_matrix2 = glm::translate(model_matrix2 * model_matrix2, count * glm::fvec3{0.0f, 0.0f, -1.0f});

    //scale
    model_matrix2 = glm::scale(model_matrix2, 1.5f * glm::fvec3{1.0f, 1.0f, 1.0f});
  }
  //other planets
  else
  {
    //radius
    model_matrix2 = glm::translate(model_matrix2, (count)*glm::fvec3{0.0f, 0.0f, -1.0f});

    //scale
    model_matrix2 = glm::scale(model_matrix2, 0.3f * glm::fvec3{(1 / count), (1 / count), (1 / count)});
  }

  glm::fmat4 model_matrix2cop = model_matrix2;
  glUniformMatrix4fv(m_shaders.at(modes[render_mode]).u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix2));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix2 = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix2);
  glUniformMatrix4fv(m_shaders.at(modes[render_mode]).u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix2));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);
  // send colors to Shader
  glUniform3f(m_shaders.at(modes[render_mode]).u_locs.at("PlanetColor"), color.x, color.y, color.z);
  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  /// adding some moons just for fun
  //rotation
  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, (float)(((uint)count % 3) + 1) * (100.0f / (count + 100)) * float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
  //radius
  model_matrix = glm::translate(model_matrix2 * model_matrix, glm::fvec3{0.0f, 0.0f, -3.0f});
  //scale
  model_matrix = glm::scale(model_matrix, 0.1f * glm::fvec3{count, count, count});
  glUniformMatrix4fv(m_shaders.at(modes[render_mode]).u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));
  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at(modes[render_mode]).u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));
  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
}

void ApplicationSolar::render() const
{

  float count = 0.0f;
  //loops and counts planets
  for (auto planet : m_planets)
  {
    //actual planted rendering
    update_planet(planet, count);
    count += 1.0f;
    ;
  }
  //render stars
  drawStars();
}

void ApplicationSolar::uploadView()
{
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet1").handle);
  glUniformMatrix4fv(m_shaders.at("planet1").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  glUseProgram(m_shaders.at("planet2").handle);
  glUniformMatrix4fv(m_shaders.at("planet2").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  glUseProgram(m_shaders.at("orbit").handle);
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection()
{
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet1").handle);
  glUniformMatrix4fv(m_shaders.at("planet1").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  //planet mode 2
  glUseProgram(m_shaders.at("planet2").handle);
  glUniformMatrix4fv(m_shaders.at("planet2").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("star").handle);
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("orbit").handle);
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms()
{
  // bind shader to which to upload unforms
  updateUniformLocations();
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms()
{
  // store shader program objects in container
  m_shaders.emplace("planet1", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/planet.vert"},
                                               {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet.frag"}}});
  m_shaders.emplace("planet2", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/planet_cel.vert"},
                                               {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet_cel.frag"}}});
  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/vao.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  m_shaders.emplace("orbit", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/orbit.vert"},
                                             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/orbit.frag"}}});

  // request uniform locations for shader program
  //for planets rendermode 1
  m_shaders.at("planet1").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet1").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet1").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet1").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet1").u_locs["PlanetColor"] = -1;
  //render mode 2
  m_shaders.at("planet2").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet2").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet2").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet2").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet2").u_locs["PlanetColor"] = -1;
  // for stars
  m_shaders.at("star").u_locs["ModelViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;
  // for orbits
  m_shaders.at("orbit").u_locs["ModelMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ViewMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;
}

// load models //planets
void ApplicationSolar::initializeGeometry()
{

  // filling planet colors with random values
  float r, g, b;
  for (int i = 0; i < 10; i++)
  {
    r = float((rand() % 255)) / 255.0f;
    g = float((rand() % 255)) / 255.0f;
    b = float((rand() % 255)) / 255.0f;
    planet_colors.push_back(glm::vec3(r, g, b)); //change
  }

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

// load stars (points)
void ApplicationSolar::initializeStars()
{

  //filling empty star container
  for (int i = 0; i < 6 * 100000; i++)
  {
    stars.push_back(float((rand() % 200)) - 50.0f); //change
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);

  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 6 * sizeof(float), (void *)3);

  // generate generic buffer
  glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object
  star_object.num_elements = GLsizei(stars.size());
}
// load orbits
void ApplicationSolar::initializeOrbits()
{
  model orbit_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &orbit_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(orbit_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &orbit_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbit_model.data.size(), orbit_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, orbit_model.vertex_bytes, orbit_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, orbit_model.vertex_bytes, orbit_model.offsets[model::NORMAL]);

  // generate generic buffer
  glGenBuffers(1, &orbit_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * orbit_model.indices.size(), orbit_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  orbit_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object
  orbit_object.num_elements = GLsizei(orbit_model.indices.size());
}

//initializing textures
void ApplicationSolar::initializeTextures()
{
  // loading textures to create a vector of length 10 with right order of planets
  auto textures_path = m_resource_path + "/textures/";
  texture_images.push_back(texture_loader::file(textures_path + "sunmap.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "mercurymap.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "venusmap.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "earthmap1k.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "mars_1k_color.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "jupitermap.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "saturnmap.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "uranusmap.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "neptunemap.jpg"));
  texture_images.push_back(texture_loader::file(textures_path + "plutomap1k.jpg"));

  // actual initializing for each texture
  for (uint i = 0; i < texture_images.size(); i++)
  {
    texture_object t;
    textures.push_back(t);
    auto image = texture_images[i];
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &t.handle);
    glBindTexture(GL_TEXTURE_2D, t.handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, image.channels, (uint)image.width,
                 (uint)image.height, 0, image.channels, image.channel_type, image.ptr());
    //std::cout<<image.channels<<" "<<image.channel_type<<" "<<(uint)image.width<<" "<<(uint)image.height<<std::endl;
  }
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods)
{
  if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadView();
  }
  if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, -0.0f});
    uploadView();
  }
  if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.0f});
    uploadView();
  }
  if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, -0.0f});
    uploadView();
  }
  if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    render_mode = 1;
    std::cout << "switching to render mode " << render_mode << std::endl;
  }
  if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    render_mode = 2;
    std::cout << "switching to render mode " << render_mode << std::endl;
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y)
{
  // mouse handling

  m_view_transform = glm::rotate(m_view_transform, float(pos_y) / 300, glm::fvec3{-1.0f, 0.0f, 0.0f});
  m_view_transform = glm::rotate(m_view_transform, float(pos_x) / 300, glm::fvec3{0.0f, -1.0f, 0.0f});
  uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height)
{
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}

// exe entry point
int main(int argc, char *argv[])
{
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
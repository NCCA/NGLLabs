#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLStream.h>
#include <ngl/NGLInit.h>
#include <ngl/Util.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Random.h>
#include <iostream>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <algorithm>

NGLScene::NGLScene(int _numSpheres)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("First NGL");
  m_numSpheres=_numSpheres;
}


NGLScene::~NGLScene() noexcept
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::scatterSpheres(int _num)
{
  auto rng=ngl::Random::instance();
  Sphere s;
  float x,y,z;
  for(int i=0; i<_num; ++i)
  {
    x=rng->randomNumber(40);
    y=0;//rng->randomPositiveNumber(20);
    z=rng->randomNumber(40);
    s.setPos({x,y,z});
    s.setColour(rng->getRandomColour4());
    s.setRadius(rng->randomPositiveNumber(2.0)+0.2f);
    m_collideObjects.push_back(s);
  }
}


void NGLScene::drawConnections()
{
  if(m_collideObjects.size()==0) return;
  auto shader=ngl::ShaderLib::instance();
  shader->setUniform("MVP",m_project*m_view);
  shader->setUniform("Colour",1.0f,1.0f,1.0f,1.0f);
  std::vector<ngl::Vec3> data;
  // fill in lines from m_sphere to each sphere for visualisation
  for(auto s : m_collideObjects)
  {
    data.push_back(m_sphere.getPos());
    data.push_back(s.getPos());
  }

  m_vao->bind();
  m_vao->setData( ngl::SimpleVAO::VertexData(data.size()*sizeof(ngl::Vec3),data[0].m_x));
  // We must do this each time as we change the data.
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setNumIndices(data.size());
  m_vao->draw();
  m_vao->unbind();
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_project = ngl::perspective(45.0f, static_cast<float>(_w) / _h, 0.5f, 300.0f);

}


void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.6f, 0.6f, 0.6f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  m_view = ngl::lookAt({0.0f, 40.0f, 40.0f}, { 0.0f,0.0f,0.0f }, ngl::Vec3::up());

  std::cout << m_view << '\n';
  ngl::VAOPrimitives::instance()->createSphere("sphere",1.0f,20.0f);
  ngl::VAOPrimitives::instance()->createLineGrid("floor",100,100,100);
  m_sphere.setColour(1.0f,0.0f,0.0f);
  m_sphere.moveRelative(0,0,0);
  scatterSpheres(m_numSpheres);
  findClosestSphere();
  // create the VAO but don't populate
  m_vao=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_LINES);

  startTimer(10);
}

void NGLScene::findClosestSphere()
{
  // set to largest size.
  ngl::Real closestDistance=std::numeric_limits<ngl::Real>::max();

  for(auto s : m_collideObjects)
  {
    float len =(m_sphere.getPos()-s.getPos()).lengthSquared();
    if(len <= closestDistance)
    {
      closestDistance=len;
      m_closestPos=s.getPos();
    }
  }

}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  auto shader = ngl::ShaderLib::instance();
  shader->use(ngl::nglColourShader);
  ngl::Mat4 VP = m_project * m_view;




  m_collideObjects.erase(
                         std::remove_if(
                                         std::begin(m_collideObjects),
                                         std::end(m_collideObjects),
                                         [=]( Sphere s)
                                         {
                                           return s.isColliding(m_sphere);
                                         })
                          ,std::end(m_collideObjects)
                        );

  m_sphere.draw(VP);


  for(auto s : m_collideObjects)
    s.draw(VP);

  drawConnections();
  shader->setUniform("MVP",VP);
  shader->setUniform("Colour",0.3f,0.3f,0.3f,1.0f);
  ngl::VAOPrimitives::instance()->draw("floor");
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  constexpr float s_moveAmmount = 0.5f;
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

      break;
  case Qt::Key_W: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
  case Qt::Key_S: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
  case Qt::Key_Left : m_sphere.moveRelative(-s_moveAmmount,0.0f,0.0f); break;
  case Qt::Key_Right : m_sphere.moveRelative(s_moveAmmount,0.0f,0.0f); break;
  case Qt::Key_Down : m_sphere.moveRelative(0.0f,0.0f,s_moveAmmount); break;
  case Qt::Key_Up : m_sphere.moveRelative(0.0f,0.0f,-s_moveAmmount); break;
  case Qt::Key_R : m_collideObjects.clear(); scatterSpheres(m_numSpheres); break;
  case Qt::Key_1 :
      m_view = ngl::lookAt({0.0f, 40.0f, 40.0f}, { 0.0f,0.0f,0.0f }, ngl::Vec3::up()); break;
  case Qt::Key_2 :
      m_view = ngl::lookAt({0.0,100.0f,0.0f}, { 0.0f,0.0f,0.0f }, -ngl::Vec3::in());
    break;

  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}


void NGLScene::timerEvent(QTimerEvent *)
{
  findClosestSphere();
  auto dir=m_closestPos-m_sphere.getPos();
  dir.normalize();

  m_sphere.moveRelative(dir);
  update();
}


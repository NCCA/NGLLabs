#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <iostream>

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Particle System");
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_project = ngl::perspective(45.0f, static_cast<float>(_w) / _h, 0.1f, 300.0f);
}


void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  auto shader = ngl::ShaderLib::instance();
  shader->loadShader("ParticleShader",
      "shaders/ParticleVertex.glsl",
      "shaders/ParticleFragment.glsl");
  shader->use("ParticleShader");
  m_view = ngl::lookAt({ 50,50,50 }, ngl::Vec3::zero(), ngl::Vec3::up());

  shader->setUniform("MVP", m_project * m_view);
  m_emitter = std::make_unique<Emitter>(50000);

  startTimer(10);
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  auto shader = ngl::ShaderLib::instance();
  ngl::Mat4 xrot;
  ngl::Mat4 yrot;

  xrot.rotateX(m_win.spinXFace);
  yrot.rotateY(m_win.spinYFace);
  m_globalMouseTX = yrot * xrot;
  m_globalMouseTX.m_m[3][0] = m_modelPos.m_x;
  m_globalMouseTX.m_m[3][1] = m_modelPos.m_y;
  m_globalMouseTX.m_m[3][2] = m_modelPos.m_z;

  shader->setUniform("MVP", m_project * m_view * m_globalMouseTX);
  glPointSize(1);
  m_emitter->draw();
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
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
  case Qt::Key_Left: m_emitter->left(5.0f); break;
  case Qt::Key_Right: m_emitter->right(5.0f); break;
  case Qt::Key_Up: m_emitter->up(5.0f); break;
  case Qt::Key_Down: m_emitter->down(5.0f); break;
  case Qt::Key_I: m_emitter->in(5.0f); break;
  case Qt::Key_O: m_emitter->out(5.0f); break;

  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}

void NGLScene::timerEvent(QTimerEvent*)
{
    m_emitter->update();
    update();
}


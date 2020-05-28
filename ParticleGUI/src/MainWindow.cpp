#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_gl = new NGLScene(1000);
  ui->gridLayout->addWidget(m_gl, 0, 1, 2, 6);

  connect(ui->clearAll, SIGNAL(clicked()), m_gl, SLOT(clearParticles()));
  connect(ui->addParticle, SIGNAL(clicked()), m_gl, SLOT(addParticle()));
  connect(ui->removeParticle, SIGNAL(clicked()), m_gl, SLOT(removeParticle()));
  connect(ui->pointSize, SIGNAL(valueChanged(int)), m_gl, SLOT(setPointSize(int)));
}

MainWindow::~MainWindow()
{
  delete ui;
}


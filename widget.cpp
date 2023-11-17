#include "widget.h"
#include <QDebug>

DirectionalLight sun = {
    {-1.0f, -1.0f, -1.0f},
    { 0.5f,  0.5f,  0.5f},
};

PointLight light = {
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    1.0f,
    0.09f,
    0.032f
};

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , camera(this)
{
//    timer.setInterval(18);
//    connect(&timer,&QTimer::timeout,this,static_cast<void (Widget::*)()>(&Widget::update));
//    timer.start();
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/model.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/model.frag");
    shaderProgram.link();

    QMatrix4x4 matModel;
    matModel.scale(0.05f, 0.05f, 0.05f);
//    matModel.translate(-130.0f, -50.0f, 0.0f);
    matModel.translate(0.0f, -7.0f, 0.0f);

    model=Model::createModel("/home/zht/code/ModelLoading/nanosuit/nanosuit.obj", context(), &shaderProgram, matModel);

    glEnable(GL_DEPTH_TEST);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 projection;
    projection.perspective(45.0f, width()/(float)height(), 0.1f, 500.0f);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", camera.getView());

    shaderProgram.setUniformValue("viewPos", camera.getCameraPos());

    shaderProgram.setUniformValue("sun.color", sun.color);
    shaderProgram.setUniformValue("sun.direction", sun.direction);

    light.position.setX((float)sin(QTime::currentTime().msecsSinceStartOfDay() / 1000.0f));
    light.position.setY((float)sin(QTime::currentTime().msecsSinceStartOfDay() / 2000.0f));
    light.position.setZ(1.0f);
//    shaderProgram.setUniformValue("light.color", QVector3D(0.0f, 0.0f, 0.0f));
    shaderProgram.setUniformValue("light.color", light.color);
    shaderProgram.setUniformValue("light.position", light.position);
    shaderProgram.setUniformValue("light.constant", light.constant);
    shaderProgram.setUniformValue("light.linear", light.linear);
    shaderProgram.setUniformValue("light.quadratic", light.quadratic);

    model->draw();

    update();
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

bool Widget::event(QEvent*e){
    camera.handle(e);
    return QWidget::event(e);
}


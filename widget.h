#ifndef WIDGET_H
#define WIDGET_H

#include "camera.h"
#include "model.h"

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include <QTimer>
#include <QWidget>

struct DirectionalLight
{
    QVector3D direction;
    QVector3D color;
};

struct PointLight
{
    QVector3D position;
    QVector3D color;
    float constant;
    float linear;
    float quadratic;
};

class Widget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual bool event(QEvent *e) override;
    Model* model;
    Camera camera;
    QTimer timer;

private:
    QOpenGLShaderProgram shaderProgram;
};

#endif // WIDGET_H

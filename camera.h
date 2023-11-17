#ifndef CAMERA_H
#define CAMERA_H

#include <QSet>
#include <QVector3D>
#include <QEvent>
#include <QWidget>
#include <QtMath>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QTime>

enum class Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float RADIUS = 5.0f;
const float SCALESPEED = 1.0f;
const float MOVEMENTSPEED = 2.5f;
const float ANGULARSPEED = 50.0f;
const float ZOOM = 45.0f;
const QVector3D WORLDUP = {0.0f, 1.0f, 0.0f};

class Camera
{
public:
    Camera(QWidget *widget, QVector3D pos = QVector3D(0.0f, 0.0f, 3.0f));

    QVector3D getCameraPos() const;

    void handle(QEvent *event);

    QMatrix4x4 getView();

private:
    void init();
    void normalize();
    void updateCameraVectors();

    void move(Camera_Movement direction, float deltaTime);

private:
    QWidget *widget;

    float yaw;
    float pitch;
    float radius;

    float scaleSpeed;
    float moveSpeed;
    float angularSpeed;
    float zoom;

    QVector3D position;
    QVector3D worldUp;

    QSet<int> keys;     //记录当前被按下按键的集合

    int timeId;         //定时器id：此定时器用于完成键盘移动事件
    float deltaTime;    // 当前帧与上一帧的时间差
    float lastFrame;    // 上一帧的时间

    QMatrix4x4 view;    //观察矩阵

};

#endif // CAMERA_H

#include "camera.h"

Camera::Camera(QWidget *widget, QVector3D pos)
    : widget(widget), scaleSpeed(SCALESPEED), moveSpeed(MOVEMENTSPEED)
    , angularSpeed(ANGULARSPEED), zoom(ZOOM), position(pos), worldUp(WORLDUP)
{
    init();
    updateCameraVectors();

    widget->setMouseTracking(true);
}

QVector3D Camera::getCameraPos() const
{
    return position;
}

void Camera::handle(QEvent *e)
{
    static bool pressFlag = false;

    if(e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* event = static_cast<QMouseEvent*>(e);

        if(event->button() == Qt::LeftButton)
        {
            pressFlag = true;
        }
    }
    else if(e->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* event = static_cast<QMouseEvent*>(e);

        if(event->button() == Qt::LeftButton)
        {
            pressFlag = false;
        }
    }
    else if(e->type() == QEvent::MouseMove)
    {
        QMouseEvent* event = static_cast<QMouseEvent*>(e);

        static float lastX = event->x();
        static float lastY = event->y();

        float xoffset = event->x() - lastX;
        float yoffset = lastY - event->y(); // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的

        lastX = event->x();
        lastY = event->y();

        if(pressFlag)
        {
            move(Camera_Movement::LEFT, xoffset / 50);
            move(Camera_Movement::DOWN, yoffset / 50);
        }
    }
    else if(e->type() == QEvent::Wheel)
    {
        QWheelEvent* event = static_cast<QWheelEvent*>(e);

        move(Camera_Movement::FORWARD, event->angleDelta().y() / 1000.0f);
    }
    else if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent* event = static_cast<QKeyEvent*>(e);
        switch (event->key()) {
        case Qt::Key_W:
            move(Camera_Movement::FORWARD, deltaTime);
            break;
        case Qt::Key_S:
            move(Camera_Movement::BACKWARD, deltaTime);
            break;
        case Qt::Key_A:
            move(Camera_Movement::LEFT, deltaTime);
            break;
        case Qt::Key_D:
            move(Camera_Movement::RIGHT, deltaTime);
            break;
        case Qt::Key_Control:
            move(Camera_Movement::DOWN, deltaTime);
            break;
        case Qt::Key_Space:
            move(Camera_Movement::UP, deltaTime);
            break;
        }
    }
    else if(e->type() == QEvent::UpdateRequest)
    {
        float time = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
        deltaTime = time - lastFrame;                           //在此处更新时间差
        lastFrame = time;
    }
}

void Camera::init()
{
    radius = position.length();
    pitch = 90.0f - qRadiansToDegrees(acos(QVector3D::dotProduct(QVector3D(0.0f, 1.0f, 0.0f), position.normalized())));
    QVector3D norm = QVector3D::normal(position, worldUp);
    if (position.z() >= 0)
        yaw = qRadiansToDegrees(acos(QVector3D::dotProduct(QVector3D(0.0f, 0.0f, 1.0f), norm)));
    else
        yaw = -qRadiansToDegrees(acos(QVector3D::dotProduct(QVector3D(0.0f, 0.0f, 1.0f), norm)));

    normalize();

    widget->activateWindow();                 //激活窗口
    widget->setFocus();
}

void Camera::normalize()
{
    if (radius < 0.1f)
        radius = 0.1f;

    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;

    while (yaw > 180.0f)
        yaw -= 360.0f;
    while (yaw <= -180.0f)
        yaw += 360.0f;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void Camera::updateCameraVectors()
{
    normalize();

    position.setX(radius * cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    position.setY(radius * sin(qDegreesToRadians(pitch)));
    position.setZ(radius * sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
}

void Camera::move(Camera_Movement direction, float deltaTime)
{
    float movementVelocity = moveSpeed * deltaTime;
    float angularVelocity = angularSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        radius -= movementVelocity;
    if (direction == Camera_Movement::BACKWARD)
        radius += movementVelocity;
    if (direction == Camera_Movement::LEFT)
        yaw += angularVelocity;
    if (direction == Camera_Movement::RIGHT)
        yaw -= angularVelocity;
    if (direction == Camera_Movement::UP)
        pitch += angularVelocity;
    if (direction == Camera_Movement::DOWN)
        pitch -= angularVelocity;
}

QMatrix4x4 Camera::getView()
{
    updateCameraVectors();
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(position, QVector3D(0.0f, 0.0f, 0.0f) - position, worldUp);
    return view;
}

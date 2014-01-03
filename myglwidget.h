#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QOpenGLFunctions>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
//#include <QOpenGLDebugLogger>
#include <QOpenGLDebugMessage>

class QOpenGLDebugLogger;

#include <QTime>
#include <QTimer>

class MyGlWidget : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyGlWidget(QWidget *parent = 0);

    QColor color() const;

public slots:
    void setOffsetX(double value);
    void setOffsetY(double value);
    void setOffsetZ(double value);
    void setColor(QColor color);
    void setZNear(double value);
    void setZFar(double value);
    void setFrustrumScale(double value);
    void setEyePoint(QVector3D value);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private slots:
    void printOpenGLLoggerMessage(QOpenGLDebugMessage message);

private:
    void initializeProgram();
    void updatePerspectiveMatrix();
    void initializeOpenGLLogger();

    QOpenGLShaderProgram program;

    QOpenGLDebugLogger *openGLLogger;

//    QGLBuffer triangleBuffer;
//    const QByteArray triangleBufferName;
//    const QByteArray vertexColorName;

    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLBuffer vertexDataBuffer;
    const QByteArray vertexDataBufferName;
    QOpenGLBuffer indexDataBuffer;

    QVector3D vertexOffset;
    QColor triangleColor;
    QMatrix4x4 perspectiveMatrix;
    float zNear;
    float zFar;
    float frustumScale;
    QVector3D eyePoint;

    QTime startTime;

    QTimer updateTimer;
};

#endif // MYGLWIDGET_H

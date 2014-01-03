#include "myglwidget.h"

#include <QOpenGLDebugLogger>
#include <QDebug>

#include "vertexdata.h"

MyGlWidget::MyGlWidget(QWidget *parent) :
    QGLWidget(parent),
//    triangleBuffer(QGLBuffer::VertexBuffer),
//    triangleBufferName("vertex"),
//    vertexColorName("i_vertexColor"),
    vertexDataBuffer(QOpenGLBuffer::VertexBuffer),
    vertexDataBufferName("vertex"),
    indexDataBuffer(QOpenGLBuffer::IndexBuffer),
    vertexOffset(0.0f, 0.0f),
    triangleColor("#FFFFFF"),
    zNear(1.0f),
    zFar(3.0f),
    frustumScale(1.0f),
    startTime(QTime::currentTime())
{
    updateTimer.setInterval(1000.0 / 60.0);
    connect(&updateTimer, SIGNAL(timeout()),
            this, SLOT(updateGL()));
}

QColor MyGlWidget::color() const
{
    return triangleColor;
}

void MyGlWidget::setOffsetX(double value)
{
    vertexOffset.setX(value);
    updateGL();
}

void MyGlWidget::setOffsetY(double value)
{
    vertexOffset.setY(value);
    updateGL();
}

void MyGlWidget::setColor(QColor color)
{
    triangleColor = color;
    updateGL();
}

void MyGlWidget::setZNear(double value)
{
    zNear = value;
    updatePerspectiveMatrix();
    updateGL();
}

void MyGlWidget::setZFar(double value)
{
    zFar = value;
    updatePerspectiveMatrix();
    updateGL();
}

void MyGlWidget::setFrustrumScale(double value)
{
    frustumScale = value;
    updatePerspectiveMatrix();
    updateGL();
}

void MyGlWidget::setEyePoint(QVector3D value)
{
    eyePoint = value;
}

void MyGlWidget::initializeGL()
{
    initializeOpenGLLogger();
    initializeOpenGLFunctions();


    initializeProgram();

    // Face Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Enabling depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glClearDepth(1.0f);
}

void MyGlWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

//    const GLint side = qMin(w, h);
//    glViewport((w - side) / 2, (h - side) / 2, side, side);
}

void MyGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.setUniformValue("perspectiveMatrix", perspectiveMatrix);
    program.setUniformValue("vertexOffset", QVector2D(0.0f, 0.0f));
    qDebug() << "GL Error1:" << static_cast<int>(glGetError());

    vertexArrayObject.bind();
    qDebug() << "GL Error2:" << static_cast<int>(glGetError());

    const GLsizei cube2VertexOffset = vertexData3Size / 2;
    qDebug() << "GL Error3:" << static_cast<int>(glGetError());
    glDrawElements(GL_TRIANGLES, indexData1Count, GL_UNSIGNED_SHORT, 0);
//    glDrawElementsBaseVertex(GL_TRIANGLES, indexData1Count, GL_UNSIGNED_SHORT, 0, cube2VertexOffset);

    vertexArrayObject.release();
}

void MyGlWidget::printOpenGLLoggerMessage(QOpenGLDebugMessage message)
{
    qDebug() << "OpenGL Debug:" << message;
}

void MyGlWidget::initializeProgram()
{
    qglClearColor(QColor("#111111"));

    /* Initialize Shaders */
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/vertex_shader.vert")) {
        qDebug() << "MyGlWidget::initializeProgram: "
                    "Unable to add vertex shader:" << program.log();
    }

    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/fragment_shader.frag")) {
        qDebug() << "MyGlWidget::initializeProgram: "
                    "Unable to add fragment shader:" << program.log();
    }

    if (!program.link()) {
        qDebug() << "MyGlWidget::initializeGL: Unable to link program:"
                 << program.log();
    }

//    if (!triangleBuffer.create())
//        qDebug() << "MyGlWidget::initializeGL: Unable to create triangleBuffer";

//    triangleBuffer.setUsagePattern(QGLBuffer::StreamDraw);

//    if (!triangleBuffer.bind())
//        qDebug() << "MyGlWidget::initializeGL: Unable to bind triangleBuffer";

//    triangleBuffer.allocate(reinterpret_cast<const void *>(vertexData1),
//                            72 * 4 * sizeof(float));

    /* Init Vertex Array Object */
    if (!vertexArrayObject.create()) {
        qDebug() << "MyGlWidget::initializeGL: Unable to create vertex array object."
                 << program.log();
    }

    vertexArrayObject.bind();
    qDebug() << "VertexArrayObject.bind:" << static_cast<int>(glGetError());

    /* Init Vertex Buffers */
    if (!vertexDataBuffer.create())
        qDebug() << "MyGlWidget::initializeGL: Unable to create vertexDataBuffer.";

    vertexDataBuffer.setUsagePattern(QOpenGLBuffer::StreamDraw);

    if (!vertexDataBuffer.bind())
        qDebug() << "MyGlWidget::initializeGL: Unable to bind vertexDataBuffer";

    vertexDataBuffer.allocate(reinterpret_cast<const void *>(vertexData3),
                              vertexData3Size);

    program.enableAttributeArray(vertexDataBufferName.constData());
    program.setAttributeBuffer(vertexDataBufferName.constData(), GL_FLOAT, 0, 3);
    qDebug() << "vertex attribute buffer:" << static_cast<int>(glGetError());

    if (!indexDataBuffer.create())
        qDebug() << "MyGlWidget::initializeGL: Unable to create indexDataBuffer.";

    indexDataBuffer.setUsagePattern(QOpenGLBuffer::StreamDraw);

    if (!indexDataBuffer.bind())
        qDebug() << "MyGlWidget::initializeGL: Unable to bind indexDataBuffer";

    indexDataBuffer.allocate(reinterpret_cast<const void *>(indexData1),
                             indexData1Size);

    program.enableAttributeArray(1);
    program.setAttributeBuffer(1, GL_UNSIGNED_SHORT, 0, 3);

    vertexArrayObject.release();

    if (!program.bind()) {
        qDebug() << "MyGlWidget::initializeGL: Unable to bind program."
                 << program.log();
    }

//    program.setAttributeBuffer(triangleBufferName.constData(), GL_FLOAT, 0, 4);
//    program.enableAttributeArray(triangleBufferName.constData());

//    program.setAttributeBuffer(vertexColorName.constData(), GL_FLOAT, 36 * 4 * sizeof(float), 4);
//    program.enableAttributeArray(vertexColorName.constData());

    updatePerspectiveMatrix();
}

void MyGlWidget::updatePerspectiveMatrix()
{
//    perspectiveMatrix.setRow(0, QVector4D(frustumScale, 0.0f, 0.0f, 0.0f));
//    perspectiveMatrix.setRow(1, QVector4D(0.0f, frustumScale, 0.0f, 0.0f));
//    perspectiveMatrix.setRow(2, QVector4D(0.0f, 0.0f, 1.0f, 0.0f));
//    perspectiveMatrix.setRow(3, QVector4D(0.0f, 0.0f, 1.0f, 0.0f));

    perspectiveMatrix.setRow(0, QVector4D(frustumScale, 0.0f, 0.0f, 0.0f));
    perspectiveMatrix.setRow(1, QVector4D(0.0f, frustumScale, 0.0f, 0.0f));
    perspectiveMatrix.setRow(2, QVector4D(0.0f,
                                          0.0f,
                                          ((zFar + zNear) / (zNear - zFar)),
                                          ((2 * zFar * zNear) / (zNear - zFar))));
    perspectiveMatrix.setRow(3, QVector4D(0.0f, 0.0f, -1.0f, 0.0f));

    program.setUniformValue("perspectiveMatrix", perspectiveMatrix);
}

inline void MyGlWidget::initializeOpenGLLogger()
{
    openGLLogger = new QOpenGLDebugLogger(this);

    connect(openGLLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
            this, SLOT(printOpenGLLoggerMessage(QOpenGLDebugMessage)));

    if (openGLLogger->initialize()) {
        openGLLogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
        openGLLogger->enableMessages();
    } else {
        qDebug() << "MyGlWidget::initializeOpenGLLogger: Unable to initialize OpenGL Logger:" << static_cast<quint32>(glGetError());
    }
}

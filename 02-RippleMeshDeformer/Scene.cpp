#include "Scene.h"
#include <QOpenGLWindow>

Scene::Scene(SceneGLWindow* window):
    AbstractGLScene(window)
{
    window->setScene(this);
    connect(window, &SceneGLWindow::camPosChanged,
            this, &Scene::setCamPos);
    connect(window, &SceneGLWindow::camFrontChanged,
            this, &Scene::setCamFront);
    connect(window, &SceneGLWindow::camUpChanged,
            this, &Scene::setCamUp);
    connect(window, &SceneGLWindow::camZoomChanged, this, &Scene::setZoom);
}

Scene::~Scene()
{
    m_shader.release();
}

void Scene::initialize()
{
    AbstractGLScene::initialize();

    m_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert");
    m_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag");
    m_shader.link();

    //    m_texture = new QOpenGLTexture(QImage(":/Zhejiang_University_Logo.png").mirrored());
    //    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    //    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);



    QVector3D vertices[(NUM_X+1)*(NUM_Z+1)];

    GLushort indices[TOTAL_INDICES];
    //setup plane geometry
    //setup plane vertices
    int count = 0;
    int i=0, j=0;
    for( j=0;j<=NUM_Z;j++) {
        for( i=0;i<=NUM_X;i++) {
            vertices[count++] = QVector3D( ((float(i)/(NUM_X-1)) *2-1)* HALF_SIZE_X, 0, ((float(j)/(NUM_Z-1))*2-1)*HALF_SIZE_Z);
            qDebug()<<vertices[count-1];
        }
    }

    //fill plane indices array
    GLushort* id=&indices[0];
    for (i = 0; i < NUM_Z; i++) {
        for (j = 0; j < NUM_X; j++) {
            int i0 = i * (NUM_X+1) + j;
            int i1 = i0 + 1;
            int i2 = i0 + (NUM_X+1);
            int i3 = i2 + 1;
            if ((j+i)%2) {
                *id++ = i0; *id++ = i2; *id++ = i1;
                *id++ = i1; *id++ = i2; *id++ = i3;
            } else {
                *id++ = i0; *id++ = i2; *id++ = i3;
                *id++ = i0; *id++ = i3; *id++ = i1;
            }
        }
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_VBOIndices);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
//    glEnableVertexAttribArray(1);
    //pass indices to element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
    glEnable(GL_DEPTH_TEST);

    m_cameraPos   = QVector3D(3.0f, 3.0f,  3.0f);
    m_cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    m_cameraUp    = QVector3D(0.0f, 1.0f,  0.0f);

    //    m_shader.bind();
    //    m_shader.setUniformValue("texture", 0);
    //    m_texture->bind(0);

    //    m_shader.release();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
}


void Scene::paint()
{
    glViewport(0, 0, window()->width(), window()->height());
    glClearColor(.2f, .3f, .3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();

    QMatrix4x4 model;
    model.rotate(m_val, QVector3D(1, 1, 0));
    //    m_shader.setUniformValue("model", model);

    QMatrix4x4 view;
    view.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    //    m_shader.setUniformValue("view", view);

    QMatrix4x4 projection;
    projection.perspective(m_zoom, float(window()->width()) / window()->height(), 0.1f, 100.f);
    //    m_shader.setUniformValue("projection", projection);
    m_shader.setUniformValue("MVP",projection*view*model);

    m_shader.setUniformValue("time",float(QTime::currentTime().msecsSinceStartOfDay()/1000.0f));
    glBindVertexArray(m_VAO);

    //    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDrawElements(GL_TRIANGLES, TOTAL_INDICES, GL_UNSIGNED_SHORT, 0);
    QCursor c = window()->cursor();
    c.setPos(window()->mapToGlobal(QPoint(window()->width() / 2, window()->height() / 2)));
    c.setShape(Qt::BlankCursor);
    window()->setCursor(c);
}
qreal Scene::getAnimate() const
{
    return m_val;
}
void Scene::setAnimate(qreal val)
{
    m_val = val;
    window()->update();
}
void Scene::setCamPos(QVector3D pos)
{
    m_cameraPos = pos;
}
void Scene::setCamFront(QVector3D front)
{
    m_cameraFront = front;
}
void Scene::setCamUp(QVector3D up)
{
    m_cameraUp = up;
}
void Scene::setZoom(float zoom)
{
    m_zoom = zoom;
}

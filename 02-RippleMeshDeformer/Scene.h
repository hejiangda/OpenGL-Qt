#ifndef Scene_H
#define Scene_H

#include "AbstractGLScene.h"
#include "SceneGLWindow.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class SceneGLWindow;
class Scene: public QObject, public AbstractGLScene
{
    Q_OBJECT
    Q_PROPERTY(qreal val READ getAnimate WRITE setAnimate)

public:
    Scene(SceneGLWindow* window);
    ~Scene();

    void initialize();
    void paint();
    qreal getAnimate()const;
public slots:
    void setAnimate(qreal val);

    void setCamPos(QVector3D pos);
    void setCamFront(QVector3D front);
    void setCamUp(QVector3D up);
    void setZoom(float zoom);
private:
    QOpenGLShaderProgram m_shader;
    qreal m_val;

    GLuint m_VAO, m_VBO,m_VBOIndices;
    QOpenGLTexture* m_texture;
    const int NUM_X = 240; //total quads on X axis
    const int NUM_Z = 240; //total quads on Z axis

    const float SIZE_X = 4; //size of plane in world space
    const float SIZE_Z = 4;
    const float HALF_SIZE_X = SIZE_X/2.0f;
    const float HALF_SIZE_Z = SIZE_Z/2.0f;
    const int TOTAL_INDICES = NUM_X*NUM_Z*2*3;
    QVector3D m_cameraPos, m_cameraFront, m_cameraUp;
    float m_zoom = 45;
};

#endif // Scene_H

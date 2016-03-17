#ifndef ROTATINGSQUAREWINDOW_H
#define ROTATINGSQUAREWINDOW_H

#include "openglwindow.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtCore/QElapsedTimer>
class RotatingSquareWindow: public OpenGLWindow
{
public:
  RotatingSquareWindow();
  void initialize() Q_DECL_OVERRIDE;
  void render() Q_DECL_OVERRIDE;
  void setVelocity(float value);
protected:
  void resizeEvent(QResizeEvent *event);
private:
  // Variáveis (atributos e uniforms) dos shaders
  GLuint m_posAttr;
  GLuint m_colAttr;
  GLuint m_modelMatrixUniform;
  GLuint m_viewMatrixUniform;
  GLuint m_projectionMatrixUniform;

  // Buffers
  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer m_vbo_pos;
  QOpenGLBuffer m_vbo_col;
  QOpenGLShaderProgram *m_program;

  // Matrizes
  QMatrix4x4 m_modelMatrix;
  QMatrix4x4 m_viewMatrix;
  QMatrix4x4 m_viewMatrix_init; // without rotation
  QMatrix4x4 m_projectionMatrix;

  // Animação
  QElapsedTimer m_timer;
  int    m_frame;
  float  m_angle;
  float  m_zpos;
  float  m_angular_velocity;

  // Dados do retângulo (posição e cor)
  // 1 square = 2 triangles x 3 vértices = 6 3D positions = 18 numbers
  GLfloat m_square_pos[18] = {-1.0f, -1.0f,  0.0f,
                               1.0f, -1.0f,  0.0f,
                               1.0f,  1.0f,  0.0f,
                               1.0f,  1.0f,  0.0f,
                              -1.0f,  1.0f,  0.0f,
                              -1.0f, -1.0f,  0.0f};
  GLfloat m_square_col[18] = { 1.0f,  0.0f,  0.0f,
                               0.0f,  1.0f,  0.0f,
                               0.0f,  0.0f,  1.0f,
                               1.0f,  0.0f,  0.0f,
                               1.0f,  0.0f,  0.0f,
                               1.0f,  0.0f,  0.0f};

  void updateProjection();
};

#endif // ROTATINGSQUAREWINDOW_H

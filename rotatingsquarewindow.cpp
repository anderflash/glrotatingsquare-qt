#include "rotatingsquarewindow.h"
#include "glassert.h"
#include <iostream>

RotatingSquareWindow::RotatingSquareWindow()
  : m_vbo_pos(QOpenGLBuffer::VertexBuffer)
  , m_vbo_col(QOpenGLBuffer::VertexBuffer)
  , m_program(0)
  , m_frame(0)
  , m_angle(0)
  , m_zpos(0)
  , m_angular_velocity(45.0f)

{
}

// Geralmente esquecemos algum passo ao criar aplicações
// OpenGL. Bom, depois de criar o contexto OpenGL (a classe
// básica faz isso, precisamos:
// - carregar nossos shaders
// - criar o programa OpenGL (que representará um modelo de
//   transformação dos vértices aos pixels). Chuva pode deman-
//   dar diferentes efeitos do que o chão, lago, céu, sol...
//   Então o OpenGL não trabalha diretamente com o shader, e
//   sim com o programa, até porque os `uniform`s são variáveis
//   disponíveis em todos os shaders daquele programa. Cada
//   programa tem seu espaço de memória.
// - Compilá-los, vinculá-los os diferentes elementos entre si
//   (linking) e capturar os representantes dos atributos dos
//   vértices (posição, cor, coordenada de textura...), bem
//   como os buffers disponíveis para todos os vértices, como
//   as matrizes de transformação.
// - Criar os buffers na GPU para os dados da posição e da cor
// - Enviar também para a GPU as matrizes
// Vincular (link) um programa não significa usá-lo. Posso vin-
// cular vários programas (um para chuva, outro para espelho...)
// e dizer quando uso um ou outro no momento certo.
void RotatingSquareWindow::initialize(){
  glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
  // Criar os shaders e os programas
  m_program = new QOpenGLShaderProgram(this);
  m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,"vertex.glsl");
  m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"fragment.glsl");
  m_program->link();

  // Usar o programa e capturar suas variáveis (uniforms, attributes...)
  m_program->bind();
  m_posAttr = m_program->attributeLocation("posAttr");
  m_colAttr = m_program->attributeLocation("colAttr");
  m_modelMatrixUniform      = m_program->uniformLocation("modelMatrix");
  m_viewMatrixUniform       = m_program->uniformLocation("viewMatrix");
  m_projectionMatrixUniform = m_program->uniformLocation("projectionMatrix");

  // Criar os buffers
  m_vao.create();
  m_vao.bind();

  m_vbo_pos.create();
  m_vbo_pos.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo_pos.bind();
  m_vbo_pos.allocate(m_square_pos, 18 * sizeof(GLfloat));
  m_program->enableAttributeArray(m_posAttr);
  m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3);

  m_vbo_col.create();
  m_vbo_col.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vbo_col.bind();
  m_vbo_col.allocate(m_square_col, 18 * sizeof(float));
  m_program->enableAttributeArray(m_colAttr);
  m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 0, 3);

  glCheckError();

  // Upload das matrizes
  m_modelMatrix.setToIdentity();
  m_modelMatrix.scale(2.0f);
  m_program->setUniformValue(m_modelMatrixUniform, m_modelMatrix);

  m_viewMatrix.setToIdentity();
  m_viewMatrix.lookAt(QVector3D(0.0f,0.0f, 7.0f),
                      QVector3D(0.0f,0.0f, 0.0f),
                      QVector3D(0.0f,1.0f, 0.0f));
  m_program->setUniformValue(m_viewMatrixUniform, m_viewMatrix);

  updateProjection();

  // Nesse exemplo não temos conflitos entre diferentes rotinas
  // que também criam buffers opengl, mas é melhor evitar, não?
  m_program->disableAttributeArray(m_posAttr);
  m_program->disableAttributeArray(m_colAttr);
  m_vbo_pos.release();
  m_vbo_col.release();
  m_vao.release();
  m_program->release();

  // iniciar nosso temporizador
  m_timer.start();
}

// O que acontece aqui?
// - dizer que parte da tela vou desenhar (lembra do mario kart
//   64, quando tem 4 jogadores? A tela é dividida em 4 pedaços,
//   cada um mostrando, ao mesmo tempo, uma câmera diferente, que
//   seguem carrinhos diferentes (o glViewport permite isso)
// - limpar a tela (remover o efeito fantasma)
// - Usar o programa (poderíamos criar diferentes shaders para
//   efeito de chuva e outro para espelho, nesse caso só temos 1),
//   por isso precisamos associar o programa a ser usado.
// - representar a rotação do objeto e atualizá-lo na GPU através
//   da uniform correspondente (não estamos nem mudando projeção
//   nem a câmera).
// - Dizendo o formato dos atributos dos vértices:
//     - posição é 2D, float, sem normalização e entrelaçamento (quê???)
//     - cor é RGB, float, também sem normalização e entrelaçamento
// - Dizer o formato não significa usá-lo. Podemos "desligar" um atributo.
//   Nesse caso queremos usar a posição e a cor. Ex: textura e cor são mutua-
//   mente exclusivos, podemos ativar um e desativar outro.
// - Agora sim, desenhar de verdade (drawArrays ou drawElements) (UFA!!!!)
// - Boas práticas de programação: desativar, desalocar...
void RotatingSquareWindow::render(){
  const qreal retinaScale = devicePixelRatio();
  glViewport(0,0,width()*retinaScale, height()*retinaScale);
  glClear(GL_COLOR_BUFFER_BIT);
  m_program->bind();
  m_vao.bind();

  // Calcular o incremento do ângulo de acordo com o tempo e velocidade
  m_angle = m_angular_velocity * m_timer.elapsed()/1000;
  m_modelMatrix.rotate(m_angle, 0.0f, 0.0f, 1.0f);
  m_program->setUniformValue(m_modelMatrixUniform, m_modelMatrix);

  m_program->enableAttributeArray(m_posAttr);
  m_program->enableAttributeArray(m_colAttr);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  m_program->disableAttributeArray(m_posAttr);
  m_program->disableAttributeArray(m_colAttr);

  m_vao.release();
  m_program->release();
  m_frame++;

  // zerar para que o valor do timer seja exatamente a duração do frame
  m_timer.restart();
}

void RotatingSquareWindow::resizeEvent(QResizeEvent *event){
  QWindow::resizeEvent(event);
  if(m_program){
    updateProjection();
    renderNow();
  }
}

void RotatingSquareWindow::setVelocity(float value=50.0f){
  m_angular_velocity = value;
}

void RotatingSquareWindow::updateProjection(){
  m_program->bind();
  m_projectionMatrix.setToIdentity();

  // Sem isso, o quadrado não fica quadrado quando redimensiona
  // (tenta colocar `aspect = 1;` após esta linha
  float aspect = (float)width()/(float)height();

  // Para perspectiva
//  m_projectionMatrix.perspective(60.0f, aspect, 0.1f, 100.0f);

  // Para ortográfica
  if(aspect >= 1.0f)
    m_projectionMatrix.ortho(-10.0f*aspect,10.0f*aspect,-10.0f,10.0f,10.0f,-10.0f);
  else
    m_projectionMatrix.ortho(-10.0f,10.0f,-10.0f/aspect, 10.0f/aspect,10.0f,-10.0f);

  // Jogar a matriz para a GPU
  m_program->setUniformValue(m_projectionMatrixUniform, m_projectionMatrix);

  m_program->release();
}

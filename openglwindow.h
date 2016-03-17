
#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLPaintDevice>
#include <QtCore/QCoreApplication>
#include <QtGui/QPainter>

/**
 * @brief Classe que reúne as funções para OpenGL Core Profile (2.1+)
 *        e base para aplicações OpenGL
 *
 * Uma janela Qt precisa tratar eventos, renderizar-se, e guardar os
 * dispositivos de desenho (ele precisa ser informado como você vai
 * desenhar nele). Uma janela Qt com OpenGL precisa também
 * ser animada. Como essas tarefas são comuns a todas as aplicações
 * OpenGL em Qt, melhor fazer uma classe básica.
 */
class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  /**
   * @brief Toda vez que você criar um objeto, essa função será
   * chamada.
   *
   * Serve para inicializar variáveis (zerar algumas, ou deixar
   * valores padrão)
   *
   * @param parent Uma janela responsável pela OpenGLWindow. Se
   * A janela `parent` fechar, essa também fecha.
   */
  explicit OpenGLWindow(QWindow *parent = 0);

  /**
   * @brief Destrutor, serve para limpar memória
   */
  ~OpenGLWindow();

  /**
   * @brief Na hora que a janela for desenhada, essa função é
   *        chamada.
   *
   * `virtual` significa que qualquer aplicação OpenGL pode imple-
   * mentar sua própria rotina de desenho.
   *
   * @param painter círculos, retângulos, polígonos ou até mesmo os
   * próprios componentes GUI. O QPainter contém estas funções (não
   * ficaria coeso se tudo ficasse na QWindow). No nosso caso, o
   * OpenGL proverá um buffer representando a imagem final, e o
   * QPainter desenhará na QWindow esse buffer (não usaremos as
   * outras funções de desenho do QPainter).
   *
   */
  virtual void render(QPainter *painter);
  virtual void render();

  /**
   * @brief Nem toda a inicialização acontece ao criar o objeto.
   *
   * Pode ser que precisemos esperar rotinas externas para
   * continuar a inicialização. Cada aplicação personalizada
   * terá sua rotina (por isso o `virtual`)
   */
  virtual void initialize();

  /**
   * @brief Que tal brincar de estátua? É um flag para controlarmos
   *        o fluxo do desenho.
   * @param animating nosso flag que será guardado
   */
  void setAnimating(bool animating);

/*
 * Os componentes em um sistema se comunicam uns com os outros.
 * Às vezes um componente lança um evento (ex: salvei o arquivo,
 * pessoal!!!), denominada um sinal, daí você pode conectar
 * outros componentes para escutar esse evento e fazer tarefas
 * sincronizadas.
 *
 * A ordem é signal => slot, ou evento => função, de componentes
 * distintos ou do mesmo componente.
 */
public slots:
  /**
   * @brief Para eventos que podem esperar serem desenhados quando
   *        possível
   */
  void renderLater();
  /**
   * @brief Para apressados
   */
  void renderNow();

protected:
  /**
   * `renderLater` e `renderNow` não vão se chamar sozinhos.
   * Vamos tratar eventos de atualização da janela e chamar
   * nossas renderizações.
   */
  bool event(QEvent* event) Q_DECL_OVERRIDE;

  /**
   * Toda vez que a janela for mostrada, vamos também
   * chamar nosso `renderLater`
   */
  void exposeEvent(QExposeEvent *) Q_DECL_OVERRIDE;
private:
  /**
   * @brief É desperdício atualizar o desenho o tempo todo.
   * É melhor aguardar um evento de atualização.
   */
  bool m_update_pending;

  /**
   * @brief Pode até atualizar o desenho (talvez outra janela
   * tenha coberto uma parte), mas com o mesmo desenho.
   */
  bool m_animating;

  /**
   * @brief clearcolor, projection...
   */
  QOpenGLContext *m_context;
  /**
   * @brief Associa o contexto OpenGL com o QPainter
   */
  QOpenGLPaintDevice *m_device;
};

// Parece grande, mas se retirar os comentários, essa classe
// fica bem pequena (inicializar, renderizar, animar e eventos)

#endif // OPENGLWINDOW_H

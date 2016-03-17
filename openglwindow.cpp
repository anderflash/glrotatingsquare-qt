#include "openglwindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent)
  :QWindow(parent)
  ,m_update_pending(false)
  ,m_animating(false)
  ,m_context(0)
  ,m_device(0)
{
  // Tem o QWindow::RasterSurface se quiséssemos
  // utilizar diretamente as funções de desenho
  // no canvas da janela, mas queremos deixar
  // o trabalho para o OpenGL
  setSurfaceType(QWindow::OpenGLSurface);
}
void OpenGLWindow::render(QPainter *painter){
  // Nessa classe básica, não faz nada,
  // mas as aplicações sobrescreverão essa
  // função para desenhar o que quiserem.
  Q_UNUSED(painter);
}

// No cabeçalho tem uma expliccação legal dessa
// função tão void
void OpenGLWindow::initialize()
{
}

// Tem comandos que sempre aparecem na renderização.
// Vamos então deixar já na classe básica.
void OpenGLWindow::render(){
  // Criar nosso dispositivo OpenGL antes de
  // desenhar qualquer coisa
  if(!m_device)
    m_device = new QOpenGLPaintDevice;

  // Se não limpar o desenho, aparecerão fantoms
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Nosso glViewport
  m_device->setSize(size());

  // O QPainter pode desenhar o buffer do dispositivo
  // (OpenGL no caso) para o canvas da janela
  QPainter painter(m_device);

  // Agora sim, desenhar
  render(&painter);
}

// Enviar o sinal de que quer atualizar o desenho
void OpenGLWindow::renderLater(){
  // Esperar atualização se não estiver ainda
  if(!m_update_pending){
    m_update_pending = true;
    // Dizer ao sistema que modificou o buffer através de
    // um evento, agora é só a CPU/GPU ficar livre para desenhar
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
  }
  // Se já está esperando atualização, não faz nada
}

// Receber o sinal (meu ou de outro componente) da
// atualização do desenho
bool OpenGLWindow::event(QEvent *event){
  switch(event->type()){
  // Desenhar no evento certo
  case QEvent::UpdateRequest:
    m_update_pending = false;
    renderNow();
    return true;
  // Encaminhar corretamente os outros eventos
  default:
    return QWindow::event(event);
  }
}

// Ocultado, minimizado, maximizado, redimensionado... É muito
// sofrimento Para uma janela! Ele precisa ser desenhado, e
// o ambiente OpenGL também.
void OpenGLWindow::exposeEvent(QExposeEvent * event){
  Q_UNUSED(event);
  if(isExposed())
    renderNow();
}

// Reduzir a
void OpenGLWindow::renderNow(){
  // Pra que desenhar uma janela oculta?
  if (!isExposed())
    return;

  // Tem coisas que só serão feitas quando o contexto
  // for criado.
  bool needInitialize = false;

  // Mexer no OpenGL só com as funções do contexto
  if(!m_context){
    m_context = new QOpenGLContext(this);
    // Nível de Antialiasing fica codificado no
    // formato
    m_context->setFormat(requestedFormat());
    // A parte mais importante, equivalente ao
    // canvas.getContext no WebGL.
    // Tenta alugar um pequeno espaço no processo
    // OpenGL para realizar seus serviços. Sem essa
    // papelada, não dá para fazer nada.
    m_context->create();

    needInitialize = true;
  }
  m_context->makeCurrent(this);
  if(needInitialize){
    // Lembras do GLEW? É parecido
    initializeOpenGLFunctions();

    // Esqueceu que as aplicações precisam
    // inicializar suas variáveis? :P
    initialize();
  }

  // Qual o nome da função mesmo?
  render();

  // Buffer duplo, para evitar flickering
  m_context->swapBuffers(this);

  // Agendar uma próxima renderização.
  // Acho que isso significa animar, não?
  if(m_animating)
    renderLater();
}

// Permitir o usuário parar ou continuar a animação
void OpenGLWindow::setAnimating(bool animating){
  m_animating = animating;
  if(animating)
    renderLater();
}

// Destruindo tudo que foi criado
OpenGLWindow::~OpenGLWindow(){
  delete m_device;
  delete m_context;
}

# MAC 422 / 5477: Introdução à Computação Gráfica

Author: Anderson Tavares (NUSP 8102082)

## Rotating Square em Qt

Este é um exemplo de desenvolvimento em OpenGL sobre Qt.

### Características:

- Classe base para outras aplicações OpenGL em Qt
- Exemplo de aplicação girando um quadrado
- Utiliza o Core Profile

### Como compilar:

- Trabalhei com dois branches: [mostrar_janela](https://github.com/anderflash/glrotatingsquare-qt/tree/mostrar_janela), que tem apenas o código da classe básica e [mostrar_quadrado](https://github.com/anderflash/glrotatingsquare-qt/tree/mostrar_quadrado), que além da classe básica, contém a aplicação específica do movimento do quadrado. Você pode estudar o passo-a-passo.
- Baixar um deles, ou o [master](https://github.com/anderflash/glrotatingsquare-qt):
```bash
$ git clone https://github.com/anderflash/glrotatingsquare-qt.git
```
- Compilar

```bash
$ cd glrotatingsquare-qt
$ qmake && make
```
- Executar
```bash
$ ./rotatingSquare
```

### Dependências:

- Qt
- OpenGL (mesa, nvidia, catalyst...)

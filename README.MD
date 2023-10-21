## PROJETO FINAL - DISCIPLINA DE SISTEMAS EMBARCADOS ARQUITETURA E APLICAÇÕES

### Descritivo do projeto

O projeto consiste na criação de um adaptador para conectar um controle/joystick compatível com Atari (que possui um conector do tipo DB9) a uma porta USB de um computador.
Para isso foi utilizado um microcontrolador Raspberry Pi Pico, o pacote de desenvolvimento Pico-SDK e a biblioteca TinyUSB, além de alguns componentes auxiliares para a montagem do circuito.

Seguem imagens do projeto:
![Imagem do controle conectado à placa](IMGS/gamepad-atari_01.jpg?raw=true "Imagem do controle conectado à placa")

![Montagem dos Componentes na placa](IMGS/gamepad-atari_02.jpg?raw=true "Montagem dos Componentes na placa")

#### Compilando no Windows 
Para compilar o projeto no Windows, execute os seguintes comandos no diretório raiz do projeto

```console
mkdir build
cd build
cmake ..
ninja
```

#### Compilando em Sistemas Operacionais Baseados em Linux 
Para compilar o projeto em Sistemas Operacionais Baseados em Linux, execute os seguintes comandos no diretório raiz do projeto

```console
mkdir build
cd build
cmake ..
make
```
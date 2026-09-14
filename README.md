# Microprocessors & Microcontrollers

Repositório com projetos práticos, simulações e códigos. 
Este repositório serve como guia de estudos e base de consulta para modelagem de máquinas de estado em hardware, programação bare-metal para microcontroladores PIC18 e interfaceamento de periféricos no Proteus.

---

## Sumário

- [Visão Geral dos Módulos](#-visão-geral-dos-módulos)
- [1. Pattern Generator (Proteus)](#1-pattern-generator-proteus)
- [2. PIC18 Architecture & MPLAB](#2-pic18-architecture--mplab)
- [3. LCD Interfacing & Peripherals](#3-lcd-interfacing--peripherals)
- [Ferramentas Utilizadas](#️-ferramentas-utilizadas)
- [Como Executar as Simulações](#-como-executar-as-simulações)

---

## Visão Geral dos Módulos

```text
microprocessors-microcontrollers/
├── pattern-generator/      # FSMs em hardware, multiplexação e geração de padrões digitais
├── pic18/                  # Aplicações para microcontroladores PIC18 (C e Assembly)
└── lcd/                    # Interfaceamento e temporização com displays alfanuméricos
```

---

## 1. Pattern Generator (Proteus)

Implementação de circuitos digitais e máquinas de estados finitas (FSM) utilizando exclusivamente o instrumento **Pattern Generator** do Proteus VSM, explorando sincronismo, clock e transições sem o uso de código de programa tradicional.

### Demonstração em Vídeo
Confira a explicação detalhada de cada projeto no YouTube:  
[![Assista no YouTube](https://img.shields.io/badge/YouTube-Assistir%20Demonstração-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=PBoiozIbCmw)

> **Link direto:** [Pattern Generator - Proteus Projects (YouTube)](https://www.youtube.com/watch?v=PBoiozIbCmw)


### Projetos Desenvolvidos
- **Semáforo Inteligente:** Controle de cruzamento com temporização adaptável (modo normal vs. horário de pico selecionado por multiplexador) com sincronização por flip-flop D.
- **Relógio Digital de 6 Dígitos:** Contagem de horas, minutos e segundos em displays de 7 segmentos com ajuste manual de hora e minuto e lógica de trigger em cascata.
- **Letreiro de Matriz de LEDs:**
  - *Parte 1:* Exibição estática da letra inicial com varredura rápida de colunas e controle de linhas.
  - *Parte 2:* Efeito de texto contínuo em deslocamento (scroll horizontal) com compensação de *shadowing*.
- **Frequencímetro Digital (10 Hz a 99 Hz):** Medição de sinal TTL com taxa de erro <= 1%, utilizando mapeamento de posições de memória para períodos calculados.
- **Transmissor Serial UART:** Envio assíncrono de caracteres a 1200 baud (formato 8N1: 8 bits de dados, sem paridade, 1 stop bit).
- **Controlador de Servo Motor:** Modulação com 5 ângulos fixos (0°, 45°, 90°, 120° e 180°), calculando o passo de 15° por divisão de clock e offset de pulso.

---

## 2. PIC18 Architecture & MPLAB

Projetos embarcados desenvolvidos para a família **PIC18** (Microchip), explorando a manipulação direta de registradores de I/O, interrupções, timers e lógica de controle em baixo nível.

### Demonstração em Vídeo
[![Assista no YouTube](https://img.shields.io/badge/YouTube-Assistir%20Demonstração-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=3aw8htt9vh0)

> **Link direto:** [PIC 18 - Proteus Projects & MPLAB (YouTube)](https://www.youtube.com/watch?v=3aw8htt9vh0)


### Tópicos Abordados
- Configuração de pinos de propósito geral (`TRIS`, `PORT`, `LAT`).
- Temporização precisa via Hardware Timers (Timer0/Timer1).
- Rotinas de tratamento de interrupção (ISR).
- Simulação integrada do firmware compilado com o circuito esquemático no Proteus.

---

## 3. LCD Interfacing & Peripherals

Módulo dedicado à comunicação entre o microcontrolador e displays alfanuméricos (controlador padrão HD44780 ou equivalente), abordando sinais de controle, temporizações críticas e barramento de dados.

### Demonstração em Vídeo
[![Assista no YouTube](https://img.shields.io/badge/YouTube-Assistir%20Demonstração-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=MUeVrRSDSxg&t=25s)

> **Link direto:** [LCD - Interfacing & Demonstration (YouTube)](https://www.youtube.com/watch?v=MUeVrRSDSxg&t=25s)


### Tópicos Abordados
- Inicialização e envio de comandos/instruções para o controlador do display.
- Operação em modo 4-bits vs. 8-bits para economia de pinos de I/O.
- Temporização e pulsos na linha `Enable` (E) e chaveamento `RS` (Register Select).
- Exibição dinâmica de strings, caracteres customizados e leituras de sensores.

---

## Ferramentas Utilizadas

- **Labcenter Proteus VSM:** Esquemáticos e simulação interativa de hardware e sinais.
- **Microchip MPLAB X IDE / MPLAB IDE:** Ambiente de desenvolvimento e compilação do firmware.
- **Compiladores:** XC8 / C18 / MPASM.
- **Planilhas de Cálculo:** Dimensionamento temporal de padrões e frequências.

---

## Como Executar as Simulações

1. Clone o repositório:
   ```bash
   git clone https://github.com/raquelmcoelho/microprocessors-microcontrollers.git
   ```
2. Abra o arquivo `.pdsprj` correspondente ao módulo desejado no **Proteus**.
3. No caso dos projetos do **PIC18 / LCD**, certifique-se de que o caminho do binário compilado (`.hex` ou `.cof`) nas propriedades do componente no Proteus aponte para o arquivo gerado pelo MPLAB.
4. Execute a simulação interativa (botão *Play* no canto inferior esquerdo do Proteus).
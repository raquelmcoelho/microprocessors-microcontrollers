# Microprocessors & Microcontrollers - Proteus Projects

Projetos práticos e máquinas de estados desenvolvidos na disciplina de Microprocessadores e Microcontroladores (IFCE). Todas as soluções foram modeladas e simuladas no software Proteus ISIS, utilizando amplamente o componente virtual **Pattern Generator** como gerador de sequências digitais e controlador de estados sem a necessidade de lógica discreta complexa.

---

## Demonstração em Vídeo

Todas as simulações, arquiteturas e tabelas de estados estão explicadas e demonstradas no vídeo:

[![Assista no YouTube](https://img.shields.io/badge/YouTube-Assistir%20Demonstração-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=PBoiozIbCmw)

> **Link direto:** [Pattern Generator - Proteus Projects (YouTube)](https://www.youtube.com/watch?v=PBoiozIbCmw)

---

## Sumário
- [Guia Rápido: O que é o Pattern Generator?](#guia-rápido-o-que-é-o-pattern-generator)
- [Projetos Implementados](#projetos-implementados)
  - [1. Semáforo Duplo com Alternância de Tempo](#1-semáforo-duplo-com-alternância-de-tempo)
  - [2. Relógio Digital (6 Dígitos com Ajuste Manual)](#2-relógio-digital-6-dígitos-com-ajuste-manual)
  - [3. Letreiro em Matriz de LEDs (Estático e Rolagem)](#3-letreiro-em-matriz-de-leds-estático-e-rolagem)
  - [4. Frequencímetro Digital (10 Hz a 99 Hz)](#4-frequencímetro-digital-10-hz-a-99-hz)
  - [5. Transmissor Serial UART](#5-transmissor-serial-uart)
  - [6. Controlador de Servo Motor por Ângulo](#6-controlador-de-servo-motor-por-ângulo)
- [Como Simular no Proteus](#como-simular-no-proteus)
- [Repositórios Relacionados](#repositórios-relacionados)

---

## Guia Rápido: O que é o Pattern Generator?

Para quem é iniciante em simulação digital no Proteus, o **Pattern Generator** funciona como uma memória de padrões programável sequencial:

* **Buffer de Padrões:** Cada linha do componente armazena o nível lógico (0 ou 1) que será injetado em cada pino de saída.
* **Avanço por Clock:** A cada pulso de clock recebido, ele avança para o próximo endereço de memória e atualiza as saídas.
* **Pino de Trigger (Reset/Loop):** Permite configurar uma condição para reiniciar a varredura, criando ciclos infinitos ou máquinas de estados finitas.
* **Arquivos de Padrão (.ptn):** As tabelas de transição podem ser salvas, exportadas ou geradas via scripts e planilhas eletrônicas.

---

## Projetos Implementados

### 1. Semáforo Duplo com Alternância de Tempo
Controle de sinalização para cruzamento de duas vias com alternância entre fluxo normal e horário de pico.

* **Bases de Tempo:**
  * Modo Normal: período de 1,0 s por estado.
  * Modo Pico: período de 500 ms (dobro da velocidade).
* **Sincronismo e Continuidade:** A alternância do clock de entrada é feita por um Multiplexador acionado via Logic State. Para evitar glitches ou saltos indesejados durante a comutação, a saída passa por um **Flip-Flop tipo D** sincronizado diretamente com a mesma linha de clock do Pattern Generator.

---

### 2. Relógio Digital (6 Dígitos com Ajuste Manual)
Cronômetro/relógio completo com formato de 6 dígitos (HH:MM:SS) em displays de 7 segmentos.

* **Estrutura em Cascata:**
  * **Unidades (Segundos / Minutos):** Sequência de 0 a 9. Ao atingir o valor 9, emite pulso de trigger para a dezena.
  * **Dezenas (Segundos / Minutos):** Sequência de 0 a 5. O estado correspondente ao 6 dispara o reset imediato.
* **Controle de Horas:** Conta com botões para ajuste rápido de horas e minutos. O projeto explora duas soluções:
  1. *Clock Compartilhado:* Unidades e dezenas avançam em conjunto até 11/12 h (repetindo valores na dezena para casar os estados).
  2. *Trigger Desacoplado:* A dezena de horas só avança a cada 10 incrementos da unidade, mantendo os buffers de memória limpos e independentes.

---

### 3. Letreiro em Matriz de LEDs (Estático e Rolagem)
Controle visual em matriz de LEDs utilizando varredura matricial (multiplexação por colunas e linhas).

* **Parte 1 (Caractere Estático):** O gerador ativa uma coluna por vez em alta frequência (período de 1 ms). A taxa de atualização engana a visão humana pela persistência retiniana, parecendo um caractere fixo e estável. Um segundo Pattern Generator gera o loop de repetição.
* **Parte 2 (Letreiro Deslizante / Rolagem):** Desenho frame a frame da letra entrando pela borda, cruzando o display e saindo.
  * *Solução para Fantasmas (Ghosting):* Foi adicionado um ciclo vazio (espaçamento) entre a ativação das colunas adjacentes, eliminando sombras causadas pelo tempo de resposta dos LEDs.

---

### 4. Frequencímetro Digital (10 Hz a 99 Hz)
Instrumento virtual capaz de mensurar a frequência de ondas quadradas com erro de no máximo 1% (±1 Hz).

* **Base de Cálculo:**
  * Para medir até 100 Hz com erro inferior a 1%, o clock do Pattern Generator foi fixado em **10.000 Hz (10 kHz)**.
  * Cada pulso do gerador equivale a um período base de **0,1 ms (0,0001 s)**.
  * A frequência final exibida nos displays corresponde ao inverso do tempo acumulado pela onda de teste:  
    `Frequência (Hz) = 1 / Tempo Medido`
* **Indicação de Fora de Faixa:** Sinais abaixo de 10 Hz ou acima de 99 Hz acionam a mensagem de erro `EE` nos displays. O arquivo `.ptn` foi pré-calculado e populado via planilha eletrônica.

---

### 5. Transmissor Serial UART
Emulação por hardware de uma linha de transmissão serial assíncrona (TX) para envio cíclico de texto em ASCII.

* **Parâmetros da UART:**
  * Baud Rate: 1200 bps
  * Quadro de Dados: 8 bits de payload
  * Paridade: Nenhuma (None)
  * Bit de Parada: 1 Stop Bit
* **Protocolo de Linha:**
  * Repouso / Idle: nível lógico alto (1)
  * Start Bit: nível lógico baixo (0)
  * Dados: 8 bits enviados com LSB primeiro
  * Stop Bit: nível lógico alto (1)
* **Validação:** A saída digital do Pattern Generator foi conectada diretamente à porta de recepção (RX) do **Virtual Terminal** do Proteus.

---

### 6. Controlador de Servo Motor por Ângulo
Geração de sinal PWM para controle angular de servo motor em 5 posições pré-definidas: 0°, 45°, 90°, 120° e 180°.

* **Especificação do Sinal:**
  * Período do trem de pulsos: **20 ms** (frequência de 50 Hz).
  * Largura mínima do pulso (0°): **1,0 ms** (offset base).
  * Largura máxima do pulso (180°): **2,0 ms**.
  * Faixa útil de controle: **1,0 ms** distribuído linearmente entre 0° e 180°.
* **Discretização em 15 Graus:**
  * Maior divisor comum entre os ângulos desejados: 15°.
  * Total de divisões para 180°: 12 partes de 15°.
  * Resolução temporal por passo: `1 ms / 12 = 83,33 us`.
  * Frequência de clock adotada no gerador: **12.000 Hz (12 kHz)**.
* **Composição dos Estados:**
  * **0°:** Offset base (12 passos de 83,33 us = 1,0 ms)
  * **45°:** Offset + 3 passos (1,25 ms)
  * **90°:** Offset + 6 passos (1,50 ms)
  * **120°:** Offset + 8 passos (1,67 ms)
  * **180°:** Offset + 12 passos (2,00 ms)

---

## Como Simular no Proteus

1. Clone o repositório:
   ```bash
   git clone [https://github.com/raquelmcoelho/microprocessors-microcontrollers.git](https://github.com/raquelmcoelho/microprocessors-microcontrollers.git)
    ```

2. Abra o software Proteus ISIS (versão 8.0 ou superior).

3. Entre na pasta do projeto desejado e abra o arquivo de esquemático (.pdsprj ou .dsn).

4. Verifique se o componente Pattern Generator está com o arquivo .ptn correto vinculado em suas propriedades.

5. Clique no botão de Play (Simular) no canto inferior esquerdo.

---  
##  Repositórios Relacionados
[Digital Eletronics](https://github.com/raquelmcoelho/digital-eletronics): Circuitos de Eletrônica Digital e portas lógicas no Proteus.
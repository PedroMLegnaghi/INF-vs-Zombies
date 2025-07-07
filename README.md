# INF VS ZOMBIES 🎮🧟‍♂️

> Reimplementação do clássico *Plants vs Zombies*, feita em C utilizando a biblioteca [Raylib](https://www.raylib.com/).

🎥 **Gameplay disponível aqui:**  
[https://youtu.be/nMT4FZlNxA8](https://youtu.be/nMT4FZlNxA8)

---

## 🧠 Visão Geral

INF VS ZOMBIES é um jogo de defesa de torre no qual o jogador deve impedir que zumbis cheguem à sua base, utilizando plantas com habilidades especiais. O jogador coleta **sóis**, que são a moeda do jogo, e usa esses recursos para plantar defesas no campo.

O jogo termina quando:
- ✅ O jogador elimina todas as hordas de zumbis → vitória!
- ❌ Um zumbi alcança a base → derrota.

Há também um sistema de pontuação que determina os 5 melhores jogadores, salvo em `leaderboard.bin`.

---

## 🎮 Como Jogar

- O jogador começa com **0 sóis**.
- Para ganhar sóis, **clique nos sóis que caem do céu** ou use plantas do tipo `Sunflower`.
- Clique com o mouse na planta desejada ou pressione teclas numéricas `1` a `5` para selecionar plantas.
- Posicione as plantas no gramado para defender sua base.
- Para deselecionar uma planta, pressione a tecla `w` ou clique no `botão direito` do mouse.

### 🪴 Plantas disponíveis:
| Planta        | Função                                                                 |
|---------------|------------------------------------------------------------------------|
| 🌻 Sunflower   | Gera sóis periodicamente.                                              |
| 🌱 Peashooter  | Atira projéteis que causam dano aos zumbis.                            |
| 🥥 Wallnut     | Planta escudo com vida alta.                                           |
| 🥔 Potato Mine | Explode ao ser pisada por um zumbi, causando muito dano.              |
| 🧹 Shovel      | Remove plantas do campo (sem custo ou recarga).                        |

Cada planta tem **custo e tempo de recarga diferentes**. Não é possível usá-las se esses limites não forem respeitados.

---

## ⚙️ Detalhes Técnicos

- Resolução fixa: `1920x1080`
- Configuração de hordas: edite o arquivo `zombies.txt`.  
  Exemplo:
  ```
  5 6 7 3 8 9
  ```
  Isso define 6 hordas com essas quantidades de zumbis (primeira com 5 zumbis, segunda com 6 ... última com 9).
- A última horda inclui um **Boss**.
- Número máximo de hordas: 10.
- Para **resetar o placar**, delete o arquivo `leaderboard.bin`.
- Código foi feito buscando sempre documentação, organização, reutilização, clareza, modularização e eficiência adequada.

---

## 🖥️ Execução

- Basta rodar o executável:

```bash
./main.exe
```

**OBS**: Alterações no `zombies.txt` **não requerem recompilação**.

---

## 🔊 Sons

Efeitos sonoros foram adicionados:
- Coleta de sóis
- Disparo de ervilhas
- Zumbis

É possível desativá-los individualmente na aba de configurações.

---

## 🎬 Animações

- Plantas com animações:
  - Sunflower 🌻
  - Peashooter 🌱
- As demais plantas são desenhadas estaticamente.

---

## 🧟 Tipos de Zumbis

Existem **6 tipos de zumbis** com variações de:
- Vida
- Dano
- Velocidade
- Tamanho

**Zombie Yeti**: ao morrer, ele zera seus sóis e remove até 10 plantas.

---

## 📋 Menus e Interface

- Tela inicial com opções.
- Tela "About" e "Configurações".
- **Menu durante o jogo** pausa a partida corretamente (sem alterar o tempo das ações das plantas).
- Confirmação de saída em qualquer tela.
- Dificuldade aumenta com o tempo: o intervalo de surgimento dos zumbis diminui, e o tempo de geração de sóis aumenta.

---

## 👨‍💻 Créditos

Este projeto foi desenvolvido como parte de um trabalho da disciplina `Algorítmos e Programação` da `Universidade Federal do Rio Grande do Sul (UFRGS)`, com o objetivo de aplicar conceitos de programação em C e uso da biblioteca Raylib.

---

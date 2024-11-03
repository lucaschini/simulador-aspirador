# Simulador de Aspirador de Pó

Este projeto é uma implementação em C de um simulador de aspirador de pó, um problema clássico no estudo de Inteligência Artificial. O simulador é projetado para demonstrar diferentes cenários em que um agente aspirador pode operar, utilizando algoritmos apropriados para cada situação.

## Ambientes

O simulador possui dois ambientes principais:

1. **Ambiente Observável**:
   - O agente tem acesso a todas as informações sobre o ambiente, incluindo a localização de sujeira e sua própria posição.
   - Algoritmo Utilizado: **A\***, que é ideal para encontrar o caminho mais curto para limpar a sujeira, considerando um estado completo do ambiente.

2. **Ambiente Parcialmente Observável**:
   - O agente tem informações limitadas, sabendo apenas sobre a sala em que se encontra e se há sujeira presente.
   - Algoritmo Utilizado: **Minimax com Heurísticas**, onde o agente avalia as possíveis jogadas para otimizar a limpeza, mesmo com informações incompletas.

3. **Ambiente Parcialmente Observável B**:
   - O agente conhece informações sobre as salas adjacentes, permitindo um pouco mais de contexto do ambiente.
   - Algoritmo Utilizado: **Busca em Largura**, que permite explorar as salas vizinhas de maneira eficiente, mesmo com incerteza.

## Funcionalidades

- O usuário pode definir a quantidade de salas sujas.
- A posição inicial do robô e das sujeiras é gerada aleatoriamente.
- O agente pode ser controlado manualmente ou por IA em todos os ambientes.
- Cada ambiente possui uma controladora (IA) específica que soluciona o problema da maneira mais adequada.

# Projeot-ED

Projeto acadêmico desenvolvido por Álex Matheus Coleta Santos, Sibele Oliveira Cruz e Wanessa Silva Santos durante o curso da matéria de Estrutura da Dados, do Departamento de Computação da  Universidade Federal de Sergipe, sob a orientação do Prof. Dr. Gilton José Ferreira da Silva.

O projeto trata-se de um sistema para a gestão administrativa e operacional de uma concessionária de veículos. O foco é organizar o fluxo de trabalho, desde a chegada de clientes e o histórico de ações do sistema até o controle individual de desempenho dos vendedores e registros detalhados de vendas.

O programa é uma aplicação de terminal que permite gerenciar as entidades fundamentais de uma concessionária. As principais funcionalidades incluem:
Gestão de Vendedores: Cadastro e remoção de funcionários com controle por matrícula única.
Controle de Vendas: Registro de transações vinculadas a cada vendedor (incluindo marca, modelo, ano e nota fiscal).
Fila de Atendimento: Gerenciamento de clientes em espera por ordem de chegada.
Relatórios e Logs: Visualização de faturamento global, identificação do vendedor mais lucrativo, pesquisa de vendas por marca e um histórico (log) das últimas ações realizadas no sistema.
Metas: Exibição de projeções financeiras trimestrais.

Dos requisitos mínimos solicitados:
Lista Circular Simples: utilizada no Cadastro de Vendedores, permite percorrer todos os vendedores de forma cíclica, facilitando a busca e listagem contínua a partir de qualquer ponto.
Lista Duplamente Encadeada: utilizada no Registro de Vendas, possibilita que cada vendedor possua uma lista de suas vendas. O encadeamento duplo facilita a navegação entre registros anteriores e próximos, além de simplificar a remoção de dados.
Fila (FIFO): utilizada para a função de Atendimento de Clientes, irá garantir que o primeiro cliente a chegar seja o primeiro a ser atendido, respeitando a ordem cronológica (First-In, First-Out).
Pilha (LIFO): utilizada para a função de Histórico de Logs, as ações mais recentes são empilhadas no topo, permitindo que o usuário visualize o histórico da última alteração para a primeira.
Array Estático: utilizado para definir as Metas Trimestrais, que contam com valores fixos e de acesso rápido que não precisam de redimensionamento durante a execução.


Instruções de Execução

Pré-requisitos

Compilador GCC: Certifique-se de que o GCC está instalado e configurado no seu PATH (conforme as etapas realizadas anteriormente com o MSYS2 ou MinGW).
Compilação via Terminal
Abra o terminal na pasta onde o arquivo está salvo.
No terminal, digite:  gcc “NomeDoArquivo”.c -o “NomeExecutavel”.exe
Será gerado um arquivo executável com o nome escolhido
Nos seus arquivos, execute o arquivo gerado.
Siga as instruções pelo menu disponível

No VS Code ou Geany (compiladores utilizados pelo grupo)
VS Code: Pressione F5 (se o launch.json estiver configurado) ou use o botão de "Play" da extensão C/C++ Runner.

Geany: Clique no ícone de Construir (parede de tijolos) e depois no ícone de Executar (engrenagens).

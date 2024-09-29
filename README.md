# PROJETO SOCKETS - SIMULADOR DE MPI_REDUCE()

## DESCRICÃO
Este projeto é uma implementação de uma simulação do algoritmo MPI_REDUCE utilizando sockets TCP/IP em C. A ideia central é dividir a soma de um conjunto de números entre diferentes processos (chamados de workers), que executam em paralelo, seguindo uma estrutura de árvore binária.
### Funcionalidades
- Arquitetura Paralela: Utiliza uma abordagem de árvore binária, na qual os workers realizam a redução de valores de maneira hierárquica, até que o resultado final seja obtido.
- Comunicação via Sockets: O envio e recebimento de dados entre os processos é feito através de sockets TCP/IP, simulando a comunicação ponto-a-ponto entre os workers e o manager.
- Escalabilidade: O número de workers pode ser configurado facilmente, permitindo a execução em diferentes tamanhos de árvore binária para paralelismo.
- Sincronização: Implementação de sincronização para garantir que os workers trabalhem em conjunto e enviem dados ao manager somente após a conclusão da operação de redução.

## CONFIGURÇÃO

Você deve ter instalado no linux o gcc e o build-essentials para rodar o processo.
Para limpar os arquivos executáveis basta digitar
	
	make clean

Para buildar o trabalho basta digitar
	
	make build
	
Para executar o trabalho basta digitar
	
	make exec
	
Após executar o os processos via make exec, espere que o manager ira printar o resultado


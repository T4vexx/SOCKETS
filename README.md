# PROJETO SOCKETS - SIMULADOR DE MPI_REDUCE()

## DESCRICÃO

O projeto foi desenvolvido em C com o intuido de imitar um MPI_REDUCE(), nesse projeto usamos 8 processos diferentes para somar um array de 8 valores.
Os valores são divididos em 8 processos, que usando o método de sincronismo borboleta, soma os valores e devolve para o servidor central que printa o valor da soma.

## CONFIGURÇÃO

Você deve ter instalado no linux o gcc e o build-essentials para rodar o processo.
Para limpar os arquivos executáveis basta digitar
	
	make clean

Para buildar o trabalho basta digitar
	
	make build
	
Para executar o trabalho basta digitar
	
	make exec
	
Após executar o os processos via make exec, espere que o manager ira printar o resultado


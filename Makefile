clean: 
	rm -f manager worker

build_worker: 
	gcc -o worker worker.c -lm

build_manager:
	gcc -o manager manager.c

build: build_worker build_manager

exec_proccess:
	bash exec_trabalho.sh

exec: exec_proccess
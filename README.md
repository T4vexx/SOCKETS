# SOCKETS PROJECT - MPI_REDUCE() SIMULATOR

## DESCRIPTION
This project is an implementation of an MPI_REDUCE algorithm simulation using TCP/IP sockets in C. The main idea is to distribute the sum of a set of numbers across different processes (called workers) that execute in parallel, following a binary tree structure.

### Features
- Parallel Architecture: Uses a binary tree approach, where workers hierarchically perform value reduction until the final result is obtained.
- Socket Communication: Data transmission between processes is done via TCP/IP sockets, simulating point-to-point communication between workers and the manager.
- Scalability: The number of workers can be easily configured, allowing execution with different binary tree sizes for parallelism.
- Synchronization: Implements synchronization to ensure that workers operate in coordination and send data to the manager only after completing the reduction operation.

## CONFIGURATION

You must have GCC and build-essentials installed on your Linux system to run the process.

To clean executable files, run:
	
	make clean

To build the project, run:
	
	make build
	
To execute the project, run:
	
	make exec
	
After executing the processes via make exec, wait for the manager to print the final result.

![image](https://github.com/user-attachments/assets/a8db22d3-f8f9-4f6f-a949-82f390cfad22)

### Considerações
The worker initialization format is as follows: 
```Bash
	./worker <id_do_worker_começando_pelo_0> <valor_do_woker_para_ser_somado>
```
In the logic used to spawn worker processes, the exec_trabalho.sh script starts all workers with the value 10.
If you want to change the values used for summation, simply modify this logic:

	for i in {0..7}
	do
		./worker $i 10 &
		if [ $? -ne 0 ] then
			echo "Erro ao iniciar o processo worker $i"
			exit 1
		fi
		sleep 1
	done

Thus, in the current logic, **the correct answer is 80.** 🚀

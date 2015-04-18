#include "tasks.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <assert.h>

using namespace std;

void TaskConsola(int pid, vector<int> params) {
	for (int i = 0; i < params[0]; i++) {
		int duracion = rand()%(params[2]-params[1]) +params[1];	// número aleatorio entre dos números
		uso_IO(pid, duracion);
	}
	
}
void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConBloqueo(int pid, vector<int> params) {
	int tiempo = params[0];
	int inicioBloq = params[1];
	int finBloq = params[2];

	if(inicioBloq > tiempo){
		uso_CPU(pid, tiempo);
	}else{
		uso_CPU(pid,inicioBloq-2);
		uso_IO(pid,finBloq-inicioBloq+1);
		uso_CPU(pid,tiempo-finBloq);
	}
}

void TaskBatch(int pid,std::vector<int> params){

	unsigned int total_cpu = (unsigned int) params[0];
	unsigned int cant_bloqueos = (unsigned int) params[1];

	unsigned int tiempo_de_lanzamiento_IO = cant_bloqueos;
	/* Tiempo solicitado de CPU es insuficiente para la cantidad de IO pedidos*/
	assert(total_cpu>=tiempo_de_lanzamiento_IO); 

	unsigned int uso_cpu_sin_bloqueos = total_cpu - tiempo_de_lanzamiento_IO;
	unsigned int* momentos_de_bloqueo = new unsigned int [cant_bloqueos];
	for(unsigned int i = 0;i<cant_bloqueos;i++){
		momentos_de_bloqueo[i] = rand()%(uso_cpu_sin_bloqueos+1);
	}
	sort(momentos_de_bloqueo,momentos_de_bloqueo+cant_bloqueos);

	unsigned int ultimo_bloqueo = 0;
	for(unsigned int i = 0;i<cant_bloqueos;i++){
		unsigned int tiempo_cpu = momentos_de_bloqueo[i] - ultimo_bloqueo;
		if(tiempo_cpu!=0){
			uso_CPU(pid,tiempo_cpu);
		}
		uso_IO(pid,1);
		ultimo_bloqueo = momentos_de_bloqueo[i];
	}
	if(ultimo_bloqueo < uso_cpu_sin_bloqueos){
		uso_CPU(pid,uso_cpu_sin_bloqueos - ultimo_bloqueo);
	}

	delete[](momentos_de_bloqueo);

	return;
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConBloqueo,3);
	register_task(TaskConsola,3);
	register_task(TaskBatch,2);
}

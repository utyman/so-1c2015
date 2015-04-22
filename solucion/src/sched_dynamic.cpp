#include "sched_dynamic.h"
#include <iostream>

using namespace std;

SchedDynamic::SchedDynamic(vector<int> argn) {
	unsigned int cantidadCores = argn[0];

	for (unsigned int i = 0; i < cantidadCores; i++) { // guardo la información de quantums
		contadorQuantums.push_back(argn[1+i]);
		contadorQuantumsOriginal.push_back(argn[1+i]);
	}

}

SchedDynamic::~SchedDynamic() {
}

void SchedDynamic::initialize() {
}

void SchedDynamic::load(int pid) {
	tareas.push_back(pid);		// la agrego para indicar que existe esta tarea
	tiempoFaltante.push_back(period(pid));	// guardo el periodo, falta periodo para overflow
	habilitadas.push_back(1);	// indico que esta en ready
}

void SchedDynamic::unblock(int pid) {
	for (unsigned int i = 0; i < habilitadas.size(); i++) { // habilito a la tarea desbloqueada
		if (tareas[i] == pid) {
			habilitadas[i] = 0;
		}
	}
}

// devuelve la tarea prioritaria
int SchedDynamic::obtenerTareaPrioritaria() {
	int tareaPrioritaria = IDLE_TASK;
	int minimoFaltanteOverflow = -9999;
	for (unsigned int i = 0; i < tareas.size(); i++) { // itero por todas las tareas habilitadas
		if ((habilitadas[i] == 1 && tiempoFaltante[i] < minimoFaltanteOverflow) || minimoFaltanteOverflow == -9999) {
			tareaPrioritaria = tareas[i];
			minimoFaltanteOverflow = tiempoFaltante[i];
		}
	}
	return tareaPrioritaria;	
} 

int SchedDynamic::tick(int cpu, const enum Motivo m) {
	 int pid = current_pid(cpu);
	for (unsigned int i = 0; i < tareas.size(); i++) { // reduzco la cantidad de ticks para el overflow
		if (habilitadas[i] == 1) {
			tiempoFaltante[i] = tiempoFaltante[i]-1;
		}
	}

	if (m == EXIT) {
		contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // actualizo el contador de quantums
		for (unsigned int i = 0; i < tareas.size(); i++) { // reduzco la cantidad de ticks para el overflow
			if (tareas[i] == pid) {
				habilitadas[i] = 0;
			}
		}

		return obtenerTareaPrioritaria();
	} 
	if (m == BLOCK) {
		contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // actualizo el contador de quantums
		for (unsigned int i = 0; i < tareas.size(); i++) { // reduzco la cantidad de ticks para el overflow
			if (tareas[i] == pid) {
				habilitadas[i] = 0;
			}
		}

		return obtenerTareaPrioritaria();
	}
	if (m == TICK) {
		contadorQuantums[cpu] = contadorQuantums[cpu]-1;
		if (contadorQuantums[cpu] != 0 && current_pid(cpu) != IDLE_TASK) { // si no se acabo el quantum lo bajo y no cambio el proceso
			return current_pid(cpu);
		} else {
			contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // reseto el quantu original
		}
		return obtenerTareaPrioritaria();

	}
	return IDLE_TASK;
}


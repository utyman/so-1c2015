#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	unsigned int cantidadCores = argn[0];
	for (unsigned int i = 0; i < cantidadCores; i++) {
		contadorQuantums.push_back(argn[1+i]);
		contadorQuantumsOriginal.push_back(argn[1+i]);
	}

}

SchedRR::~SchedRR() {
}


void SchedRR::load(int pid) {
	q.push(pid); // llegó una tarea nueva
}

void SchedRR::unblock(int pid) {

	q.push(pid); // vuelve a la cola de ready
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	if (m == EXIT) {
		contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // actualizo el contador de quantums
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.front(); q.pop();
			return sig;
		}
	} 
	if (m == BLOCK) {
		contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // actualizo el contador de quantums
		if (!q.empty()) {
			int sig = q.front(); q.pop();
			return sig;
		} else {
			return IDLE_TASK; // si el único proceso está bloquedo
					  // ejecuta IDLE_TASK
		}
	}
	if (m == TICK) {
		contadorQuantums[cpu] = contadorQuantums[cpu]-1;
		if (contadorQuantums[cpu] != 0 && current_pid(cpu) != IDLE_TASK) { // si no se acabo el quantum lo bajo y no cambio el proceso
			return current_pid(cpu);
		} else {
			contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // reseto el quantu original
		}
		if (!q.empty()) {
			int sig = q.front(); q.pop();
			int des = current_pid(cpu);
			if (des != IDLE_TASK) {
				q.push(des); // vuelvo a encolar el proceso desalojado
			}
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
	return IDLE_TASK;
}

int SchedRR::next(int cpu) {
	return -2;
}

#include "sched_fixed.h"
#include <iostream>

using namespace std;



SchedFixed::SchedFixed(vector<int> argn) {
	// guardo la informacion de los quantums
	unsigned int cantidadCores = argn[0];
	for (unsigned int i = 0; i < cantidadCores; i++) {
		contadorQuantums.push_back(argn[1+i]);
		contadorQuantumsOriginal.push_back(argn[1+i]);
	}
}

SchedFixed::~SchedFixed() {

}

void SchedFixed::initialize() {
}

void SchedFixed::load(int pid) {
	TaskComparable tsk;
	tsk = TaskComparable(pid, period(pid));
	q.push(tsk); // llegó una tarea nueva
}

void SchedFixed::unblock(int pid) {
	TaskComparable tsk;
	tsk = TaskComparable(pid, period(pid));
	q.push(tsk); // llegó una tarea nueva
}

int SchedFixed::tick(int cpu, const enum Motivo m) {
	if (m == EXIT) {
		contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // actualizo el contador de quantums
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.top().get_pid(); q.pop();
			return sig;
		}
	} 
	if (m == BLOCK) {
		contadorQuantums[cpu] = contadorQuantumsOriginal[cpu]; // actualizo el contador de quantums

		if (!q.empty()) {
			int sig = q.top().get_pid(); q.pop();
			return sig;
		} else {
			return IDLE_TASK; // si no hay tareas encoladas
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
			int des = current_pid(cpu);
			if (des != IDLE_TASK) {
				TaskComparable tsk;
				tsk = TaskComparable(des, period(des));
				q.push(tsk); // vuelvo a encolar la tarea que esta ejecutando
			}
			int sig = q.top().get_pid(); q.pop();
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
	return IDLE_TASK;
}


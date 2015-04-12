#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
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
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.front(); q.pop();
			return sig;
		}
	} 
	if (m == BLOCK) {
		if (!q.empty()) {
			int sig = q.front(); q.pop();
			return sig;
		} else {
			return IDLE_TASK; // si el único proceso está bloquedo
					  // ejecuta IDLE_TASK
		}
	}
	if (m == TICK) {
		if (!q.empty()) {
			int sig = q.front(); q.pop();
			int des = current_pid(cpu);
			q.push(des); // vuelvo a encolar el proceso desalojado
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
}

int SchedRR::next(int cpu) {
}

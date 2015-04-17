#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	unsigned int cantidadCores = argn[0];
	for (unsigned int i = 0; i < cantidadCores; i++) {
		queue<int> pids;
		vector<int> procesos;
		qs.push_back(pids);
		nucleos.push_back(procesos);
	}
}

SchedRR2::~SchedRR2() {

}

bool contiene(vector<int> nucleo, int pid) {
	for (unsigned int i = 0; i < nucleo.size(); i++) {
		if(nucleo[i] == pid) {
		  return true;
		}
	}
	return false;
}

void SchedRR2::load(int pid) {
	unsigned int nucleoMenosCongestionado = 0;
	unsigned int cantidadEnNucleoMenosCongestionado = qs[0].size();
	for (unsigned int i = 0; i < qs.size(); i++) {
		if (qs[i].size() < cantidadEnNucleoMenosCongestionado) {
			nucleoMenosCongestionado = i;
			cantidadEnNucleoMenosCongestionado = qs[i].size();
		}
	}

	qs[nucleoMenosCongestionado].push(pid);

}

void SchedRR2::unblock(int pid) {
	for (unsigned int i = 0; i < nucleos.size(); i++) {
		if(contiene(nucleos[i], pid)) {
			qs[i].push(pid); // vuelve a la cola de ready de su núcleo
			return;		
		} 
	} 
}



int SchedRR2::tick(int cpu, const enum Motivo m) {
	if (m == EXIT) {
		// Si el pid actual terminó, sigue el próximo.
		if (qs[cpu].empty()) return IDLE_TASK;
		else {
			int sig = qs[cpu].front(); qs[cpu].pop();
			return sig;
		}
	} 
	if (m == BLOCK) {
		if (!qs[cpu].empty()) {
			int sig = qs[cpu].front(); qs[cpu].pop();
			return sig;
		} else {
			return IDLE_TASK; // si el único proceso está bloquedo
					  // ejecuta IDLE_TASK
		}
	}
	if (m == TICK) {
		if (!qs[cpu].empty()) {
			int sig = qs[cpu].front(); qs[cpu].pop();
			int des = current_pid(cpu);
			qs[cpu].push(des); // vuelvo a encolar el proceso desalojado
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
	return current_pid(cpu);
}



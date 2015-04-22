#ifndef __SCHED_DYNAMIC_
#define __SCHED_DYNAMIC_

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

class SchedDynamic : public SchedBase {
	public:
		SchedDynamic(std::vector<int> argn);
        	~SchedDynamic();
		virtual void initialize();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		std::vector<int> contadorQuantums; // se usa para controlar los quantums
		std::vector<int> contadorQuantumsOriginal; // guardo la cantidad de quantums de cada nucleo
		std::vector<int> tareas; // guarda las ids de las tareas que se estan ejecutando
		std::vector<int> tiempoFaltante; // guarda lo que le falta a cada tarea para el overflow
		std::vector<int> habilitadas; // con 0 indica que la tarea no esta READY con 1 en READY

};

#endif

#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        	~SchedRR2();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		std::vector< std::queue<int> > qs; // guarda las colas de prioridades asignadas a cada nuclo
		std::vector< std::vector<int> > nucleos; // guarda las tareas asignadas a cada nucleo
		int next(int cpu);
};

#endif

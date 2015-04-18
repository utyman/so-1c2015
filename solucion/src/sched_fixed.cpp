#include "sched_fixed.h"
#include <iostream>

using namespace std;



SchedFixed::SchedFixed(vector<int> argn) {
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
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.top().get_pid(); q.pop();
			return sig;
		}
	} 
	if (m == BLOCK) {
		if (!q.empty()) {
			int sig = q.top().get_pid(); q.pop();
			return sig;
		} else {
			return IDLE_TASK; // si el único proceso está bloquedo
					  // ejecuta IDLE_TASK
		}
	}
	if (m == TICK) {
		if (!q.empty()) {
			int sig = q.top().get_pid(); q.pop();
			int des = current_pid(cpu);
			if (des != IDLE_TASK) {
				TaskComparable tsk;
				tsk = TaskComparable(des, period(des));
				q.push(tsk); // llegó una tarea nueva
			}
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
	return IDLE_TASK;
}


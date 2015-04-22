#ifndef __SCHED_FIXED_
#define __SCHED_FIXED_

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

class TaskComparable
{
  public:
    TaskComparable() {};                                      //constructor default
    TaskComparable(int pid, int priority) { this->pid = pid; this->period = period; }    //constructor
    bool operator<(const TaskComparable& right) const{
    	return (right.period)  < (this->period);
    }

    int get_pid() const { return pid; }             //accessor methods
    int get_period() const { return period; }

  private:
    int pid, period;                                 //data fields
};

class SchedFixed : public SchedBase {
	public:
		SchedFixed(std::vector<int> argn);
        	~SchedFixed();
		virtual void initialize();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		std::priority_queue<TaskComparable> q;
		std::vector<int> contadorQuantums; // se usa para controlar los quantums
		std::vector<int> contadorQuantumsOriginal; // guardo la cantidad de quantums de cada nucleo
		
};


#endif

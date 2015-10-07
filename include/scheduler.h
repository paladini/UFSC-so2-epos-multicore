#ifndef __scheduler_h
#define __scheduler_h

#include <utility/list.h>

__BEGIN_SYS

class Priority : public List_Element_Rank {
public:
	Priority(int p = NORMAL) : List_Element_Rank(p){}

	 enum {
		MAIN   = 0,
		HIGH   = 1,
		NORMAL = (unsigned(1) << (sizeof(int) * 8 - 1)) - 4,
		LOW    = (unsigned(1) << (sizeof(int) * 8 - 1)) - 3,
		IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 2
	};

	static const bool preemptive = true;
};

class RoundRobin: public List_Element_Rank
{
public:
	enum {
		MAIN   = 0,
		NORMAL = (unsigned(1) << (sizeof(int) * 8 - 1)) - 4,
		IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 2
	};

	static const bool preemptive = true;

public:
	RoundRobin(int p = NORMAL): List_Element_Rank(p) {}
};

template<typename T>
class Scheduler : public Scheduling_List<T> {};

__END_SYS



#endif

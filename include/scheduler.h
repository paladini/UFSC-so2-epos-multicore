#ifndef __scheduler_h
#define __scheduler_h
#include <utility/queue.h>
#include <utility/list.h>
#include <cpu.h>
#include <machine.h>
#include "thread.h"
#include "priority.h"

#define CRITERIA typename T::Criterion

__BEGIN_SYS
__USING_UTIL

// precisa funcionar p/ round-robin e prioridade.
// pensamos em criar um Scheduler que teria um scheduler 
// (ou uma fila) de suspensos, mas isso ia tirar a atomicidade do escalonador, que
// deve ser genérico e aplicado de forma específica somente por quem o utiliza.
template<typename T, typename R = CRITERIA>
class Scheduler: public Scheduling_List<T>
{

public:
    typedef Scheduling_List<T> List;
    typedef typename List::Element Element;

    Scheduler() {}
    ~Scheduler() {}

    void insert(const T &t) const { List::insert(t._link); }
    Element* remove(const T &t) const {	return List::remove(t._link); }
    Element* remove() const { return List::remove(); }
    void suspend(const T &t) const { _suspended.insert(remove(t._link)); }
    void resume(const T &t) const { List::insert(_suspended.remove(t._link)); }
    // Element* choosen() { return List::} how to implement?
    // void choose_next() { return List:: } how to implement?

    static const bool preemptive = Traits<T>::preemptive || false; // to be fixed.

private:
	Queue<T> _suspended;

};

__END_SYS

#endif
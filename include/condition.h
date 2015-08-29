// EPOS Condition Variable Abstraction Declarations

#ifndef __condition_h
#define __condition_h

#include <synchronizer.h>

__BEGIN_SYS

// This is actually no Condition Variable
// check http://www.cs.duke.edu/courses/spring01/cps110/slides/sem/sld002.htm
class Condition: protected Synchronizer_Common
{
public:
    Condition();
    ~Condition();

    void wait();
    void signal();
    void broadcast();
};

// This is an alternative implementation, which does impose ordering
// on threads waiting at "wait". Nontheless, it's still susceptible to counter
// overflow
// class Condition: public Synchronizer_Common
// {
// private:
//     typedef Traits<Synchronizer> Traits;
//     static const Type_Id TYPE = Type<Condition>::TYPE;

// public:
//     Condition() : _wait(0), _signal(0) {
// 	db<Synchronizer>(TRC) << "Condition() => " << this << "\n";
//     }
//     ~Condition() {
// 	db<Synchronizer>(TRC) << "~Condition(this=" << this << ")\n";
//     }

//     void wait() {
// 	db<Synchronizer>(TRC) << "Condition::wait(this=" << this 
// 			      << ",wt=" << _wait
// 			      << ",sg=" << _signal << ")\n";
// 	int rank = finc(_wait);
// 	while(rank >= _signal)
// 	    sleep();
//     }
//     void signal() {
// 	db<Synchronizer>(TRC) << "Condition::signal(this=" << this 
// 			      << ",wt=" << _wait
// 			      << ",sg=" << _signal << ")\n";
// 	finc(_signal);
// 	wakeup();
//     }
//     void broadcast() { // warning: overflow is not being handled!
// 	db<Synchronizer>(TRC) << "Condition::broadcast(this=" << this 
// 			      << ",wt=" << _wait
// 			      << ",sg=" << _signal << ")\n";
// 	_signal = _wait + 1;
// 	wakeup_all();
//     }

// static int init(System_Info * si);

// private:
//     volatile int _wait;
//     volatile int _signal;
// };

// This is an alternative implementation, which does not impose any ordering
// on threads waiting at "wait". In this case, ordering would be implemented
// in "sleep" and "wakeup" through ordered queues.
// class Condition: public Synchronizer_Common
// {
// private:
//     typedef Traits<Condition> Traits;
//     static const Type_Id TYPE = Type<Condition>::TYPE;

// public:
//     Condition() : _not_condition(true), _broadcast(0), _time_stamp(1) {
// 	db<Condition>(TRC) << "Condition() => " << this << ")\n";
//     }
//     ~Condition() {
// 	db<Condition>(TRC) << "Condition() => " << this << ")\n";
//     }

//     void wait() {
// 	db<Condition>(TRC) << "Condition::wait(this=" << this 
// 			   << ",ts=" << _time_stamp << ")\n";
// 	int ts = finc(_time_stamp);
// 	while(tsl(_not_condition) && (ts > _broadcast))
// 	    sleep();
//     }
//     void signal() {
// 	db<Condition>(TRC) << "Condition::signal(this=" << this 
// 			   << ",ts=" << _time_stamp << ")\n";
// 	_not_condition = false;
// 	wakeup();
//     }
//     void broadcast() {
// 	db<Condition>(TRC) << "Condition::broadcast(this=" << this 
// 			   << ",ts=" << _time_stamp << ")\n";
// 	_broadcast = finc(_time_stamp);
// 	wakeup_all();
//     }

//     static int init(System_Info * si);

// private:
//     volatile bool _not_condition;
//     volatile int _broadcast;
//     volatile int _time_stamp;
// };

__END_SYS

#endif



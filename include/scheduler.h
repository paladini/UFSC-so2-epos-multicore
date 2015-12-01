// EPOS Scheduler Abstraction Declarations

#ifndef __scheduler_h
#define __scheduler_h

#include <utility/list.h>
#include <cpu.h>
#include <machine.h>

__BEGIN_SYS

// All scheduling criteria, or disciplines, must define operator int() with
// the semantics of returning the desired order of a given object within the
// scheduling list
namespace Scheduling_Criteria
{
    // Priority (static and dynamic)
    class Priority
    {
    public:
        enum {
            MAIN   = 0,
            HIGH   = 1,
            NORMAL = (unsigned(1) << (sizeof(int) * 8 - 1)) - 3,
            LOW    = (unsigned(1) << (sizeof(int) * 8 - 1)) - 2,
            IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 1
        };

        static const bool timed = false;
        static const bool dynamic = false;
        static const bool preemptive = true;

    public:
        Priority(int p = NORMAL): _priority(p) {}

        operator const volatile int() const volatile { return _priority; }

        void update() {}

    protected:
        volatile int _priority;
    };

    // Round-Robin
    class RR: public Priority
    {
    public:
        enum {
            MAIN   = 0,
            NORMAL = 1,
            IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 1
        };

        static const bool timed = true;
        static const bool dynamic = false;
        static const bool preemptive = true;
        static const unsigned int HEADS = Traits<Machine>::CPUS;

    public:
        RR(int p = NORMAL): Priority(p) {}

        static unsigned int current_head() { return Machine::cpu_id(); }
    };

    // First-Come, First-Served (FIFO)
    class FCFS: public Priority
    {
    public:
        enum {
            MAIN   = 0,
            NORMAL = 1,
            IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 1
        };

        static const bool timed = false;
        static const bool dynamic = false;
        static const bool preemptive = false;

    public:
        FCFS(int p = NORMAL); // Defined at Alarm
    };

    template<typename T>
    class CpuAffinity: public Priority
	{
	public:
		enum {
			MAIN   = 0,
			NORMAL = 1,
			IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 1
		};

		static const bool timed = true;
		static const bool dynamic = false;
		static const bool preemptive = true;
		static const unsigned int QUEUES = Traits<Machine>::CPUS;
		unsigned int _queue;

	public:
		CpuAffinity(int p = NORMAL): Priority(p), _queue(T::schedule_queue(p)) {}

		static unsigned int current_queue() { return Machine::cpu_id(); }

		const unsigned int queue() const { return _queue; }
	};

    template<typename T>
    class CFSAffinity: public Priority
	{
	public:
		enum {
			MAIN   = 0,
			NORMAL = 1,
			IDLE   = (unsigned(1) << (sizeof(int) * 8 - 1)) - 1
		};

		static const bool timed = true;
		static const bool dynamic = false;
		static const bool preemptive = true;
		static const unsigned int QUEUES = Traits<Machine>::CPUS;
		unsigned int _queue;

	public:
		CFSAffinity(int p = NORMAL): Priority(p) {
			if(_priority == IDLE || _priority == MAIN)
				_queue = Machine::cpu_id();
			else
				_queue = T::schedule_queue();
		}

		CFSAffinity(int p, unsigned int queue): Priority(((1.0 / p) * (IDLE - 2)) + 1 ) {
			_queue = queue;
		}

		static unsigned int current_queue() { return Machine::cpu_id(); }

		const unsigned int queue() const { return _queue; }
	};
}


// Scheduling_Queue
template<typename T, typename R = typename T::Criterion>
class Scheduling_Queue: public Scheduling_Multilist<T> {};

// Scheduler
// Objects subject to scheduling by Scheduler must declare a type "Criterion"
// that will be used as the scheduling queue sorting criterion (viz, through
// operators <, >, and ==) and must also define a method "link" to export the
// list element pointing to the object being handled.
template<typename T>
class Scheduler: public Scheduling_Queue<T>
{
private:
    typedef Scheduling_Queue<T> Base;

public:
    typedef typename T::Criterion Criterion;
    typedef Scheduling_List<T, Criterion> Queue;
    typedef typename Queue::Element Element;

public:
    Scheduler() {}

    unsigned int schedulables() { return Base::size(); }

    T * volatile chosen() {
    	// If called before insert(), chosen will dereference a null pointer!
    	// For threads, we this won't happen (see Thread::init()).
    	// But if you are unsure about your new use of the scheduler,
    	// please, pay the price of the extra "if" bellow.
//    	return const_cast<T * volatile>((Base::chosen()) ? Base::chosen()->object() : 0);
    	return const_cast<T * volatile>(Base::chosen()->object());
    }

    void insert(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::insert(" << obj << ")" << endl;

        Base::insert(obj->link());
    }

    T * remove(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::remove(" << obj << ")" << endl;

        return Base::remove(obj->link()) ? obj : 0;
    }

    void suspend(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::suspend(" << obj << ")" << endl;

        Base::remove(obj->link());
    }

    void resume(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::resume(" << obj << ")" << endl;

        Base::insert(obj->link());
    }

    T * choose() {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::choose() => ";

        T * obj = Base::choose()->object();

        db<Scheduler>(TRC) << obj << endl;

        return obj;
    }

    T * choose_another() {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::choose_another() => ";

        T * obj = Base::choose_another()->object();

        db<Scheduler>(TRC) << obj << endl;

        return obj;
    }

    T * choose(T * obj) {
        db<Scheduler>(TRC) << "Scheduler[chosen=" << chosen() << "]::choose(" << obj;

        if(!Base::choose(obj->link()))
            obj = 0;

        db<Scheduler>(TRC) << obj << endl;

        return obj;
    }

    unsigned int queue_min_size() const {
		unsigned int min = -1;
		unsigned int queue = -1;

		for(unsigned int i = 0; i < Q; i++) {
			if(min > Base::_list[i].size()){
				min = Base::_list[i].size();
				queue = i;
			}
		}
		return queue;
	}

    T* get_idle(unsigned int queue = T::Criterion::current_queue()){
    	Element* e = Base::_list[queue].tail();
    	if(e && e->rank() == Criterion::IDLE){
    		return e->object();
    	} else {
    		return chosen_from_list(queue);
    	}
    }

    T* chosen_from_list(unsigned int list = T::Criterion::current_queue()){
    	return Base::_list[list].chosen()->object();
    }
};

__END_SYS

#endif

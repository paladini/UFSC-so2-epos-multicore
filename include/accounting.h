// Thread Accounting

#ifndef __accounting_h
#define __accounting_h

#include <tsc.h>
#include <cpu.h>
#include <machine.h>
#include <chronometer.h>
#include <utility/list.h>

__BEGIN_SYS
template<typename T> // T should be Time_Stamp, Tick or something like that.
class Accounting
{

protected:
	static const unsigned int MAX_HISTORY = Traits<Thread>::ACCOUNTING_MAX_HISTORY;

public:
	typedef Simple_List<T> List;
   	typedef typename List::Element Element;

	Accounting() {
		_last_runtime = 0;
		_created_at = Machine::cpu_id();
		_wait_cron_running = false;
		_runtime_cron_running = false;

		for(unsigned int i = 0; i < Traits<Build>::CPUS; i++) {
			_total_runtime[i] = 0;
		}
   	}

	int created_at() { 
		return _created_at; 
	}
   	
   	// Runtime related to the current CPU
	T last_runtime() { 
		return _last_runtime;
	}

	void last_runtime(T ts) {
		_last_runtime = ts;
		_total_runtime[Machine::cpu_id()] += ts;
	}

	// Runtime related to all CPUs
	T total_runtime_at(int cpu_id) {
		return _total_runtime[cpu_id];
	}

	// Wait-time related to the current CPU
	void wait_cron_start() {
		_wait_cron.reset(); 
		_wait_cron.start();
		_wait_cron_running = true;
	}
	
	void wait_cron_stop() { 
		_wait_cron.stop(); 
		_wait_cron_running = false;
		T my_value = _wait_cron.read_ticks();
		_wait_history.insert_head(new (SYSTEM) Element(&my_value));
		if(_wait_history.size() >= MAX_HISTORY){
			_wait_history.remove_tail();
		}
	}

	void runtime_cron_start() { 
		_runtime_cron.reset(); 
		_runtime_cron.start();
		_runtime_cron_running = true; 
	}
	
	void runtime_cron_stop() { 
		_runtime_cron.stop(); 
		_runtime_cron_running = false;
		T my_value = _runtime_cron.read_ticks();
		_runtime_history.insert_head(new (SYSTEM) Element(&my_value));
		if(_runtime_history.size() >= MAX_HISTORY){
			_runtime_history.remove_tail();
		}
	}
	
	bool wait_cron_running() { 
		return _wait_cron_running; 
	}

	bool runtime_cron_running() {
		return _runtime_cron_running;
	}
	
	T wait_cron_ticks() { 
		return _wait_cron.read_ticks(); 
	}

	T runtime_cron_ticks() {
		return _runtime_cron.read_ticks();
	}

	// Wait-time history
	List wait_history() { return _wait_history; }

	T wait_history_head() { return _wait_history.head()->object(); }

	T wait_history_media(){
		T media = 0;
		Element* e = _wait_history.head();
		while(e->next()) {
			media += *e->object();
			e = e->next();
		}
		return media / _wait_history.size();
	}

	// Runtime history
	List runtime_history() { 
		return _runtime_history; 
	}

	T runtime_history_head() { 
		return _runtime_history.head()->object(); 
	}

	T runtime_history_media() {
		T media = 0;
		Element* e = _runtime_history.head();
		while(e->next()) {
			media += *e->object();
			e = e->next();
		}
		return media / _runtime_history.size();
	}	

public: // MUDAR PARA PRIVATE
	T _last_runtime;
	T _total_runtime[Traits<Build>::CPUS];
	
	// Account the history of waits and runs (account only MAX_HISTORY data)
	List _wait_history;
	List _runtime_history;

	Chronometer _wait_cron;
	Chronometer _runtime_cron;
	bool _wait_cron_running;
	bool _runtime_cron_running;

	int _created_at; // At which CPU this resource was created

};

__END_SYS

#endif

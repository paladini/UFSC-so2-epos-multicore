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
		_wait_history_pointer = 0;

		_runtime_cron_running = false;
		_runtime_history_pointer = 0;

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
		unsigned int position = (_wait_history_pointer < MAX_HISTORY) ? _wait_history_pointer : _wait_history_pointer % MAX_HISTORY; 
		_wait_history[position] = wait_cron_ticks();
		_wait_history_pointer++;
	}

	void runtime_cron_start() { 
		_runtime_cron.reset(); 
		_runtime_cron.start();
		_runtime_cron_running = true; 
	}
	
	void runtime_cron_stop() { 
		_runtime_cron.stop(); 
		_runtime_cron_running = false;
		unsigned int position = (_runtime_history_pointer < MAX_HISTORY) ? _runtime_history_pointer : _runtime_history_pointer % MAX_HISTORY; 
		_runtime_history[position] = runtime_cron_ticks();
		_runtime_history_pointer++;
	}
	
	bool wait_cron_running() { 
		return _wait_cron_running; 
	}

	bool runtime_cron_running() {
		return _runtime_cron_running;
	}
	
	T wait_cron_ticks() { 
		return _wait_cron.read(); 
	}

	T runtime_cron_ticks() {
		return _runtime_cron.read();
	}

	// Wait-time history
	List wait_history() { return _wait_history; }

	T wait_history_head() { return _wait_history.head()->object(); }

	T wait_history_media(){
		T media = 0;
		unsigned int length = (_wait_history_pointer < MAX_HISTORY) ? _wait_history_pointer : MAX_HISTORY; 
		for (unsigned int i = 0; i < length; i++) {
			media += _wait_history[i];
		}
		return media / length;
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
		unsigned int length = (_runtime_history_pointer < MAX_HISTORY) ? _runtime_history_pointer : MAX_HISTORY; 
		for (unsigned int i = 0; i < length; i++) {
			media += _runtime_history[i];
		}
		return media / length;
	}	

public:
	T _last_runtime;
	T _total_runtime[Traits<Build>::CPUS];
	T _wait_history[MAX_HISTORY];
	T _runtime_history[MAX_HISTORY];
	
	unsigned int _wait_history_pointer;
	unsigned int _runtime_history_pointer;

	Chronometer _wait_cron;
	Chronometer _runtime_cron;
	bool _wait_cron_running;
	bool _runtime_cron_running;

	int _created_at; // At which CPU this resource was created

};

__END_SYS

#endif



#ifndef __thread_handler_h
#define __thread_handler_h

#include <utility/handler.h>
#include <thread.h>

__BEGIN_SYS

class Thread_Handler : public Handler{
public:
	Thread_Handler(Thread* thread){
		this->thread = thread;
	}

	~Thread_Handler(){}

	void handler(){
		thread->resume();
	}

private:
	Thread* thread;

};

__END_SYS

#endif

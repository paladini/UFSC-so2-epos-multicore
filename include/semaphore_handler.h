
#ifndef __semaphore_handler
#define __semephore_handler

#include <utility/handler.h>
#include <semaphore.h>


__BEGIN_SYS

class Semaphore_Handler : public Handler {
public:
	Semaphore_Handler(Semaphore* semaphore){
		this->semaphore = semaphore;
	}

	~Semaphore_Handler(){}

	void handler(){
		semaphore->v();
	}

private:
	Semaphore* semaphore;

};

__END_SYS


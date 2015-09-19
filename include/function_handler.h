

#ifndef __function_handler_h
#define __function_handler_h

#include <system/config.h>
#include <utility/handler.h>

__BEGIN_SYS

class Funciton_Handler : public Handler {
public:
	typedef void (Function)();

	Funciton_Handler(Function * function){
		this->function = function;
	}

	~Funciton_Handler() {}

	void handler(){
		function();
	}

private:
	Function * function;
};


__END_SYS

#endif

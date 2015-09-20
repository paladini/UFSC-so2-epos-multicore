

#ifndef __function_handler_h
#define __function_handler_h

#include <system/config.h>
#include <utility/handler.h>

__BEGIN_SYS

class Function_Handler : public Handler {
public:
	typedef void (Function)();

	Function_Handler(Function * function){
		this->function = function;
	}

	~Function_Handler() {}

	void handler(){
		function();
	}

private:
	Function * function;
};


__END_SYS

#endif

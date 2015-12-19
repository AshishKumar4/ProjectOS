
#include <sys/callback.h>
#include <sys/interrupt.h>

void_callback_arg_t interruptHandlers[256];

void interruptHandlerRegister(unsigned char _n, void_callback_arg_t _f)
{
	interruptHandlers[_n] = _f;		// Set function into the vector
}

void unhandledInterrupt(unsigned int _intNum, ...)
{
}

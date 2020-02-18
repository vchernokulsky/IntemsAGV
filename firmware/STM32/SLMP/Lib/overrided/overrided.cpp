#include "overrided.h"

std::string float_to_std_string(float number) {
	int ceil = int(number);
	float fraction_float = number - (float)ceil;
	float precision = 1e-2, err;

	unsigned int front_fraction_zero_counter = 0;

	float digit = 0;

	 do {
		 fraction_float *= 10;

		 digit = unsigned(fraction_float);

		 if (!digit) {front_fraction_zero_counter++;}

		 err = fraction_float - digit;

	 } while(err >= precision);

	 unsigned int fraction_int = unsigned(fraction_float);

	 return std::to_string(ceil) + "." + std::string(front_fraction_zero_counter, '0') +  std::to_string(fraction_int);
}

void * operator new( size_t size )
{
    return pvPortMalloc( size );
}

void * operator new[]( size_t size )
{
    return pvPortMalloc(size);
}

void operator delete( void * ptr )
{
    vPortFree ( ptr );
}

void operator delete[]( void * ptr )
{
    vPortFree ( ptr );
}

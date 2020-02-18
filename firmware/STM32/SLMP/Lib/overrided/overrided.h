#include <string>
#include "cmsis_os.h"

std::string float_to_std_string(float number);

void * operator new( size_t size );
void * operator new[]( size_t size );
void operator delete( void * ptr );
void operator delete[]( void * ptr );


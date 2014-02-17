#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint8_t	 DimmerField[5];	//array of DMX vals

void init_phase_cntrl();

#ifdef __cplusplus
}
#endif

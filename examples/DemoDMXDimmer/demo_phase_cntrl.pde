
#include "lib_phase_cntrl.h"

int main(void)
{
cli();

init_phase_cntrl();

DimmerField[1]= 10;
DimmerField[2]= 25;				//some fixed vals for output
DimmerField[3]= 50;				//ch0,1 are captured from DIPs
DimmerField[4]= 100;
DimmerField[5]= 150;
DimmerField[6]= 200;
DimmerField[7]= 255;

DDRC= 0;						//enable DIPs
PORTC= 0xFF;

sei();
for(;;)
	{
	DimmerField[0]= ~(PINC);
	};
}


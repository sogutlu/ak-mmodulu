/*
 *	Delay functions
 *	See delay.h for details
 *
 *	Make sure this code is compiled with full optimization!!!
 */

#include	"delay.h"

void
DelayMs(unsigned char cnt)
{
#if	XTAL_FREQ <= 2MHZ
	do {
		DelayUs(996);
	} while(--cnt);
#endif

#if    XTAL_FREQ > 2MHZ	
	unsigned char	i;
	do {
		i = 1;
		do {
			DelayUs(250);
		} while(--i);
	} while(--cnt);
#endif
}

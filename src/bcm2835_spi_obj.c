#include "bcm2835_spi_obj.h"
#include <bcm2835.h>



void bcm2835_spi_obj_init()
{
	
	// Inicializiruyem SPI                                                     	
	bcm2835_spi_begin();                                                       	
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);		// DEFAULT 	
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);						// DEFAULT	
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);		// KHz  	
	bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);					// DEFAULT 	

}

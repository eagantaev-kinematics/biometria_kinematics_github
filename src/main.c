#include <bcm2835.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "bcm2835_spi_obj.h"
#include "udp_send_thread.h"
#include "sensors_obj.h"


#define ADDR_A0 RPI_BPLUS_GPIO_J8_29
#define ADDR_A1 RPI_BPLUS_GPIO_J8_31
#define ADDR_A2 RPI_BPLUS_GPIO_J8_33
#define ADDR_A3 RPI_BPLUS_GPIO_J8_35
#define ADDR_A4 RPI_BPLUS_GPIO_J8_37
int addressA[] = {ADDR_A0, ADDR_A1, ADDR_A2, ADDR_A3, ADDR_A4};
uint8_t address_a = 0;


int16_t dataToTransmit[9* NUMBER_OF_SENSORS];
int16_t gyro_calibration[NUMBER_OF_SENSORS][3];
int16_t magnet_calibration[NUMBER_OF_SENSORS][3];

// vector zakreplyaet sootvetstvie chipselektov i datchikov:
// poziciya v massive - nomer datchika, chislo - nomer sootvetstvuyuschego chipselekta
// "-1" - ne naznachen 
int assignment[] = {0,1,2,   3,4,5,   6,7,8,   9,10,11,   12,13,14,   15,16,17,   18};  // po poryadku
//int assignment[] = {-1,-1,-1,   -1,-1,-1,   -1,-1,0,   0,0,-1,   -1,-1,-1,   -1,-1,-1};  // levaya ruka celikom
//int assignment[] = {13,14,15,   16,-1,-1,   -1,-1,-1,   -1,-1,-1,   -1,-1,-1,   -1,-1,-1};  // 13 - 16 (test)
//int assignment[] = {8,9,10,   11,12,13,   -1,-1,-1,   -1,-1,-1,   -1,-1,-1,   -1,-1,-1};  // 8 - 13 (test)

void *pointer;

int16_t sensorsData[NUMBER_OF_SENSORS][9];
double norma[NUMBER_OF_SENSORS];

int udp_send_thread_parameter = 0;

int16_t data_4x_board[9];


// functions ********************************************************

void pause(void)
{
	volatile int i;
	for(i=0; i<100; i++)
	{
		i++;
		i--;
	}
}

void one_usec_delay(void)
{
	volatile int i;
	for(i=0; i<33; i++)
	{
		i++;
		i--;
	}
}

void chipselhigh()
{
	long i;

	// test delay
	for(i=0; i<10; i++)
	{
		i++;
		i--;
	}


	bcm2835_gpio_write(ADDR_A4, HIGH); 	
	bcm2835_gpio_write(ADDR_A0, HIGH); 	
	bcm2835_gpio_write(ADDR_A1, HIGH); 	
	bcm2835_gpio_write(ADDR_A2, HIGH); 	
	bcm2835_gpio_write(ADDR_A3, HIGH); 	
}

void set_address(uint8_t address)
{
	// u nas dve serii adresov: 0 - 14 i 16 - ...

	if(address >= 15)
		address += 1;

	int i;
	uint8_t mask = 0x01;
	for(i=0; i<5; i++)
	{
		if(address & mask)
			bcm2835_gpio_write(addressA[i], HIGH); 
		else
			bcm2835_gpio_write(addressA[i], LOW); 
		mask = mask << 1;

	}

}


// end functions ********************************************************


//*****************************
//*****************************
//*****************************
//*****************************
//*****************************

int main(int argc, char **argv)
{

	//FILE *log_file;
	//char *application_log = "/root/app_logs/biometria_kinematics_log";

	struct timespec sleep_interval;

	sleep_interval.tv_sec = 0;
	sleep_interval.tv_nsec = 24000000;	// 25 mSec


	pthread_t udpSendThread;

	pointer = dataToTransmit;


// local variables
//****************************
	int i, j, k, l;

	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	
	int16_t hx, hy, hz;
	

// end of local variables block
//************************************



	// Inicializiruem periferiyu
	if(!bcm2835_init())
	{
		printf("\nbcm2835 initialization failed\n");
		return 1;
	}
	else // Everything OK with periphery
	{
		printf("\nTest of raspberry pi SPI interface with 4X board.\n");
		bcm2835_spi_obj_init();

		pthread_create(&udpSendThread, NULL, udp_send_thread, pointer);

		chipselhigh();
		int doJob = 1;
		while(doJob)
		{
			
			uint8_t data_msb;
			uint8_t data_lsb;
			uint8_t data_out;
			uint16_t data_aux;

			
			for(j=0; j<NUMBER_OF_SENSORS; j++)
			{
				//*
				address_a = j;                                                        	
				set_address(address_a);                                                   
                                                                                          
				data_out = 0x21; // komanda chteniya                                      
	            bcm2835_spi_transfer(data_out);// send data                               
	            one_usec_delay();                                                         
			    one_usec_delay();                                                         
			    chipselhigh();                                                            
	            one_usec_delay();                                                         
			    one_usec_delay();                                                         
	                                                                                                           
	                                                                                                           
	            //read_data_from_4x_board();                                              
			    for(i=0; i<9; i++)                                                                             
			    {                                                                         
	            	set_address(address_a);                                    	       
	                data_out = 0x55;                                                      
	                data_msb = bcm2835_spi_transfer(0x55);// read data                    
	                data_lsb = bcm2835_spi_transfer(0xaa);// read data                    
	            	one_usec_delay();                                                     
			    	one_usec_delay();                                                     
			        chipselhigh();                                                        
	            	one_usec_delay();                                                     
			    	one_usec_delay();                                                     
	                                                                                      
                    data_aux = (((uint16_t)data_msb)<<8) + (uint16_t)data_lsb;            
	                data_4x_board[i] = (int16_t)data_aux;                                 
	            }                                                                         
				//printf("\r\n");                                                     	
                                                                                            
				gyro_x  = data_4x_board[0];                                               
                gyro_y  = data_4x_board[1];                                               
                gyro_z = data_4x_board[2];                                                
                accel_x = data_4x_board[3];                                               
                accel_y = data_4x_board[4];                                               
                accel_z = data_4x_board[5];                                               
                hx = data_4x_board[6];                                                    
                hy = data_4x_board[7];                                                    
                hz = data_4x_board[8];                                                      


				// kalibrovka ****************

				{
					//*/
					// without rotation          
				    sensorsData[j][0] = gyro_x;
				    sensorsData[j][1] = gyro_y;
				    sensorsData[j][2] = gyro_z;
				    sensorsData[j][3] = accel_x;
				    sensorsData[j][4] = accel_y;
				    sensorsData[j][5] = accel_z;
				    sensorsData[j][6] = hx;
				    sensorsData[j][7] = hy;
				    sensorsData[j][8] = hz;
					//*
				}

			
			}// end for(j=0; ...


			for(k=0; k<NUMBER_OF_SENSORS; k++)
			{
				if(assignment[k] >= 0)
				{
					for(l=0; l<9; l++)
			    	{
			    		dataToTransmit[k*9 + l] = sensorsData[assignment[k]][l];
			    	}	 
				}
			}
			//*/


			nanosleep(&sleep_interval, NULL);


		}// end while (main loop)

		
		// Vyklyuchaem SPI
		bcm2835_spi_end();
		// Deinicializiruem periferiyu
		bcm2835_close();

	
		return 0;
	}//end if(!bcm2835_init())
}// end main

//***************************************************
//***************************************************
//***************************************************
//***************************************************
//***************************************************


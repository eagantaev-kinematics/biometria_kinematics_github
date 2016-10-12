#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "udp_send_thread.h"
#include "sensors_obj.h"


void *udp_send_thread(void *pointer)
{
	FILE *log_file;
	char *application_log = "/root/app_logs/biometria_kinematics_log";
	
	int doJob = 1;
	int i;
	long counter_long;

	struct timespec sleep_interval;

	sleep_interval.tv_sec = 0;
	//sleep_interval.tv_nsec = 15000000;	// 15 miliseconds
	sleep_interval.tv_nsec = 25000000;	// 


	int sock;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		log_file = fopen(application_log, "a");
		time_t current_time;
		time(&current_time);
		fprintf(log_file, "\r\n[%ld]ERROR: socket failure ******************\r\n", current_time);
		fclose(log_file);
		return_code = 1;
		return (void *)(&return_code);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(112);
	//addr.sin_addr.s_addr = inet_addr("192.168.173.2");
	addr.sin_addr.s_addr = inet_addr("192.168.2.1");
	//addr.sin_addr.s_addr = inet_addr("192.168.1.3");
	//addr.sin_addr.s_addr = inet_addr("192.168.0.177");
	//addr.sin_addr.s_addr = inet_addr("192.168.0.191");
	//addr.sin_addr.s_addr = inet_addr("192.168.43.139");

	printf("\r\n%s\r\n", inet_ntoa(addr.sin_addr));


	i = 0;
	counter_long = 0;
	while(doJob)
	{
		//printf("%d\r\n", i);
		i++;
		if(i >= 40)
		{
			i = 0;
			counter_long++;
			printf("sent %ld series --------------->\r\n", counter_long);
		}

		// send data via udp socket ***************************************************
		sendto(sock, (char *)pointer, 9* NUMBER_OF_SENSORS*2, 0, (struct sockaddr *)&addr, sizeof(addr));
		//*****************************************************************************

		nanosleep(&sleep_interval, NULL);
	}

	return_code = 0;
	return (void *)(&return_code);
}

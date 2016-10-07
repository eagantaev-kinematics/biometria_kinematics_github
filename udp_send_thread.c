void *udp_send_thread(void *param)
{
	int doJob = 1;
	int i,j,k;
	long counter_long;

	struct timespec sleep_interval;

	sleep_interval.tv_sec = 0;
	//sleep_interval.tv_nsec = 15000000;	// 15 miliseconds
	sleep_interval.tv_nsec = 25000000;	// 


	int sock;
	struct sockaddr_in addr;

	i = 1;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		printf("\r\nsocket failure ******************\r\n");
		*((int *)param) = 1; 
		return (void *)param;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(112);
	//addr.sin_addr.s_addr = inet_addr("192.168.173.2");
	addr.sin_addr.s_addr = inet_addr("192.168.1.1");
	//addr.sin_addr.s_addr = inet_addr("192.168.1.3");
	//addr.sin_addr.s_addr = inet_addr("192.168.0.177");
	//addr.sin_addr.s_addr = inet_addr("192.168.0.191");
	//addr.sin_addr.s_addr = inet_addr("192.168.43.139");

	printf("\r\%s\r\n", inet_ntoa(addr.sin_addr.s_addr));


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
		sendto(sock, (char *)pointer, 9* number_of_sensors*2, 0, (struct sockaddr *)&addr, sizeof(addr));
		//*****************************************************************************

		nanosleep(&sleep_interval, NULL);
	}
}

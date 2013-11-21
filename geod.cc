#include "geod.h"
#include "ioid.h"
#include "start.h"

#if LINUX
#include <pthread.h>
#endif


#include "client.h"
#include "server.h"
#include <iostream>
#include <string.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;



int main(int argc, char **argv)
{
  
  int i;
 
  
#if WINDOWS
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != 0) 
  {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  }
#endif
  
    start_data sd;
    int numCPU = 2;
    
    
#if LINUX
    numCPU = sysconf(_SC_NPROCESSORS_ONLN);;
#elif WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	numCPU = sysinfo.dwNumberOfProcessors;
#elif MINIX
	numCPU = 2;
#endif
  
    int nthr = numCPU;
#if LINUX
    pthread_t* pth = new pthread_t[nthr];
#elif WINDOWS
	HANDLE *pth = new HANDLE[nthr];
	DWORD *tid = new DWORD[nthr];
#elif MINIX
    pid_t *pth = new pid_t[nthr];
#endif


    std::cout<<"Machine with "<<numCPU<<" CPU\n";
    std::cout<<"\n";
    
    if (argc > 1)
    {
#if WINDOWS
      strcpy_s(server_ip, argv[1]);
#elif LINUX || MINIX
      strcpy(server_ip, argv[1]);
#endif
    }
    
    for (i = 0; i < nthr; i++)
    {
      
#if WINDOWS
	  pth[i] = CreateThread(NULL, 0, geodesic_winthreads, NULL, 0, &(tid[i]));
	  // ждем, чтобы поток успел скопировать данные
	  Sleep(100);
#elif LINUX
	  
	  int res = pthread_create(&(pth[i]), NULL, geodesic_pthread, NULL);
	  // ждем, чтобы поток успел скопировать данные
	  usleep(100000);
#elif MINIX
	  pth[i] = fork();
	  if (pth[i] == 0)
	    geodesic_fork(NULL);
#endif
	}
  
	   // ждем окончания всех потоков
 
#if WINDOWS
    WaitForMultipleObjects(nthr, pth, TRUE, INFINITE);
	for(int i=0; i < nthr; i++)
        CloseHandle(pth[i]);
#elif LINUX
    for (i = 0; i < nthr; i++)
      pthread_join(pth[i], NULL);
#elif MINIX
    ;
#endif  
  
    
    delete[] pth;
  return 0;
}

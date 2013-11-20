#include "geod.h"
#include "ioid.h"
#include "start.h"

#ifdef LINUX
#include <pthread.h>
#endif

#include <iostream>
#include <string.h>

const int deflen = 4;
const real eps=1e-7;
const real H=1e-5;



int main(int argc, char **argv)
{
  
  int i;
  int server = 0;
  
#ifdef WINDOWS
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != 0) 
  {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  }
#endif
  
  if (argc > 1)
  {
    if (strcmp(argv[1],"server")==0)
      server = 1;
  } 
  if (server)
  {
    int L = 4;
    recv_server(&L);
  }
  else
  {
    start_data sd;
    int numCPU = 2;
    
    
#ifdef LINUX
    numCPU = sysconf(_SC_NPROCESSORS_ONLN);;
#elif WINDOWS
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	numCPU = sysinfo.dwNumberOfProcessors;
#endif
  
    int nthr = numCPU;
#ifdef LINUX
    pthread_t* pth = new pthread_t[nthr];
#elif WINDOWS
	HANDLE *pth = new HANDLE[nthr];
	DWORD *tid = new DWORD[nthr];
#endif


    std::cout<<"Machine with "<<numCPU<<" CPU\n";
    std::cout<<"\n";
    
    if (argc > 1)
    {
#ifdef WINDOWS
      strcpy_s(server_ip, argv[1]);
#elif LINUX
      strcpy(server_ip, argv[1]);
#endif
    }
    io_init(argc, argv);
 
    for (i = 0; i < nthr; i++)
    {
      
#ifdef WINDOWS
	  pth[i] = CreateThread(NULL, 0, geodesic_winthreads, NULL, 0, &(tid[i]));
	  // ждем, чтобы поток успел скопировать данные
	  Sleep(100);
#elif LINUX
	  
	  int res = pthread_create(&(pth[i]), NULL, geodesic_pthread, NULL);
	  // ждем, чтобы поток успел скопировать данные
	  usleep(100000);
#endif
	}
  
	   // ждем окончания всех потоков
 
#ifdef WINDOWS
    WaitForMultipleObjects(nthr, pth, TRUE, INFINITE);
	for(int i=0; i < nthr; i++)
        CloseHandle(pth[i]);
#elif LINUX
    for (i = 0; i < nthr; i++)
      pthread_join(pth[i], NULL);
#endif  
  
    io_close();
  
    delete[] pth;
  }
  return 0;
}

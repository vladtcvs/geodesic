#include <stdio.h>
#include "geod.h"
#include "ioid.h"
#include "start.h"
#ifdef LINUX
#include <glib.h>
#endif
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#include "message.h"

char server_ip[20]="192.168.1.238";

char my_ip[20]="0.0.0.0";




void ioid::io_open()
{
  struct sockaddr_in addr;
  udpSocket = socket(AF_INET, SOCK_DGRAM, 0);


  memset(&addr, 0, sizeof(addr));
  addr.sin_family   = AF_INET;
  addr.sin_port     = htons(0);
  
  
  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family   = AF_INET;
  srv_addr.sin_port     = htons(2345); 

#ifdef LINUX
  inet_aton(my_ip, &addr.sin_addr);
  inet_aton(server_ip, &srv_addr.sin_addr);
  bind(udpSocket, (struct sockaddr*)&addr, sizeof(addr));
#elif WINDOWS
  addr.sin_addr.s_addr = inet_addr(my_ip);
  srv_addr.sin_addr.s_addr = inet_addr(server_ip);
  bind(udpSocket, (LPSOCKADDR)&addr, sizeof(addr));
#endif

 
  struct timeval timeout;      
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;
  setsockopt (udpSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
}


void ioid::io_close()
{
  msgbuf.clear();
#ifdef WINDOWS
  closesocket(udpSocket);
#elif LINUX
  close(udpSocket);
#endif
}

ioid::ioid()
{
  msgbuf.clear();
}

ioid::~ioid()
{
  msgbuf.clear();
}

void ioid::setlen(int l)
{
  int L  = 2*l+2;
  msgbuf.resize(L);
}
  
/**
 * Эта функция запускается на клиентах. 
 * С ее помощью они посылают результаты расчетов на сервер в 
 * формате double 
 * первое число - тип сообщения (msg_type), потом номер расчета (calc_id), 
 * дальше идут векторы позиции и касательной
 */
int ioid::write(char * buf, int len)
{
  sendto(udpSocket, (const char*)buf, len, 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr)); 
  return 0;
}



int ioid::read(char *buf, int len)
{
  int ans;
  
  ans = recv(udpSocket, buf, len, 0);
  PRINT_LOG
  return ans;
}


srv_ioid::srv_ioid()
{
  
}

srv_ioid::~srv_ioid()
{
  
}

int srv_ioid::srv_open()
{
#ifdef LINUX
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
#elif WINDOWS
  udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#endif
  sockaddr_in m_addr;
  
  if (udpSocket < 0)
  {
	printf("Failed to open socket\n");
	return -1;
  }


  memset(&m_addr, 0, sizeof(m_addr));
  
	  
  m_addr.sin_family   = AF_INET;  // обязательно AF_INET!
  m_addr.sin_port     = htons(2345);
  
  
  /* Переводим адрес в нужный нам формат */
  m_addr.sin_addr.s_addr = inet_addr(my_ip);
#ifdef LINUX
  int br = bind(udpSocket, (struct sockaddr*)&m_addr, sizeof(m_addr));
#elif WINDOWS
  int br = bind(udpSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));
#endif
  if (br < 0)
  {
	printf("Unable to bind socket\n");
	return -1;
  }
  return 0;
}


void srv_ioid::srv_close()
{
#ifdef WINDOWS
    closesocket(udpSocket);
#elif LINUX
    close(udpSocket);
#endif
}

int srv_ioid::read(char *buf, int maxlen, client_id *client)
{
  
  int bytesrecv = -1;
 
  
  
  client->c_addr_l = sizeof(client->c_addr);
  bytesrecv = recvfrom(udpSocket, buf, maxlen, 0, (sockaddr*)&(client->c_addr), &(client->c_addr_l));
 
  
  return bytesrecv;
}

int srv_ioid::write(char *buf, int len, client_id *client)
{
  return sendto(udpSocket, (const char*)buf, len, 0, (sockaddr*)&(client->c_addr), client->c_addr_l);
}


client_id::client_id()
{
  c_addr_l = sizeof(c_addr);
  memset(&c_addr, 0, sizeof(c_addr));
}


char *client_id::client_name()
{
  return inet_ntoa(c_addr.sin_addr);
}

int io_init(int argc, char **argv)
{
 
  return 0;
}


int io_close()
{ 
  
  return 0;
}






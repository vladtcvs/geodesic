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

char server_ip[20]="192.168.1.238";

char my_ip[]="0.0.0.0";

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

void ioid::getnew()
{
  double msg[1] = {GD_GETNEW};
#ifdef WINDOWS
  sendto(udpSocket, (const char*)msg, sizeof(double), 0, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));
#elif LINUX
  sendto(udpSocket, (const char*)msg, sizeof(double), 0, (sockaddr*)&srv_addr, sizeof(srv_addr));
#endif
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
int ioid::write_poskas(poskas pk, int calc_id)
{
  int L = pk.p.dim();
  int i;
  int f = 1;
  for (i = 0; f==1 && i < L; i++)
  {
    f *= (fabs(pk.p[i]) < 1e20);
    f *= (fabs(pk.v[i]) < 1e20);
  }
  
  if (f == 0)
    return -1;
  msgbuf[0] = GD_POSKAS;  
  msgbuf[1] = calc_id;
  for (i = 0; i < L; i++)
  {
    msgbuf[i+2] = pk.p[i];
  }
  for (i = 0; i < L; i++)
  {
    msgbuf[L+i+2] = pk.v[i];
  }
  // здесь мы посылаем сообщение 
  double *data = msgbuf.data();
  sendto(udpSocket, (const char*)data, msgbuf.size()*sizeof(double), 0, 
	  (struct sockaddr*)&srv_addr, sizeof(srv_addr)); 
  return 0;
}

void ioid::fin()
{
  double msg = GD_FIN;
  sendto(udpSocket, (const char*)&msg, sizeof(double), 0, 
	  (struct sockaddr*)&srv_addr, sizeof(srv_addr));  
}

int ioid::read_start(double *buf, int len)
{
  int ans;
  
  ans = recv(udpSocket, (char*)buf, len*sizeof(double), 0);
  PRINT_LOG
  return ans;
}


int io_init(int argc, char **argv)
{
 
  return 0;
}


int io_close()
{ 
  return 0;
}




start_data *get_start(void)
{
  start_data* sd;
  
  ioid id;
  
  double buf[1000];
  
  id.io_open();
  id.getnew();
  if (id.read_start(buf, 1000) < 0)
    return NULL;
  
  int msglen = (int)buf[0];
  if (msglen == 0)
    return NULL;
  
  if (msglen < 13)
    return NULL;
  
  int dim = (int)buf[1];
  
  poskas pk(dim);
  id.setlen(dim);
  
  pk.p[0] = buf[2];
  pk.p[1] = buf[3];
  pk.p[2] = buf[4];
  pk.p[3] = buf[5];
  pk.v[0] = buf[6];
  pk.v[1] = buf[7];
  pk.v[2] = buf[8];
  pk.v[3] = buf[9];
  
  sd = new start_data;
  
  sd->pk = pk;
  sd->N = (int)buf[10];
  sd->h = buf[11];
  sd->dh = buf[12];
  sd->id = id;
  
  sd->calc_id = (int)buf[13];
  
  
  return sd;
}


/**
 * Эта функция запускается только на сервере, она сохраняет принятые от клиентов 
 * данные 
 */
void save_pos(poskas pk, int calc_id)
{
  if (calc_id == -1)
  {
    int i, L = pk.p.dim();
    printf("%i ", calc_id);
    for (i = 0; i < L; i++)
      printf("%lf ", (double)(pk.p[i]));
    printf("\n");
  }
  
}



/** 
 * Эта функция запускается на сервере. 
 * Она ожидает поступающих от клиентов результатов расчетов.
 */
void* recv_server(void* data)
{
  int L = *((int*)data);
  int len = L*2+2;
  std::vector<double> buf;
  
  
  
  poskas pk;
  int calc_id;
  
  buf.resize(len);
  PRINT_LOG
#ifdef LINUX
  int udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
#elif WINDOWS
  SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
#endif
  sockaddr_in m_addr, c_addr;
  
  if (udpSocket == -1)
  {
	printf("Failed to open socket\n");
    return NULL;
  }


  memset(&m_addr, 0, sizeof(m_addr));
  memset(&c_addr, 0, sizeof(c_addr));
	  
	  
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
  }
  int go = 1;
  
  int dlen = -1;
  while (go)
  {
	  double *bdata = buf.data();
	  int i;
	  int msglen = buf.size()*sizeof(double);
#ifdef LINUX
	  socklen_t frln;
#elif WINDOWS
	  int frln;
#endif
	  frln = sizeof(c_addr);
	  int bytesrecv = recvfrom(udpSocket, (char*)bdata, msglen, 0, (sockaddr*)&c_addr, &frln);
	  
	  if (bytesrecv == -1)
	    break;
	  Msgtype msg_type = (Msgtype)((int)buf[0]);
	    
	  switch(msg_type)
	  {
	    case GD_POSKAS:
	      {
			calc_id = (int)buf[1];
			for (i = 0; i < L; i++)
			{
			  pk.p[i] = buf[i+2];
			  pk.v[i] = buf[i+L+2];
			}
			save_pos(pk, calc_id);
	      }
	      break;
	    case GD_GETNEW:
	      {
		int res;
		
		if (dlen == 0)
		{
		  double msg[2] = {0};
		  sendto(udpSocket, (const char*)&msg, sizeof(double), 0, (sockaddr*)&c_addr, frln);	
		  go = 0;
		}  
		else
		{
		  double *start = srv_get_start();
		  dlen = (int)start[0];
		  PRINT_LOG
		  
		  
		  res = sendto(udpSocket, (const char*)start, 
				(dlen+1)*sizeof(double), 0, (sockaddr*)&c_addr, frln);
		
		  delete[] start;
		}
	      }
	      break;
	    case GD_FIN:
	      {
				PRINT_LOG
				printf("recv FIN from %s\n", inet_ntoa(c_addr.sin_addr));
	      }
	      break;
	    default:
	      break;
	  }
  }
#ifdef WINDOWS
  closesocket(udpSocket);
#elif LINUX
  close(udpSocket);
#endif
  buf.clear();
  return NULL;
}



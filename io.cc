#include <stdio.h>
#include "geod.h"
#include <glib.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

char server_ip[20]="127.0.0.1";

char my_ip[]="0.0.0.0";

void ioid::io_open()
{
  struct sockaddr_in addr;
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family   = AF_INET;
  addr.sin_port     = htons(0);
  inet_aton(my_ip, &addr.sin_addr);
  bind(udpSocket, (struct sockaddr*)&addr, sizeof(addr));
  
  bzero(&s_addr, sizeof(s_addr));
  s_addr.sin_family   = AF_INET;
  s_addr.sin_port     = htons(2345); 
  inet_aton(server_ip, &s_addr.sin_addr);
  
  struct timeval timeout;      
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;
  setsockopt (udpSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
}

void ioid::getnew()
{
  double msg[1] = {GD_GETNEW};
  sendto(udpSocket, msg, sizeof(double), MSG_NOSIGNAL, (struct sockaddr*)&s_addr, sizeof(s_addr));
}

void ioid::io_close()
{
  msgbuf.clear();
  close(udpSocket);
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
  sendto(udpSocket, data, msgbuf.size()*sizeof(double), MSG_NOSIGNAL, (struct sockaddr*)&s_addr, sizeof(s_addr)); 
  return 0;
}

void ioid::fin()
{
  double msg = GD_FIN;
  sendto(udpSocket, &msg, sizeof(double), MSG_NOSIGNAL, (struct sockaddr*)&s_addr, sizeof(s_addr));
  
}

int ioid::read_start(double *buf, int len)
{
  int ans;
  
  ans = recv(udpSocket, buf, len*sizeof(double), MSG_NOSIGNAL);
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




start_data *get_start()
{
  start_data* sd;
  
  ioid id;
  char fname[10];
  double buf[1000];
  
  id.io_open();
  id.getnew();
  if (id.read_start(buf, 1000) < 0)
    return NULL;
  
  int msglen = buf[0];
  if (msglen == 0)
    return NULL;
  
  if (msglen < 13)
    return NULL;
  
  int dim = buf[1];
  
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
  sd->N = buf[10];
  sd->h = buf[11];
  sd->dh = buf[12];
  sd->id = id;
  
  sd->calc_id = buf[13];
  
  
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
    for (i = 0; i < L; i++)
      printf("%lf ", (double)(pk.p[i]));
    printf("\n");
  }
  
}



/** 
 * Эта функция запускается на сервере. 
 * Она ожидает поступающих от клиентов результатов расчетов.
 */
gpointer recv_server(gpointer data)
{
  int L = *((int*)data);
  int len = L*2+2;
  std::vector<double> buf;
  
  
  
  poskas pk;
  int calc_id;
  
  buf.resize(len);
  
  int udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
  sockaddr_in m_addr, c_addr;
  
  bzero(&m_addr, sizeof(m_addr));
  bzero(&c_addr, sizeof(c_addr));
	  
	  
  m_addr.sin_family   = AF_INET;  // обязательно AF_INET!
  m_addr.sin_port     = htons(2345);
  
  
  /* Переводим адрес в нужный нам формат */
  inet_aton(my_ip, &m_addr.sin_addr);
  bind(udpSocket, (struct sockaddr*)&m_addr, sizeof(m_addr));
  int go = 1;
  
  int dlen = -1;
  while (go)
  {
	  double *bdata = buf.data();
	  int i;
	  int msglen = buf.size()*sizeof(double);
	
	  socklen_t frln;
	  
	  int bytesrecv = recvfrom(udpSocket, bdata, msglen, MSG_NOSIGNAL, (sockaddr*)&c_addr, &frln);
	  
	  if (bytesrecv == -1)
	    break;
	  Msgtype msg_type = (Msgtype)buf[0];
	    
	  switch(msg_type)
	  {
	    case GD_POSKAS:
	      {
		calc_id = buf[1];
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
		  sendto(udpSocket, &msg, sizeof(double), MSG_NOSIGNAL, (sockaddr*)&c_addr, frln);	
		  go = 0;
		}  
		else
		{
		  double *start = srv_get_start();
		  dlen = start[0];
		  PRINT_LOG
		  
		  
		  res = sendto(udpSocket, start, (dlen+1)*sizeof(double), MSG_NOSIGNAL, (sockaddr*)&c_addr, frln);
		
		  delete start;
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
  
  close(udpSocket);
  buf.clear();
}



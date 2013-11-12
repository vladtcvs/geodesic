#include <stdio.h>
#include "geod.h"
#include <glib.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

char server_ip[]="127.0.0.1";

char my_ip[]="0.0.0.0";

void ioid::io_open(int ind, int len)
{
  calc_id = ind;
  setlen(len);
  
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
int ioid::write_poskas(poskas pk)
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

int io_init(int argc, char **argv)
{
 
  return 0;
}


int io_close()
{ 
  return 0;
}

/**
 * Эта функция запускается только на сервере, она сохраняет принятые от клиентов 
 * данные 
 */
void save_pos(poskas pk, int calc_id)
{
  if (calc_id == 1)
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
  sockaddr_in m_addr;
  
  bzero(&m_addr, sizeof(m_addr));
  m_addr.sin_family   = AF_INET;  // обязательно AF_INET!
  m_addr.sin_port     = htons(2345);
  /* Переводим адрес в нужный нам формат */
  inet_aton(my_ip, &m_addr.sin_addr);
  bind(udpSocket, (struct sockaddr*)&m_addr, sizeof(m_addr));
  
  while (1)
  {
	  double *bdata = buf.data();
	  int i;
	  int msglen = buf.size()*sizeof(double);
	  
	  int bytesrecv = recvfrom(udpSocket, bdata, msglen, MSG_NOSIGNAL, (sockaddr*)NULL, NULL);
	  
	  if (bytesrecv != msglen)
	    break;
	  int msg_type = buf[0];
	    
	  if (msg_type == GD_POSKAS)
	  {
	    calc_id = buf[1];
	    for (i = 0; i < L; i++)
	    {
	      pk.p[i] = buf[i+2];
	      pk.v[i] = buf[i+L+2];
	    }
	    save_pos(pk, calc_id);
	  }
  }
  
  close(udpSocket);
  buf.clear();
}



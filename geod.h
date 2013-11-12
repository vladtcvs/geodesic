#ifndef GEOD_H
#define GEOD_H
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <glib.h>


#include <Qt/qobject.h>
#include <Qt/QtNetwork>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef long double real;

extern const int deflen;
extern const real eps;
extern const real H;




using namespace std;

class Lvector
{
	private:
		int L;
		vector<real> V;
	public:
		Lvector();
		Lvector(int l);
		~Lvector();
		void alloc(int n);
		int dim() {return L;}
		real& operator [] (int i);
		Lvector operator + (Lvector &v);
		Lvector operator - (Lvector &v);
		Lvector operator += (Lvector &v);
		Lvector operator -= (Lvector &v);
		Lvector operator * (real n);
		Lvector operator *= (real n);

		Lvector operator / (real n);
		Lvector operator /= (real n);
};

class tensor2
{
	private:
	int L;
	vector<Lvector> m;
	public:
	tensor2();
	int dim() {return L;}
	tensor2 adj(int i, int j);
	tensor2 inv();
	real det();
	void alloc(int n);
	tensor2(int i);
	~tensor2();
	Lvector& operator [] (int i);
	tensor2 operator / (real n);
	tensor2 operator * (real n);
	tensor2 operator += (tensor2 t);

};

class tensor3
{
	private:
	int L;
	vector<tensor2> m;
	public:
	int dim() {return L;}
	tensor3();
	void alloc(int n);
	tensor3(int i);
	~tensor3();
	tensor2& operator [] (int i);

};

class poskas
{
	public:
	Lvector p, v;
	poskas()
	{
		p.alloc(deflen);
		v.alloc(deflen);
	}
	poskas(int l)
	{
		p.alloc(l);
		v.alloc(l);
	}
	poskas operator + (poskas pos);
	poskas operator - (poskas pos);
	poskas operator += (poskas pos);
	poskas operator -= (poskas pos);
	poskas operator * (real n);
	poskas operator / (real n);
	poskas operator *= (real n);
	poskas operator /= (real n);

};


class ioid
{
   std::vector<double> msgbuf;
   int udpSocket;
   sockaddr_in s_addr;
   
   int calc_id;
   public:
      ioid();
      ~ioid();
      void setlen(int l);
      void io_open(int ind, int len);
      void io_close();
      int write_poskas(poskas pk);
      
};



struct start_data
{
  poskas pk;
  real h;
  real dh;
  int N;
  ioid id;
  int p_id;
  void close();
};


tensor2 dGi(Lvector p, int n);
tensor3 Kristofel(Lvector p);
tensor2 Metric(Lvector p);
poskas runge_kutta4(poskas &pk, real h);
start_data* get_start();
int io_init(int argc, char **argv);
int io_close();
void save_pos(poskas pk, int io_id);
gpointer recv_server(gpointer data);

#define PRINT_LOG printf("%s (%s:%i)\n", __FUNCTION__, __FILE__, __LINE__);



#define GD_POSKAS 1


#endif

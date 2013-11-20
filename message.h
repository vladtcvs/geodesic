#ifndef MESSAGE_H
#define MESSAGE_H

#include "tensor.h"
#include "start.h"

enum Msgtype {GD_NONE, GD_POSKAS, GD_FIN, GD_GETNEW, GD_START, GD_SIGNAL};
enum Sigtype {GD_S_NONE, GD_S_WAIT, GD_S_END};
class msg
{
protected:
  Msgtype type;
  
public:
#ifdef LINUX
  int64_t thread;
#elif WINDOWS
  int64_t thread;
#endif
  Msgtype mtype();
  msg();
};

class msg_poskas : public msg
{
public:
  msg_poskas();
  poskas pk;
  int calc_id;
  int dim;
};

class msg_start : public msg
{
public:
  msg_start();
  poskas pk;
  int calc_id;
  int dim;
  int N;
  double h;
  double dh;
};

class msg_fin : public msg
{
public:
  msg_fin();
  int calc_id;
};

class msg_getnew : public msg
{
public:
  msg_getnew();
};

class msg_signal : public msg
{
  
public:
  Sigtype sig;
  msg_signal();
};

msg* decode(char *buf, int blen);
int encode(char *buf, int blen, msg *message);
start_data *msg2start(msg_start *ms, ioid& id);
msg_start *start2msg(start_data *ms);

#endif
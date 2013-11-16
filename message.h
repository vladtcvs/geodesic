#ifndef MESSAGE_H
#define MESSAGE_H

#include "tensor.h"

enum Msgtype {GD_NONE, GD_POSKAS, GD_FIN, GD_GETNEW, GD_START};
class msg
{
protected:
  Msgtype type;
  
public:
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

msg* decode(char *buf, int blen);
int encode(char *buf, int blen, msg *message);


#endif
#include "Aodv_message.hpp"

Aodv_message(std::string dst_ip, int dst_seq){
    dest_ip = dst_ip;
    dest_seq = dst_seq;
    time_stamp = //the environment time
}

std::string get_dest_ip(){
  return dest_ip;
}

int get_dest_seq(){
  return dest_seq;
}

int get_time_stamp(){
  return time_stamp;
}

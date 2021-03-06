/*
This file is part of octoDrone.

octoDrone is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

octoDrone is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with octoDrone.  If not, see <http://www.gnu.org/licenses/>.
*/

///Base class for all AODV message types

#include "Aodv_message.hpp"

Aodv_message::Aodv_message(std::string dst_ip, int dst_seq, int ttl_time): Message("AODV"){
    dest_ip = dst_ip;
    dest_seq = dst_seq;
    ttl = ttl_time;
}

///Getter method for the IP address of the destination node
std::string Aodv_message::get_dest_ip(){
  return dest_ip;
}

///Getter method for the time to live
int Aodv_message::get_ttl(){
    return ttl;
}

///Getter method for the sequence number of the destination node
int Aodv_message::get_dest_seq(){
  return dest_seq;
}

///Returns a string representation of the message
std::string Aodv_message::serialize(){
	std::string result;
	result = dest_ip + ";" + std::to_string(dest_seq) + ";" + std::to_string(ttl);
	return result;
}

#include "Aodv_rreq.hpp"

Aodv_rreq(int hop, int id, std::string src_ip, std::string dst_ip, int src_seq, int dst_seq, int ttl){
	Aodv_message::Aodv_message(dest_ip, dest_seq, ttl);
	hop_count = hop;
	rreq_id = id;
	source_ip = src_ip;
	source_seq = src_seq;
}

int get_hop_count(){
	return hop_count;
}

int get_rreq_id(){
	return rreq_id;
}
    
std::string get_source_ip(){
	return source_ip;
}

int get_source_seq(){
	return source_seq;
}

std::string serialize(){
	std::string result = Aodv_message::serialze();
	result = "RREQ;" + result;
	result += ";" + std::to_string(hop_count) + ";" + std::to_string(rreq_id) + ";"
		+ source_ip + ";" + std::to_string(source_seq);
	return result;
}
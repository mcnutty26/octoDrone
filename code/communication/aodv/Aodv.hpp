#ifndef EOW_AODV_MAIN
	#define EOW_AODV_MAIN

	#include <string>
	#include <map>
	#include <Environment.hpp>
	#include <CommMod.hpp>
	#include "Aodv_message.hpp"
	#include "Aodv_rreq.hpp"
	#include "Aodv_rrep.hpp"
	#include "Aodv_rerr.hpp"
	#include "Aodv_route.hpp"
	#include <atomic>

	class Aodv: public CommMod {
		public:
			Aodv(Environment*, std::string, std::atomic_flag*, bool);
		protected:
			void comm_function();
		private:
			std::map<std::string, Aodv_route*> route_table;
			std::string ip_address;
			int HELLO_INTERVAL;
			int SEQUENCE_NUMBER;
			int ACTIVE_ROUTE_TIMEOUT;
			int PATH_DISCOVERY_TIME;
			int BROADCAST_ID;
			int RANGE;
			int TTL;

			double xpos;
			double ypos;
			double zpos;

			Aodv_rreq* create_hello();
			Aodv_rreq* create_rreq(std::string, int);
			Aodv_rrep* create_rrep(std::string, int);
			Aodv_rerr* create_rerr(std::string, int);

			void process_rreq(Aodv_rreq*);
			void process_rrep(Aodv_rrep*);
			void process_rerr(Aodv_rerr*);
			void process_data(std::string);

			std::string get_attribute(std::string);
			bool have_route(std::string);
			void add_route(std::string, int, int, std::string, int);

			Aodv_rreq* deserialize_rreq(std::string);
			Aodv_rrep* deserialize_rrep(std::string);
			Aodv_rerr* deserialize_rerr(std::string);

			std::pair<std::string, std::string> current_message;
			int state;
			std::atomic_flag* lock;
			void log(std::string);
			bool logging;
	};
#endif

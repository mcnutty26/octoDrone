#ifndef EOW_PROGRAMS_AODV
	#define EOW_PROGRAMS_AODV

	#include <string>
	#include <Drone.hpp>
	#include <CommMod.hpp>
	#include <Message.hpp>

	typedef std::vector<std::vector<std::vector<double>>> data_type;

	class AodvComms: public Drone {
		public:
			AodvComms(CommMod*, double, double, double, double, Environment*, int, int*);
			bool message_callback(Message*);
			void run();
		private:
			int m_task;
			int* m_flag;
			void quit();
	};
#endif
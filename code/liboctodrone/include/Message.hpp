#ifndef EOW_MESSAGE_HPP
	#define EOW_MESSAGE_HPP

	#include <ctime>
	#include <string>

	class Message
	{
		public:
			Message(std::string type);
			time_t get_current_time();
			virtual std::string to_string() = 0;
			//virtual void from_string(std::string message) = 0;
			std::string get_type();
		private:
			std::string type;
	};

#endif

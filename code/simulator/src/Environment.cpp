#include "Environment.hpp"

#include<cmath>
#include<thread>
#include<atomic>
#include "Messageable.hpp"


std::atomic_flag lock_broadcast = ATOMIC_FLAG_INIT;

Environment::Environment(BaseStation* base, std::vector<Messageable*> mess, std::map<std::string, data_type> sensor_data){
	baseStation = base;
	messageables = mess;
	data = sensor_data;
};

//should not be called by anything other than the main thread
void Environment::addData(std::string type, data_type d)
{
	data[type] = d;
}

//should not be called by anything other than the main thread
void Environment::addMessageable(Messageable* m)
{
	messageables.push_back(m);
}

//thread safe (I hope) may be a little slow though... meh, it'll be fine (again... I hope)
void Environment::broadcast(std::string message, double xOrigin, double yOrigin, double zOrigin, double range)
{
	while(lock_broadcast.test_and_set()){}
	for(auto m:messageables)
	{
		//if messageable is within range
		if(pow(m->getX() - xOrigin,2) + pow(m->getY() - yOrigin, 2) + pow(m->getZ() - zOrigin, 2) < pow(range, 2))
		{
			m->receive_message(message);
		}
	}
	lock_broadcast.clear();
}

void Environment::run()
{
	std::vector<std::thread> threads;
	for(auto x : messageables)
	{
		threads.emplace_back(&Messageable::run, x);
		threads.emplace_back(&Messageable::runCommMod, x);
		//threads.emplace_back(new boost::thread(boost::bind(&Messageable::run, x)));
		//threads.emplace_back(new boost::thread(boost::bind(&Messageable::runCommMod, x)));
	}

	for(std::vector<std::thread>::size_type i = 0; i < threads.size(); ++i)
	{
		threads[i].join();
	}
}

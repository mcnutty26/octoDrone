#include "Environment.hpp"

#include<cmath>
#include<boost/thread.hpp>
#include<atomic>


std::atomic_flag lock_broadcast = ATOMIC_FLAG_INIT;

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
	std::vector<boost::thread> threads;
	for(auto x : messageables)
	{
		threads.emplace_back(x->run);
		threads.emplace_back(x->runCommMod);
	}

	for(std::vector<boost::thread>::size_type i = 0; i < threads.size(); ++i)
	{
		threads[i].join();
	}
}

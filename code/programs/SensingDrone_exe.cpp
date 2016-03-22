#include "SensingDrone.hpp"
#include "SensingBaseStation.hpp"
#include <Environment.hpp>
#include <Basic_addressed.hpp>
#include <CommMod.hpp>
#include <map>
#include <atomic>

int main(int argv, char* argc[]){
	//create the sensor map and optionally add in some data
	std::map<std::string, data_type>* sensor_map = new std::map<std::string, data_type>;
	//data_type* sensor_data = new data_type();
	//sensor_map.insert(std::pair<std::string, int>("blank", sensor_data));

	//create the environment and comm module
	Environment* env = new Environment(*sensor_map, 1.0);
	std::atomic_flag stdout_lock = ATOMIC_FLAG_INIT;

	CommMod* comm_basic1 = new Basic_addressed(env, &stdout_lock, "10.0.0.1");
	CommMod* comm_basic2 = new Basic_addressed(env, &stdout_lock, "10.0.0.2");

	CommMod* comm_basic_base = new Basic_addressed(env, &stdout_lock, "10.0.0.255");

	//creat and add base station
	SensingBaseStation* basestation = new SensingBaseStation(comm_basic_base, 0.0, 0.0, 0.0, 1.0, 10.0, 1.0, 10.0);
	env->setBaseStation(basestation);

	//create and add drones
	SensingDrone* drone1 = new SensingDrone(comm_basic1, 1.0, 1.0, 0.0, 0.0, 0.5, env, false);
	SensingDrone* drone2 = new SensingDrone(comm_basic2, 2.0, 2.0, 0.0, 0.0, 0.5, env, true);
	env->addDrone(drone1);
	env->addDrone(drone2);

	//run the simulation
	env->run();
}
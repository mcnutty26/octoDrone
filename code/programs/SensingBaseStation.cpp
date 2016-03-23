#include "SensingBaseStation.hpp"
#include <iostream>
#include <Basic_message_addressed.hpp>




SensingBaseStation::SensingBaseStation(CommMod* cm, double xp, double yp, double zp, double areaX1, double areaY1, double areaX2, double areaY2) : BaseStation(cm, xp, yp, zp) {
	this->areaX1 = areaX1;
	this->areaX2 = areaX2;
	this->areaY1 = areaY1;
	this->areaY2 = areaY2;
}

void SensingBaseStation::run() {

	// Wait for a certain amount of time to discover number of drones
	// Drone IPs are hardcoded in for now
	droneIPs.push_back("10.0.0.1");
	droneIPs.push_back("10.0.0.2");
	int numDrones = 2;


	// Once the number of drones is known, distribute the area
	double x1s[numDrones];
	double y1s[numDrones];
	double x2s[numDrones];
	double y2s[numDrones];
	double xSize = (areaX2 - areaX1) / numDrones;
	for (int i = 0; i < numDrones; i++)
	{
		x1s[i] = (i * xSize) + areaX1;
		x2s[i] = ((i + 1) * xSize) + areaX1;
		y1s[i] = areaY1;
		y2s[i] = areaY2;
	}

	// broadcast messages to each drone so that they know which area they should be measuring
	for (int i = 0; i < numDrones; i++)
	{
		std::string message = std::string("NEWAREA=") + std::to_string(x1s[i]) + "," + std::to_string(y1s[i]) + "," + std::to_string(x2s[i]) + "," + std::to_string(y2s[i]);

		std::cout << "Sending message to " << droneIPs.at(i) << ": " << message << std::endl;

		//Basic_message_addressed* new_message = new Basic_message_addressed("HELLO", droneIPs.at(i), "");

		//std::cout << "THE MESSAGE IS:::" << std::endl;
		//std::cout << new_message->to_string() << std::endl;

		send_message(new Basic_message_addressed(message, droneIPs.at(i), ""));
	}

	std::cout << "Basestation thread ended" << std::endl;
}

bool SensingBaseStation::message_callback(Message* message) {
	return false;
}

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

///Modified implementation of Drone for the Parrot AR 2 drone

#include "Drone.hpp"
#include <cmath>
#include "Messageable.hpp"
#include <iostream>

#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define PI 3.14159265
double target_time = 0;
bool was_moving;

Drone::Drone(CommMod* cm, double iX, double iY, double iZ, double maxSpeed, Environment* e)
:Messageable(cm, iX, iY, iZ)
{
	this->maxSpeed = maxSpeed;
	env = e;
	execute("TAKEOFF", 0.0);
}

bool Drone::isAlive()
{
	return alive;
}

void Drone::kill()
{
	alive = false;
	execute("LAND", 0.0);
}

void Drone::upkeep()
{
	if (hasFinishedMoving() && was_moving){
		execute("STOP", 0.0);
	}
	was_moving = hasFinishedMoving();
}

void Drone::move(Direction direction, double speed, double distance)
{
	double radcon = PI/180;

	double normalised_speed = speed;
	if (normalised_speed > 1){
		normalised_speed = 1;
	} else if (normalised_speed < 0){
		normalised_speed = 0;
	}

	switch(direction)
	{
		case Direction::UP:
			execute("UP", normalised_speed);
			position.z += distance;
			break;
		case Direction::DOWN:
			execute("DOWN", normalised_speed);
			position.z -= distance;
			break;
		case Direction::LEFT:
			execute("LEFT", normalised_speed);
			position.x += distance*sin((ang - 90)*radcon);
			position.y += distance*cos((ang - 90)*radcon);
			break;
		case Direction::RIGHT:
			execute("RIGHT", normalised_speed);
			position.x += distance*sin((ang + 90)*radcon);
			position.y += distance*cos((ang + 90)*radcon);
			break;
		case Direction::FORWARD:
			execute("FRONT", normalised_speed);
			position.x += distance*sin((ang)*radcon);
			position.y += distance*cos((ang)*radcon);
			break;
		case Direction::BACK:
			execute("BACK", normalised_speed);
			position.x += distance*sin((ang + 180)*radcon);
			position.y += distance*cos((ang + 180)*radcon);
			break;
	}
	target_time = getTime() + (1000*distance);
}

double Drone::getMaxSpeed()
{
	return maxSpeed;
}

double Drone::getAngle()
{
	return ang;
}

bool Drone::hasFinishedMoving()
{
	return getTime() > target_time;
}

void Drone::turn(double dAngle)
{
	//get the value within acceptable bounds
	double to_turn = dAngle;
	while (to_turn >= 360){
		to_turn -= 360;
	}
	while (to_turn <= -360){
		to_turn += 360;
	}

	//is the turn left or right?
	if (to_turn > 0){
		execute("CLOCKWISE", 0.5);
	} else {
		execute("COUNTERCLOCKWISE", 0.5);
	}

	ang += to_turn;
	target_time += getTime() + abs(to_turn)/360;
}

double Drone::sense(std::string type)
{
	return env->getData(type, position.x, position.y, position.z);
}

///Takes a command and sends it to the node server which is connected to the drone
void Drone::execute(std::string command, double arg){
	//set up connection variables
	#define SOCK_PATH "parrot.sock"
	int s, len;
	struct sockaddr_un remote;
	char str[100];

	//prepare the request as a c string
	strcat(str, command.c_str());
	strcat(str, ";");
	strcat(str, std::to_string(arg).c_str());

	//get a handle for the socket
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		std::cout << "error@nodeServer: getting address of socket " << SOCK_PATH << std::endl;
		exit(1);
	}

	//connect to the socket
	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCK_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	if (connect(s, (struct sockaddr *)&remote, len) == -1) {
		std::cout << "error@nodeServer: connecting to socket " << SOCK_PATH << std::endl;
		exit(1);
	}

	//send the request
	if (send(s, str, strlen(str), 0) == -1) {
		std::cout << "error@nodeServer: sending via socket" << SOCK_PATH << std::endl;
		exit(1);
	}

	//close the connection
	close(s);
}

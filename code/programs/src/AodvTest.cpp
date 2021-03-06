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

#include "AodvTest.hpp"
#include <iostream>
#include <Basic_addressed_message.hpp>
#include <Basic_message.hpp>
#include <chrono>
#include <thread>
#include <atomic>

AodvTest::AodvTest(CommMod* cm, double xp, double yp, double zp, double speed, Environment* env, int task, int* flag, std::atomic_flag* lock): Drone(cm, xp, yp, zp, speed, env){
	m_task = task;
	m_flag = flag;
	m_lock = lock;
	m_env = env;
};

bool AodvTest::message_callback(Message*){
    return false;
}

void AodvTest::run(){
	switch(m_task){
		case 0:
			while(m_lock->test_and_set()){}
			std::cout << "Sink waiting for message" << std::endl;
			m_lock->clear();
			{
				std::string incoming = wait_for_message()->to_string();
				while(m_lock->test_and_set()){}
				std::cout << incoming << std::endl;
				m_lock->clear();
			}
			*m_flag = 1;
			break;
		case 1:
			while(m_env->getTime() < 500){}
			while(m_lock->test_and_set()){}
			std::cout << "Source sending message" << std::endl;
			m_lock->clear();
			send_message(new Basic_addressed_message("TEST MESSAGE PLEASE IGNORE", "10.0.0.1", ""));
			break;
		case -1:
			while (*m_flag != 1){
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			break;
	}
	send_message(new Basic_message("KILL"));
}

/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "billingsystem.hpp"
#include "billingsystem_interface.hpp"
#include "network/common.hpp"
#include "network/tcp_packet.hpp"
#include "network/udp_packet.hpp"
#include "network/message_handler.hpp"
#include "thread/threadpool.hpp"
#include "server/componentbridge.hpp"
#include "server/components.hpp"


#include "baseapp/baseapp_interface.hpp"
#include "cellapp/cellapp_interface.hpp"
#include "baseappmgr/baseappmgr_interface.hpp"
#include "cellappmgr/cellappmgr_interface.hpp"
#include "loginapp/loginapp_interface.hpp"

namespace KBEngine{
	
ServerConfig g_serverConfig;
KBE_SINGLETON_INIT(BillingSystem);

//-------------------------------------------------------------------------------------
BillingSystem::BillingSystem(Mercury::EventDispatcher& dispatcher, 
			 Mercury::NetworkInterface& ninterface, 
			 COMPONENT_TYPE componentType,
			 COMPONENT_ID componentID):
	ServerApp(dispatcher, ninterface, componentType, componentID),
	mainProcessTimer_()
{
}

//-------------------------------------------------------------------------------------
BillingSystem::~BillingSystem()
{
	mainProcessTimer_.cancel();
	KBEngine::sleep(300);
}

//-------------------------------------------------------------------------------------
bool BillingSystem::initializeWatcher()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool BillingSystem::run()
{
	return ServerApp::run();
}

//-------------------------------------------------------------------------------------
void BillingSystem::handleTimeout(TimerHandle handle, void * arg)
{
	switch (reinterpret_cast<uintptr>(arg))
	{
		case TIMEOUT_TICK:
			this->handleMainTick();
			break;
		default:
			break;
	}

	ServerApp::handleTimeout(handle, arg);
}

//-------------------------------------------------------------------------------------
void BillingSystem::handleMainTick()
{
	 //time_t t = ::time(NULL);
	 //DEBUG_MSG("BillingSystem::handleGameTick[%"PRTime"]:%u\n", t, time_);
	
	g_kbetime++;
	threadPool_.onMainThreadTick();
	getNetworkInterface().handleChannels(&BillingSystemInterface::messageHandlers);
}

//-------------------------------------------------------------------------------------
bool BillingSystem::initializeBegin()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool BillingSystem::inInitialize()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool BillingSystem::initializeEnd()
{
	mainProcessTimer_ = this->getMainDispatcher().addTimer(1000000 / g_kbeSrvConfig.gameUpdateHertz(), this,
							reinterpret_cast<void *>(TIMEOUT_TICK));

	return initDB();
}

//-------------------------------------------------------------------------------------		
bool BillingSystem::initDB()
{
	return true;
}

//-------------------------------------------------------------------------------------
void BillingSystem::finalise()
{
	ServerApp::finalise();
}

//-------------------------------------------------------------------------------------
void BillingSystem::onRegisterNewApp(Mercury::Channel* pChannel, int32 uid, std::string& username, 
						int8 componentType, uint64 componentID, 
						uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport)
{
	ServerApp::onRegisterNewApp(pChannel, uid, username, componentType, componentID, 
						intaddr, intport, extaddr, extport);

}


//-------------------------------------------------------------------------------------
void BillingSystem::reqCreateAccount(Mercury::Channel* pChannel, 
							 std::string& accountName, 
							 std::string& password)
{
}

//-------------------------------------------------------------------------------------
void BillingSystem::onAccountLogin(Mercury::Channel* pChannel, 
						   std::string& accountName, 
						   std::string& password)
{
}


//-------------------------------------------------------------------------------------

}
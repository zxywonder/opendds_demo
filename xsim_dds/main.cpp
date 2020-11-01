#include <iostream>
#include <boost/thread.hpp>
#include <TopSimDataInterface/TSDataInteractManager.h>
#include <TopSimMMF_Inter/Defined.h>

int main() 
{

	DataManager->InitializeWithConfigure("config/config-dist.ini");

	auto Domain =  DataManager->CreateDomain("101");

	Domain->PublishTopic(Huaru_Simulation_TruthEvent_Basic);

	while (true)
	{
		boost::this_thread::sleep(boost::chrono::seconds(3));
	}

	return 0;
}
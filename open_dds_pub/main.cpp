
#include <iostream>
#include <dds/DdsDcpsDomainC.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include "open_dds_idl/dds_demoTypeSupportImpl.h"
#include "open_dds_idl/dds_demoC.h"

int main(int argc, char *argv[]) 
{
	try {

		DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);

		DDS::DomainParticipant_var participant =

			dpf->create_participant(102, // domain ID

				PARTICIPANT_QOS_DEFAULT,

				0,  // No listener required

				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!participant) {

			std::cerr << "create_participant failed." << std::endl;

			return 1;
		}

		// Register TypeSupport (Messenger::Message)
		demo::DataTypeTypeSupport_var ts =
			new Messenger::MessageTypeSupportImpl;

		if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
			ACE_ERROR_RETURN((LM_ERROR,
				ACE_TEXT("ERROR: %N:%l: main() -")
				ACE_TEXT(" register_type failed!\n")),
				1);
		}
	
	}
	catch (...)
	{

	}

	return 0;
}
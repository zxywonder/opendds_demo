
#include <iostream>
#include <boost/thread.hpp>
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

		// 注册主题类型
		demo::DataTypeTypeSupport_var ts =
			new demo::DataTypeTypeSupportImpl();

		if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
			ACE_ERROR_RETURN((LM_ERROR,
				ACE_TEXT("ERROR: %N:%l: main() -")
				ACE_TEXT(" register_type failed!\n")),
				1);
		}

		// 
		CORBA::String_var type_name = ts->get_type_name();

		DDS::Topic_var topic =	participant->create_topic("Pos Demo",

				type_name,

				TOPIC_QOS_DEFAULT,

				0,   // No listener required

				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!topic) {

			std::cerr << "create_topic failed." << std::endl;

			return 1;

		}

		DDS::Publisher_var pub =participant->create_publisher(PUBLISHER_QOS_DEFAULT,
				0,   // No listener required
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);
		if (!pub) {

			std::cerr << "create_publisher failed." << std::endl;

			return 1;

		}

		// Create the datawriter

		DDS::DataWriter_var writer =

			pub->create_datawriter(topic,

				DATAWRITER_QOS_DEFAULT,

				0,   // No listener required

				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!writer) {

			std::cerr << "create_datawriter failed." << std::endl;

			return 1;

		}

		demo::DataTypeDataWriter_var data_writer = demo::DataTypeDataWriter::_narrow(writer);
	
		int idx = 0;

		while (true)
		{
			demo::DataType data;
			data.Handle = ++idx;
			data.Timestamp = 30.0;

			data_writer->write(data, DDS::HANDLE_NIL);

			boost::this_thread::sleep(boost::chrono::seconds(2));
		}

		participant->delete_contained_entities();

		dpf->delete_participant(participant);

		TheServiceParticipant->shutdown();
	}
	catch (...)
	{

	}

	return 0;
}

#include <iostream>
#include <boost/thread.hpp>
#include <dds/DdsDcpsDomainC.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include "open_dds_idl/dds_demoTypeSupportImpl.h"
#include "open_dds_idl/dds_demoC.h"


class DataReaderListenerImpl : public DDS::DataReaderListener
{
	virtual void on_requested_deadline_missed(
		::DDS::DataReader_ptr reader,
		const ::DDS::RequestedDeadlineMissedStatus & status) {};

	virtual void on_requested_incompatible_qos(
		::DDS::DataReader_ptr reader,
		const ::DDS::RequestedIncompatibleQosStatus & status) {};

	virtual void on_sample_rejected(
		::DDS::DataReader_ptr reader,
		const ::DDS::SampleRejectedStatus & status) {};

	virtual void on_liveliness_changed(
		::DDS::DataReader_ptr reader,
		const ::DDS::LivelinessChangedStatus & status) {};

	virtual void on_subscription_matched(
		::DDS::DataReader_ptr reader,
		const ::DDS::SubscriptionMatchedStatus & status) {};

	virtual void on_sample_lost(
		::DDS::DataReader_ptr reader,
		const ::DDS::SampleLostStatus & status) {};

	void on_data_available(DDS::DataReader_ptr reader)
	{
		try {

			demo::DataTypeDataReader_var reader_i = demo::DataTypeDataReader::_narrow(reader);

			if (!reader_i) {

				std::cerr << "read: _narrow failed." << std::endl;

				return;

			}

			demo::DataType data;

			DDS::SampleInfo si;

			DDS::ReturnCode_t status = reader_i->take_next_sample(data, si);

			if (status == DDS::RETCODE_OK) {

				if (si.valid_data == 1)
				{

					std::cout << " data:id = " << data.Handle << std::endl;

				}
				else if (si.instance_state == DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
				{
					std::cout << "instance is disposed" << std::endl;
				}
				else if (si.instance_state == DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
				{
					std::cout << "instance is unregistered" << std::endl;
				}
				else
				{
					std::cerr << "ERROR: received unknown instance state "
						<< si.instance_state << std::endl;

				}
			}
			else if (status == DDS::RETCODE_NO_DATA) {

				std::cerr << "ERROR: reader received DDS::RETCODE_NO_DATA!" << std::endl;

			}
			else {

				std::cerr << "ERROR: read data: Error: " << status << std::endl;

			}
		}
		catch (...) {}
	}
};

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

		DDS::Topic_var topic = participant->create_topic("Pos Demo",

			type_name,

			TOPIC_QOS_DEFAULT,

			0,   // No listener required

			OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!topic) {

			std::cerr << "create_topic failed." << std::endl;

			return 1;

		}
		// Create the subscriber
		
		DDS::Subscriber_var sub = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
			0,   // No listener required
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);
		if (!sub) {

			std::cerr << "Failed to create_subscriber." << std::endl;

			return 1;

		}

		// Create the Datareader

		DDS::DataReader_var   dr =

			sub->create_datareader(topic,

				DATAREADER_QOS_DEFAULT,

				new DataReaderListenerImpl(),   // No listener required

				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!dr) {

			std::cerr << "create_datawriter failed." << std::endl;

			return 1;

		}



		while (true)
		{
			boost::this_thread::sleep(boost::chrono::seconds(1));
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
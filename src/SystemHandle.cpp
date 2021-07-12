#include "Publisher.hpp"

#include <is/systemhandle/SystemHandle.hpp>
#include <is/utils/Log.hpp>



#include <iostream>
#include <thread>


namespace eprosima {
namespace is {
namespace sh {
namespace monitormanager {


class SystemHandle : public virtual FullSystem
{
    public:
        SystemHandle(): FullSystem()
        , logger_("is::sh::MonitorManager")
        {
        }

        ~SystemHandle()
        {
        }

        bool configure(const core::RequiredTypes & ,
                        const YAML::Node & configuration,
                        TypeRegistry & /*type_registry*/) override
        {

            logger_ << utils::Logger::Level::INFO << "configure" << std::endl;

            if (configuration) 
            {
                logger_ << utils::Logger::Level::INFO << "configuration" << std::endl;
            }

            return true;
        }

        bool okay() const override
        {            
            return true;
        }

        bool spin_once() override
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(100ms);
            return okay();
        }

        bool subscribe(
            const std::string& topic_name,
            const xtypes::DynamicType& message_type,
            SubscriptionCallback* callback,
            const YAML::Node& configuration) override final
        {
            logger_ << utils::Logger::Level::INFO << "subscribe" << std::endl;

            logger_ << utils::Logger::Level::INFO 
                << "subscribe topic_name:" << topic_name 
                << " message_type: " << message_type << std::endl;

            if (callback) 
            {
                logger_ << utils::Logger::Level::INFO << "callback" << std::endl;
            }

            if (configuration) 
            {
                logger_ << utils::Logger::Level::INFO << "Configuration" << std::endl;
            }

            return true;
        }

        bool is_internal_message(
            void* /*filter_handle*/) override final
        {
            return false;
        }

        std::shared_ptr<TopicPublisher> advertise(
            const std::string& topic_name,
            const xtypes::DynamicType& message_type,
            const YAML::Node& configuration) override final        
        {

            
            logger_ << utils::Logger::Level::INFO << "advertise" << std::endl;

            auto publisher = std::make_shared<Publisher>();

            logger_ << utils::Logger::Level::INFO 
                << "advertise topic_name:" << topic_name 
                << " message_type: " << message_type << std::endl;

            if (configuration) 
            {
                logger_ << utils::Logger::Level::INFO << "Configuration" << std::endl;
            }

            return publisher;
        }

    private:

        utils::Logger logger_;

};

} //  namespace monitormanager
} //  namespace sh
} //  namespace is
} //  namespace gcs

IS_REGISTER_SYSTEM("monitormanager", eprosima::is::sh::monitormanager::SystemHandle)
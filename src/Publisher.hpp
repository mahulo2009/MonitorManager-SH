#ifndef _IS_SH_MONITORMANAGER__INTERNAL__PUBLISHER_HPP_
#define _IS_SH_MONITORMANAGER__INTERNAL__PUBLISHER_HPP_

#include <is/systemhandle/SystemHandle.hpp>

#include <is/utils/Log.hpp>

namespace eprosima {
namespace is {
namespace sh {
namespace monitormanager {

class Publisher : public virtual is::TopicPublisher
{
public:

    Publisher()
        : logger_("is::sh::MONITORMANAGER::Publisher")
    {
    }

    virtual ~Publisher() override = default;

    bool publish(
            const xtypes::DynamicData& xtypes_message)
    {
        if (xtypes_message)
        {
             logger_ << utils::Logger::Level::INFO << "xtypes_message" << std::endl;
        }

        return true;
    }

    utils::Logger logger_;
};


} //  namespace monitormanager
} //  namespace sh
} //  namespace is
} //  namespace eprosima

#endif //  _IS_SH_MONITORMANAGER__INTERNAL__PUBLISHER_HPP_
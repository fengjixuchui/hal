#include "log_channel_manager/log_channel_manager.h"

log_channel_manager::log_channel_manager()
{
    //log_manager::get_instance().get_gui_callback().add_callback("gui", std::bind(&log_channel_manager::logmanager_callback, this, std::placeholders::_1));
}

void log_channel_manager::logmanager_callback(const spdlog::details::log_msg& msg)
{
//    spdlog::level::level_enum t = msg.level;
//    std::string channel_name    = *msg.logger_name;
//    std::string msg_text        = msg.formatted.str();

//    channel_item* all_channel = nullptr;
//    channel_item* item        = nullptr;
//    for (auto element : m_permanent_items)
//    {
//        if (element->name().toStdString() == ALL_CHANNEL)
//        {
//            all_channel = element;
//        }
//        if (element->name().toStdString() == channel_name)
//        {
//            item = element;
//        }
//    }
//    if (item == nullptr || all_channel == nullptr)
//    {
//        for (auto element : m_temporary_items)
//        {
//            if (element->name().toStdString() == ALL_CHANNEL)
//            {
//                all_channel = element;
//            }
//            if (element->name().toStdString() == channel_name)
//            {
//                item = element;
//            }
//        }
//    }
//    if (all_channel == nullptr)
//    {
//        all_channel = add_channel(QString::fromStdString(ALL_CHANNEL));
//    }
//    if (item == nullptr)
//    {
//        item = add_channel(QString::fromStdString(channel_name));
//    }
//    all_channel->append_entry(new channel_entry(msg_text, t));
//    item->append_entry(new channel_entry(msg_text, t));
//    Q_EMIT updated(t, ALL_CHANNEL, msg_text);
//    Q_EMIT updated(t, channel_name, msg_text);
}

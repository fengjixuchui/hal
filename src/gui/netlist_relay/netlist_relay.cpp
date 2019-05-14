#include "netlist_relay/netlist_relay.h"

#include <functional>

#include <QDebug>

netlist_relay::netlist_relay(QObject* parent) : QObject(parent)
{
    register_callbacks();
}

netlist_relay::~netlist_relay()
{
    netlist_event_handler::unregister_callback("relay");
    net_event_handler::unregister_callback("relay");
    gate_event_handler::unregister_callback("relay");
    module_event_handler::unregister_callback("relay");
}

void netlist_relay::register_callbacks()
{
    netlist_event_handler::register_callback(
                "relay",
                std::function<void(netlist_event_handler::event, std::shared_ptr<netlist>, u32)>
                (std::bind(&netlist_relay::relay_netlist_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    net_event_handler::register_callback(
                "relay",
                std::function<void(net_event_handler::event, std::shared_ptr<net>, u32)>
                (std::bind(&netlist_relay::relay_net_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    gate_event_handler::register_callback(
                "relay",
                std::function<void(gate_event_handler::event, std::shared_ptr<gate>, u32)>
                (std::bind(&netlist_relay::relay_gate_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    module_event_handler::register_callback(
                "relay",
                std::function<void(module_event_handler::event, std::shared_ptr<module>, u32)>
                (std::bind(&netlist_relay::relay_module_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
}

void netlist_relay::relay_netlist_event(netlist_event_handler::event ev, std::shared_ptr<netlist> object, u32 associated_data)
{
    Q_EMIT netlist_event(ev, object, associated_data);
    //qDebug() << "relay_netlist_event called: event ID =" << ev << "for object at" << object.get();
}

void netlist_relay::relay_net_event(net_event_handler::event ev, std::shared_ptr<net> object, u32 associated_data)
{
    Q_EMIT net_event(ev, object, associated_data);
    //qDebug() << "relay_net_event called: event ID =" << ev << "for object at" << object.get();
}

void netlist_relay::relay_gate_event(gate_event_handler::event ev, std::shared_ptr<gate> object, u32 associated_data)
{
    Q_EMIT gate_event(ev, object, associated_data);
    //qDebug() << "relay_gate_event called: event ID =" << ev << "for object at" << object.get();
}

void netlist_relay::relay_module_event(module_event_handler::event ev, std::shared_ptr<module> object, u32 associated_data)
{
    if (!object)
        return; // SHOULD NEVER BE REACHED

    //Q_EMIT module_event(ev, object, associated_data);
    //qDebug() << "relay_module_event called: event ID =" << ev << "for object at" << object.get();

    switch (ev)
    {
    case module_event_handler::event::created:
    {
        //< no associated_data

//        module_item* parent_item = m_module_items.value(object->get_parent_module()->get_id());

//        if (!parent_item)
//            return; // SHOULD NOT BE POSSIBLE

        Q_EMIT module_created(object);
        break;
    }
    case module_event_handler::event::removed:
    {
        //< no associated_data

        Q_EMIT module_removed(object);
        break;
    }
    case module_event_handler::event::name_changed:
    {
        //< no associated_data

        Q_EMIT module_name_changed(object);
        break;
    }
    case module_event_handler::event::parent_changed:
    {
        //< no associated_data

        Q_EMIT module_name_changed(object);
        break;
    }
    case module_event_handler::event::submodule_added:
    {
        //< associated_data = id of added module

        Q_EMIT module_submodule_added(object, associated_data);
        break;
    }
    case module_event_handler::event::submodule_removed:
    {
        //< associated_data = id of removed module

        Q_EMIT module_submodule_removed(object, associated_data);
        break;
    }
    case module_event_handler::event::gate_inserted:
    {
        //< associated_data = id of inserted gate

        Q_EMIT module_gate_inserted(object, associated_data);
        break;
    }
    case module_event_handler::event::gate_removed:
    {
        //< associated_data = id of removed gate

        Q_EMIT module_gate_removed(object, associated_data);
        break;
    }
    case module_event_handler::event::net_inserted:
    {
        //< associated_data = id of added net

        Q_EMIT module_net_inserted(object, associated_data);
        break;
    }
    case module_event_handler::event::net_removed:
    {
        //< associated_data = id of removed net

        Q_EMIT module_net_removed(object, associated_data);
        break;
    }
    }
}

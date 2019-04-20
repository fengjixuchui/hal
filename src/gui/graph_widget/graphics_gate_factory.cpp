#include "graph_widget/graphics_gate_factory.h"

#include "graph_widget/graphics_items/advanced_graphics_gate.h"
#include "graph_widget/graphics_items/global_graphics_net.h"
#include "graph_widget/graphics_items/separated_graphics_net.h"
#include "graph_widget/graphics_items/standard_graphics_gate.h"
#include "graph_widget/graphics_items/standard_graphics_net.h"

#include "core/log.h"

//#include "netlist/gate.h"
//#include "netlist/net.h"

namespace graphics_gate_factory
{

QStringList graphics_gate_types()
{
    return QStringList
    {
        "Standard Gate"
    };
}

graphics_gate* create_graphics_gate(std::shared_ptr<gate> g, int type)
{
    if (!g)
    {
        log_error("gui", "Unable to create graphics_gate object. Factory function received a null pointer");
        return nullptr;
    }

    switch (type)
    {
    case 0: return new standard_graphics_gate(g); break;
    case 1: return new advanced_graphics_gate(g); break;
    default: return nullptr;
    }
}
}

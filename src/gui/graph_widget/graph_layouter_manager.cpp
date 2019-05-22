#include "graph_widget/graph_layouter_manager.h"

#include "graph_widget/graphics_items/global_graphics_net.h"
#include "graph_widget/graphics_items/graphics_item.h"
#include "graph_widget/graphics_scene.h"

#include "graph_widget/graphics_items/advanced_graphics_gate.h"
#include "graph_widget/graphics_items/graphics_net.h"
#include "graph_widget/graphics_items/red_pin_gate.h"
#include "graph_widget/graphics_items/separated_graphics_net.h"
#include "graph_widget/graphics_items/standard_graphics_gate.h"
#include "graph_widget/graphics_items/standard_graphics_net.h"
#include "graph_widget/layouters/minimal_graph_layouter.h"
#include "graph_widget/layouters/orthogonal_graph_layouter.h"
#include "graph_widget/layouters/placeholder_layouter.h"
#include "graph_widget/layouters/old_standard_graph_layouter.h"

#include "gui_globals.h"

#include <QRectF>

graph_layouter_manager::graph_layouter_manager(QObject* parent) : QObject(parent)
{
    // DEBUG CODE, NULLPTRS HERE ARE WRONG, NO NEED TO FIX, CLASS WILL BE REMOVED EVENTUALLY
    m_layouters.append(qMakePair(new minimal_graph_layouter(nullptr), 0));
    m_layouters.append(qMakePair(new orthogonal_graph_layouter(nullptr), 0));
//    m_layouters.append(qMakePair(new old_standard_graph_layouter(nullptr), 0));
//    m_layouters.append(qMakePair(new placeholder_layouter(nullptr), 0));

    // DEPRECATED, REPLACE WITH STYLE ADAPTER
    graphics_item::load_settings();
    //graphics_gate::load_settings();
    advanced_graphics_gate::load_settings();
    standard_graphics_gate::load_settings();
//    red_pin_gate::load_settings();

    graphics_net::load_settings();
    global_graphics_net::load_settings();
    separated_graphics_net::load_settings();
}

graph_layouter_manager& graph_layouter_manager::get_instance()
{
    static graph_layouter_manager instance;
    return instance;
}

graph_layouter_manager::~graph_layouter_manager()
{
}

graphics_scene* graph_layouter_manager::subscribe(QString layouter)
{
    for (auto& pair : m_layouters)
    {
        if (pair.first->name() == layouter)
        {
            pair.second++;

            pair.first->layout();

            //connect scenemanager to selection relay

            return pair.first->scene();
        }
    }
    return nullptr;
}

void graph_layouter_manager::unsubscribe(QString layouter)
{
    for (auto& pair : m_layouters)
    {
        if (pair.first->name() == layouter)
        {
            if (pair.second == 0)
                return;

            pair.second--;

            if (pair.second == 0)
            {
                pair.first->reset();
            }
        }
    }
}

QStringList graph_layouter_manager::get_layouters()
{
    QStringList list;
    for (auto pair : m_layouters)
        list.append(pair.first->name());

    return list;
}

QString graph_layouter_manager::get_name(QString layouter)
{
    for (auto pair : m_layouters)
    {
        if (pair.first->name() == layouter)
            return pair.first->name();
    }
    return "";
}

QString graph_layouter_manager::get_description(QString layouter)
{
    for (auto pair : m_layouters)
    {
        if (pair.first->name() == layouter)
            return pair.first->description();
    }
    return "";
}

#include "graph_widget/graphics_items/graphics_module.h"

#include "netlist/module.h"

graphics_module::graphics_module(std::shared_ptr<module> m)
{
    m_id = m->get_id();
    m_class = item_class::gate;

    m_name = QString::fromStdString(m->get_name());
//    m_type = QString::fromStdString(g->get_type());

//    for (std::string input_pin : g->get_input_pin_types())
//        m_input_pins.append(QString::fromStdString(input_pin));

//    for (std::string output_pin : g->get_output_pin_types())
//        m_output_pins.append(QString::fromStdString(output_pin));

//    m_gate  = g;

    //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges | ItemIsFocusable);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
}

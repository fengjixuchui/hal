#include "gui/graph_widget/graphics_items/graphics_gate.h"

#include "gui/graph_widget/graph_widget_constants.h"

#include "netlist/gate.h"

#include <QApplication>
#include <qmath.h>

graphics_gate::graphics_gate(std::shared_ptr<gate> g) : graphics_item(item_type::gate, g->get_id())
{
    m_name = QString::fromStdString(g->get_name());
    m_type = QString::fromStdString(g->get_type());

    for (std::string input_pin : g->get_input_pin_types())
        m_input_pins.append(QString::fromStdString(input_pin));

    for (std::string output_pin : g->get_output_pin_types())
        m_output_pins.append(QString::fromStdString(output_pin));

    //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges | ItemIsFocusable);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF graphics_gate::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

QPainterPath graphics_gate::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(0, 0, m_width, m_height));
    return path;
}

qreal graphics_gate::get_width() const
{
    return m_width;
}

qreal graphics_gate::get_height() const
{
    return m_height;
}

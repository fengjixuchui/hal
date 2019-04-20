#include "graph_widget/graphics_items/graphics_gate.h"

#include "graph_widget/graph_widget_constants.h"

#include "netlist/gate.h"

#include <QApplication>
#include <qmath.h>

graphics_gate::graphics_gate(std::shared_ptr<gate> g)
{
    m_id = g->get_id();
    m_class = item_class::gate;

    m_name = QString::fromStdString(g->get_name());
    m_type = QString::fromStdString(g->get_type());

    for (std::string input_pin : g->get_input_pin_types())
        m_input_pins.append(QString::fromStdString(input_pin));

    for (std::string output_pin : g->get_output_pin_types())
        m_output_pins.append(QString::fromStdString(output_pin));

    m_gate  = g;

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

std::string graphics_gate::get_input_pin_type_at_position(const size_t pos) const
{
    return (pos < (size_t)m_input_pins.size()) ? m_input_pins.at(pos).toStdString() : "";
}

std::string graphics_gate::get_output_pin_type_at_position(const size_t pos) const
{
    return (pos < (size_t)m_output_pins.size()) ? m_output_pins.at(pos).toStdString() : "";
}

QVariant graphics_gate::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange)
    {
            if(QApplication::mouseButtons() == Qt::LeftButton)
            {
                QPointF new_position = value.toPointF();
                int adjusted_x = qRound(new_position.x() / graph_widget_constants::grid_size) * graph_widget_constants::grid_size;
                int adjusted_y = qRound(new_position.y() / graph_widget_constants::grid_size) * graph_widget_constants::grid_size;
                return QPoint(adjusted_x, adjusted_y);
            }
            else
                return value;
        }
        else
            return QGraphicsItem::itemChange(change, value);
}

std::shared_ptr<gate> graphics_gate::get_gate() const
{
    return m_gate;
}

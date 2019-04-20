#include "graph_widget/graphics_items/graphics_node.h"

#include <QApplication>

graphics_node::graphics_node()
{

}

QRectF graphics_node::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

QPainterPath graphics_node::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(0, 0, m_width, m_height));
    return path;
}

qreal graphics_node::width() const
{
    return m_width;
}

qreal graphics_node::height() const
{
    return m_height;
}

QVariant graphics_node::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    // NOT FUNCTIONAL, NEEDS TO USE LAYOUT DATA
    if (change == ItemPositionChange)
    {
            if(QApplication::mouseButtons() == Qt::LeftButton)
            {
                QPointF new_position = value.toPointF();
                int adjusted_x = 0;
                int adjusted_y = 0;
                return QPoint(adjusted_x, adjusted_y);
            }
            else
                return value;
        }
        else
            return QGraphicsItem::itemChange(change, value);
}

#ifndef GRAPH_GRAPHICS_ITEM_H
#define GRAPH_GRAPHICS_ITEM_H

#include "def.h"

#include <QColor>
#include <QGraphicsItem>

class graphics_item : public QGraphicsItem
{
public:
    enum class item_class
    {
        gate,
        net,
        module
    };

    static void load_settings();
    static void set_lod(const qreal lod);

    graphics_item();

    u32 id() const;
    item_class get_item_class() const;

    void set_color(const QColor& color);

protected:
    static qreal s_lod;
    static QColor s_selection_color;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;

    item_class m_class;
    u32 m_id;
    QColor m_color;
};

#endif // GRAPH_GRAPHICS_ITEM_H

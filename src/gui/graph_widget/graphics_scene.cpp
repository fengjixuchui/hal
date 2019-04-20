#include "graph_widget/graphics_scene.h"

#include "netlist/gate.h"
#include "netlist/net.h"

#include "graph_widget/graph_widget_constants.h"
#include "graph_widget/graphics_gate_factory.h"
#include "graph_widget/graphics_items/graphics_gate.h"
#include "graph_widget/graphics_items/graphics_item.h"
#include "graph_widget/graphics_items/graphics_net.h"
#include "graph_widget/graphics_items/utility_items/gate_navigation_popup.h"
#include "gui_globals.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

qreal graphics_scene::s_lod = 0;

const qreal graphics_scene::s_grid_fade_start = 0.4;
const qreal graphics_scene::s_grid_fade_end = 1.0;

graphics_scene::grid_type graphics_scene::s_grid_type = graphics_scene::grid_type::lines;
bool graphics_scene::s_grid_clusters_enabled = true;

QColor graphics_scene::s_grid_base_line_color = QColor(30, 30, 30);
QColor graphics_scene::s_grid_cluster_line_color = QColor(15, 15, 15);

QColor graphics_scene::s_grid_base_dot_color = QColor(25, 25, 25);
QColor graphics_scene::s_grid_cluster_dot_color = QColor(170, 160, 125);

void graphics_scene::set_lod(qreal lod)
{
    s_lod = lod;

    if (lod >= s_grid_fade_start && lod <= s_grid_fade_end)
    {
        const qreal alpha = (lod - s_grid_fade_start) / (s_grid_fade_end - s_grid_fade_start);

        s_grid_base_line_color.setAlphaF(alpha);
        s_grid_cluster_line_color.setAlphaF(alpha);

        s_grid_base_dot_color.setAlphaF(alpha);
        s_grid_cluster_dot_color.setAlphaF(alpha);
    }
    else
    {
        const int alpha = 255;

        s_grid_base_line_color.setAlpha(alpha);
        s_grid_cluster_line_color.setAlpha(alpha);

        s_grid_base_dot_color.setAlpha(alpha);
        s_grid_cluster_dot_color.setAlpha(alpha);
    }
}

void graphics_scene::set_grid_base_line_color(const QColor& color)
{
    s_grid_base_line_color = color;
}

void graphics_scene::set_grid_cluster_line_color(const QColor& color)
{
    s_grid_cluster_line_color = color;
}

void graphics_scene::set_grid_base_dot_color(const QColor& color)
{
    s_grid_base_dot_color = color;
}

void graphics_scene::set_grid_cluster_dot_color(const QColor& color)
{
    s_grid_cluster_dot_color = color;
}

graphics_scene::graphics_scene(QObject* parent) : QGraphicsScene(parent),
    m_grid_enabled(true),
    m_left_gate_navigation_popup(new gate_navigation_popup(gate_navigation_popup::type::left)),
    m_right_gate_navigation_popup(new gate_navigation_popup(gate_navigation_popup::type::right))
{
    // TODO FIND OUT IF MANUAL CHANGE TO DEPTH IS NECESSARY / INCREASES PERFORMANCE
    //m_scene.setBspTreeDepth(10);

    connect(this, &graphics_scene::selectionChanged, this, &graphics_scene::handle_intern_selection_changed);
//    connect(this, &graph_scene::focusItemChanged, this, &graph_scene::handle_focus_item_changed);

    connect(&g_selection_relay, &selection_relay::selection_changed, this, &graphics_scene::handle_extern_selection_changed);
    connect(&g_selection_relay, &selection_relay::subfocus_changed, this, &graphics_scene::handle_extern_subfocus_changed);

    QGraphicsScene::addItem(m_left_gate_navigation_popup);
}

// IMPORTANT TODO UPDATE SELECTION WHEN ITEM GETS ADDED !!!
void graphics_scene::addItem(graphics_item* item)
{
    if (!item)
        return;

    QGraphicsScene::addItem(item);

    switch (item->get_item_class())
    {
    case graphics_item::item_class::gate:
        {
            graphics_gate* g = static_cast<graphics_gate*>(item);
            int i = 0;
            while (i < m_gate_vector.size())
            {
                if (g->get_gate()->get_id() < m_gate_vector.at(i).first)
                    break;

                i++;
            }
            m_gate_vector.insert(i, QPair<u32, graphics_gate*>(g->get_gate()->get_id(), g));
            return;
        }
    case graphics_item::item_class::net:
        {
            graphics_net* n = static_cast<graphics_net*>(item);
            int i = 0;
            while (i < m_net_vector.size())
            {
                if (n->get_net()->get_id() < m_net_vector.at(i).first)
                    break;

                i++;
            }
            m_net_vector.insert(i, QPair<u32, graphics_net*>(n->get_net()->get_id(), n));
            return;
        }
    case graphics_item::item_class::submodule:
    {
        return; // TODO
    }
    }
}

void graphics_scene::removeItem(graphics_item* item)
{
    if (!item)
        return;

    QGraphicsScene::removeItem(item);

    switch (item->get_item_class())
    {
    case graphics_item::item_class::gate:
        {
            graphics_gate* g = static_cast<graphics_gate*>(item);
            m_gate_vector.removeOne(QPair<u32, graphics_gate*>(g->get_gate()->get_id(), g));
            delete(g);
            return;
        }
    case graphics_item::item_class::net:
        {
            graphics_net* n = static_cast<graphics_net*>(item);
            m_net_vector.removeOne(QPair<u32, graphics_net*>(n->get_net()->get_id(), n));
            delete(n);
            return;
        }
    case graphics_item::item_class::submodule:
    {
        return; // TODO
    }
    }
}

const graphics_gate *graphics_scene::get_gate_item(const u32 id) const
{
    for (const QPair<u32, graphics_gate*>& pair : m_gate_vector)
    {
        if (pair.first > id)
            break;

        if (pair.first == id)
            return pair.second;
    }

    return nullptr;
}

void graphics_scene::update_utility_items()
{
    // TEST IMPLEMENTATION
    // EXPAND WITH MORE ITEMS

    if (g_selection_relay.m_focus_type == selection_relay::item_type::gate)
    {
        for (QPair<u32, graphics_gate*>& pair : m_gate_vector)
        {
            if (pair.first == g_selection_relay.m_selected_gates[0])
            {
                graphics_gate* g = pair.second;

                // IF (ANIMATE) ANIMATE ELSE DONT
                // ALTERNATIVELY ANIMATE IF SCENE HAS LESS THAN X ITEMS
                m_left_gate_navigation_popup->stop();
                m_left_gate_navigation_popup->set_height(g->get_height());
                m_left_gate_navigation_popup->start(QPointF(g->pos().x() - 120, g->pos().y()));
            }
        }
    }
    else
        m_left_gate_navigation_popup->stop();
}

bool graphics_scene::grid_enabled() const
{
    return m_grid_enabled;
}

void graphics_scene::set_grid_enabled(const bool value)
{
    m_grid_enabled = value;
}

void graphics_scene::connect_all()
{
    connect(this, &graphics_scene::selectionChanged, this, &graphics_scene::handle_intern_selection_changed);
//    connect(this, &graph_scene::focusItemChanged, this, &graph_scene::handle_focus_item_changed);

    connect(&g_selection_relay, &selection_relay::selection_changed, this, &graphics_scene::handle_extern_selection_changed);
    connect(&g_selection_relay, &selection_relay::subfocus_changed, this, &graphics_scene::handle_extern_subfocus_changed);
}

void graphics_scene::disconnect_all()
{
    disconnect(this, &graphics_scene::selectionChanged, this, &graphics_scene::handle_intern_selection_changed);
//    disconnect(this, &graph_scene::focusItemChanged, this, &graph_scene::handle_focus_item_changed);

    disconnect(&g_selection_relay, &selection_relay::selection_changed, this, &graphics_scene::handle_extern_selection_changed);
    disconnect(&g_selection_relay, &selection_relay::subfocus_changed, this, &graphics_scene::handle_extern_subfocus_changed);
}

void graphics_scene::delete_all_items()
{
    clear();
    m_gate_vector.clear();
    m_net_vector.clear();
}

void graphics_scene::handle_intern_selection_changed()
{
    int gates = 0;
    int nets = 0;
    int modules = 0;

    for (const QGraphicsItem* const item : selectedItems())
    {
        switch (static_cast<const graphics_item* const>(item)->get_item_class())
        {
        case graphics_item::item_class::gate:
        {
            g_selection_relay.m_selected_gates[gates] = static_cast<const graphics_item* const>(item)->id();
            ++gates;
            break;
        }
        case graphics_item::item_class::net:
        {
            g_selection_relay.m_selected_nets[nets] = static_cast<const graphics_item* const>(item)->id();
            ++nets;
            break;
        }
        case graphics_item::item_class::submodule:
        {
            g_selection_relay.m_selected_modules[modules] = static_cast<const graphics_item* const>(item)->id();
            ++modules;
            break;
        }
        }
    }

    // CHANGE SELECTION RELAY MEMEBERS DIRECTLY ?
    // TODO FIND OUT WHATS FASTER
    g_selection_relay.m_number_of_selected_gates = gates;
    g_selection_relay.m_number_of_selected_nets = nets;
    g_selection_relay.m_number_of_selected_modules = modules;

    // TEST CODE
    // ADD FOCUS DEDUCTION INTO RELAY ???
    if (gates + nets + modules == 1)
    {
        if (gates)
        {
            g_selection_relay.m_focus_type = selection_relay::item_type::gate;
            g_selection_relay.m_focus_id = g_selection_relay.m_selected_gates[0]; // UNNECESSARY ??? USE ARRAY[0] INSTEAD OF MEMBER VARIABLE ???
        }
        else if (nets)
        {
            g_selection_relay.m_focus_type = selection_relay::item_type::net;
            g_selection_relay.m_focus_id = g_selection_relay.m_selected_nets[0]; // UNNECESSARY ??? USE ARRAY[0] INSTEAD OF MEMBER VARIABLE ???
        }
        else
        {
            g_selection_relay.m_focus_type = selection_relay::item_type::module;
            g_selection_relay.m_focus_id = g_selection_relay.m_selected_modules[0]; // UNNECESSARY ??? USE ARRAY[0] INSTEAD OF MEMBER VARIABLE ???
        }
    }
    else
    {
        g_selection_relay.m_focus_type = selection_relay::item_type::none;
    }
    g_selection_relay.m_subfocus = selection_relay::subfocus::none;
    // END OF TEST CODE

    g_selection_relay.sort();
    g_selection_relay.relay_selection_changed(this);
}

void graphics_scene::handle_extern_selection_changed(void* sender)
{
    // CLEAR CURRENT SELECTION EITHER MANUALLY OR USING clearSelection()
    // UNCERTAIN ABOUT THE SENDER PARAMETER

    if (sender == this)
        return;

    bool original_value = blockSignals(true);

    clearSelection();

    if (g_selection_relay.m_number_of_selected_modules)
    {
        // ACTIONS HERE ARE DEPENDENT ON WHICH DATA APPROACH WILL ULTIMATELY BE USED IN THE SELECTION RELAY
    }

    if (g_selection_relay.m_number_of_selected_gates)
    {
        u32 index = 0;

        for (auto& element : m_gate_vector)
        {
            if (element.first == g_selection_relay.m_selected_gates[index])
            {
                element.second->setSelected(true);

                if (++index == g_selection_relay.m_number_of_selected_gates)
                    break;
            }
        }
    }

    if (g_selection_relay.m_number_of_selected_nets)
    {
        u32 index = 0;

        for (auto& element : m_net_vector)
        {
            if (element.first == g_selection_relay.m_selected_nets[index])
            {
                element.second->setSelected(true);

                if (++index == g_selection_relay.m_number_of_selected_nets)
                    break;
            }
        }
    }

    //update_utility_items();

    blockSignals(original_value);
}

void graphics_scene::handle_extern_subfocus_changed(void* sender)
{
    Q_UNUSED(sender)

    //update_utility_items();
}

void graphics_scene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}

//void graph_scene::handle_focus_item_changed(QGraphicsItem* new_focus_item, QGraphicsItem* old_focus_item, Qt::FocusReason reason)
//{
//    Q_UNUSED(old_focus_item)
//    Q_UNUSED(reason)

//    graphics_item* item = dynamic_cast<graphics_item*>(new_focus_item);

//    if (item)
//    {
//        switch (item->get_item_class())
//        {
//        case item_class::gate:
//        {
//            // SET CURRENT TO GATE ID AND CALL RELAY
//            qDebug() << "gate selected: " << item->id();
//            break;
//        }
//        case item_class::net:
//        {
//            // SET CURRENT TO NET ID AND CALL RELAY
//            break;
//        }
//        case item_class::submodule:
//        {
//            // SET CURRENT TO SUBMODULE ID AND CALL RELAY
//            break;
//        }
//        }
//    }
//    else
//        // CLEAR FOCUS
//        ;
//}

void graphics_scene::drawBackground(QPainter* painter, const QRectF& rect)
{
    if (!m_grid_enabled)
        return;

    if (s_lod < s_grid_fade_start)
        return;

    QFlags original_flags = painter->renderHints(); // UNNECESSARY ?
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    pen.setWidth(2);

    // OVERDRAW NEEDED BECAUSE QT FAILS AT BASIC GEOMETRY
    const int overdraw = 2;

    const int x_from = rect.left() - overdraw;
    const int x_to = rect.right() + overdraw;

    const int y_from = rect.top() - overdraw;
    const int y_to = rect.bottom() + overdraw;

    const int x_offset = x_from % graph_widget_constants::grid_size;
    const int y_offset = y_from % graph_widget_constants::grid_size;

    switch (s_grid_type)
    {
    case grid_type::lines:
    {
        QVarLengthArray<QLine, 512> base_lines;
        QVarLengthArray<QLine, 64> cluster_lines;

        for (int x = x_from - x_offset; x < x_to; x += graph_widget_constants::grid_size)
        {
            QLine line(x, y_from, x, y_to);
            if (x % (graph_widget_constants::grid_size * graph_widget_constants::cluster_size))
                base_lines.append(line);
            else
                cluster_lines.append(line);
        }

        for (int y = y_from - y_offset; y < y_to; y += graph_widget_constants::grid_size)
        {
            QLine line(x_from, y, x_to, y);
            if (y % (graph_widget_constants::grid_size * graph_widget_constants::cluster_size))
                base_lines.append(line);
            else
                cluster_lines.append(line);
        }

        pen.setColor(s_grid_base_line_color);
        painter->setPen(pen);

        painter->drawLines(base_lines.data(), base_lines.size());

        if (s_grid_clusters_enabled)
        {
            pen.setColor(s_grid_cluster_line_color);
            painter->setPen(pen);
        }

        painter->drawLines(cluster_lines.data(), cluster_lines.size());
        break;
    }

    case grid_type::dots:
    {
        QVector<QPoint> base_points;
        QVector<QPoint> cluster_points;

        for (int x = x_from - x_offset; x < x_to; x += graph_widget_constants::grid_size)
            for (int y = y_from - y_offset; y < y_to; y += graph_widget_constants::grid_size)
            {
                if ((x % (graph_widget_constants::grid_size * graph_widget_constants::cluster_size)) && (y % (graph_widget_constants::grid_size * graph_widget_constants::cluster_size)))
                    base_points.append(QPoint(x,y));
                else
                    cluster_points.append(QPoint(x,y));
            }

        pen.setColor(s_grid_base_dot_color);
        painter->setPen(pen);

        painter->drawPoints(base_points.data(), base_points.size());

        if (s_grid_clusters_enabled)
        {
            pen.setColor(s_grid_cluster_dot_color);
            painter->setPen(pen);
        }

        painter->drawPoints(cluster_points.data(), cluster_points.size());
        break;
    }
    }

    painter->setRenderHints(original_flags); // UNNECESSARY ?
}

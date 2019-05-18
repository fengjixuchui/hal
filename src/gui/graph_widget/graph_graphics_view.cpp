#include "graph_widget/graph_graphics_view.h"

#include "gui/graph_widget/graph_widget.h"
#include "gui/graph_widget/graph_layouter_manager.h"
#include "gui/graph_widget/graph_widget_constants.h"
#include "gui/graph_widget/graphics_items/global_graphics_net.h"
#include "gui/graph_widget/graphics_items/graphics_gate.h"
#include "gui/graph_widget/graphics_items/graphics_item.h"
#include "gui/graph_widget/graphics_items/separated_graphics_net.h"
#include "gui/graph_widget/graphics_items/standard_graphics_net.h"
#include "gui/graph_widget/graphics_scene.h"
#include "gui/gui_globals.h"

#include <QAction>
#include <QColorDialog>
#include <QMenu>
#include <QStyleOptionGraphicsItem>
#include <QWheelEvent>

graph_graphics_view::graph_graphics_view(QWidget* parent) : QGraphicsView(parent),
    m_antialiasing_enabled(false),
    m_cosmetic_nets_enabled(false),
    m_grid_enabled(true),
    m_grid_clusters_enabled(true),
    m_grid_type(graph_widget_constants::grid_type::lines)
{
    connect(this, &graph_graphics_view::customContextMenuRequested, this, &graph_graphics_view::show_context_menu);

    //    connect(&g_selection_relay, &selection_relay::focus_update, this, &graph_graphics_view::conditional_update);
    //    connect(&g_selection_relay, &selection_relay::subfocus_update, this, &graph_graphics_view::conditional_update);

    connect(&g_selection_relay, &selection_relay::subfocus_changed, this, &graph_graphics_view::conditional_update);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //setOptimizationFlags(QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing);
    //setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void graph_graphics_view::conditional_update()
{
    if (QStyleOptionGraphicsItem::levelOfDetailFromTransform(transform()) >= graph_widget_constants::gate_min_lod)
        update();
}

void graph_graphics_view::handle_change_color_action()
{
    QColor color = QColorDialog::getColor();

    if (color.isValid())
        m_item->set_color(color);
}

void graph_graphics_view::handle_cone_view_action()
{
    std::shared_ptr<gate> g = g_netlist->get_gate_by_id(m_item->id());

    if (!g)
        return;
}

void graph_graphics_view::paintEvent(QPaintEvent* event)
{
    qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(transform());

    // USE CONSISTENT METHOD NAMES
    graphics_scene::set_lod(lod);
    graphics_scene::set_grid_enabled(m_grid_enabled);
    graphics_scene::set_grid_clusters_enabled(m_grid_clusters_enabled);
    graphics_scene::set_grid_type(m_grid_type);

    graphics_item::set_lod(lod);

    separated_graphics_net::update_alpha();
    global_graphics_net::update_alpha();

    QGraphicsView::paintEvent(event);
}

//void graph_graphics_view::drawForeground(QPainter* painter, const QRectF& rect)
//{
//    Q_UNUSED(rect)

//    QRectF bar(0, 0, viewport()->width(), 30);
//    painter->resetTransform();
//    painter->fillRect(bar, QColor(0, 0, 0, 170));
//}

void graph_graphics_view::wheelEvent(QWheelEvent* event)
{
    if (event->delta() > 0)
        Q_EMIT zoomed_in(6);
    else
        Q_EMIT zoomed_out(6);

    event->accept();
}

void graph_graphics_view::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        setInteractive(false);
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    }

    switch (event->key())
    {
        case Qt::Key_Space:
        {
            //qDebug() << "Space pressed";
        }
        break;
    }

    event->ignore();

    //    QList<QGraphicsItem*> items = this->scene()->selectedItems();
    //    if (items.size() > 1 || items.size() == 0)
    //        return;
    //    gui_graph_gate* selected_gui_gate = dynamic_cast<gui_graph_gate*>(items.at(0));

    //    switch (state)
    //    {
    //        case gate_selected:
    //            gate_selected_key_pressed(event, selected_gui_gate);
    //            break;
    //        case gate_input_pins_selected:
    //            gate_left_pins_selected_key_pressed(event, selected_gui_gate);
    //            break;
    //        case gate_output_pins_selected:
    //            gate_right_pins_selected_key_pressed(event, selected_gui_gate);
    //            break;
    //        case net_selected:
    //            net_selected_key_pressed(event);
    //            break;
    //        case no_gate_selected:
    //            break;    // added to suppress warning
    //    }
}

void graph_graphics_view::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Control)
    {
        setInteractive(true);
        this->setDragMode(QGraphicsView::RubberBandDrag);
    }
}

void graph_graphics_view::show_context_menu(const QPoint& pos)
{
    graphics_scene* s = static_cast<graphics_scene*>(scene());

    if (!s)
        return;

    QMenu context_menu(this);

    QGraphicsItem* item = itemAt(pos);
    if (item)
    {
        m_item = static_cast<graphics_item*>(item);

        switch (m_item->get_item_class())
        {
            case graphics_item::item_class::gate:
            {
                QAction* color_action = context_menu.addAction("Change Color");
                QObject::connect(color_action, &QAction::triggered, this, &graph_graphics_view::handle_change_color_action);
                context_menu.addAction(color_action);

                QAction* cone_view_action = context_menu.addAction("Open in Cone View");
                QObject::connect(cone_view_action, &QAction::triggered, this, &graph_graphics_view::handle_cone_view_action);
                context_menu.addAction(cone_view_action);
                break;
            }
            default:
                break;
        }
    }
    else
    {
        QAction* antialiasing_action = context_menu.addAction("Antialiasing");
        QAction* cosmetic_action = context_menu.addAction("Cosmetic Nets");
        QMenu* grid_menu = context_menu.addMenu("Grid");
        QMenu* type_menu = grid_menu->addMenu("Type");
        QMenu* cluster_menu = grid_menu->addMenu("Clustering");
        QAction* lines_action = type_menu->addAction("Lines");
        QAction* dots_action = type_menu->addAction("Dots");
        QAction* none_action = type_menu->addAction("None");
        //connect(action, &QAction::triggered, this, SLOT);
    }

    context_menu.exec(mapToGlobal(pos));
    update();
}

void graph_graphics_view::toggle_antialiasing()
{
    m_antialiasing_enabled ? setRenderHint(QPainter::Antialiasing, false) : setRenderHint(QPainter::Antialiasing, true);
}

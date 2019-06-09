#include "gui/graph_widget/graph_widget.h"

#include "netlist/gate.h"
#include "netlist/module.h"
#include "netlist/net.h"

#include "gui/graph_widget/contexts/dynamic_context.h"
#include "gui/graph_widget/contexts/module_context.h"
#include "gui/graph_widget/graph_context_manager.h"
#include "gui/graph_widget/graph_navigation_widget.h"
#include "gui/graph_widget/graph_layout_progress_widget.h"
#include "gui/graph_widget/graphics_scene.h"
#include "gui/graph_widget/graphics_widget.h"
#include "gui/gui_globals.h"
#include "gui/overlay/dialog_overlay.h"
#include "gui/toolbar/toolbar.h"

#include <QInputDialog>
#include <QToolButton>
#include <QVBoxLayout>

graph_widget::graph_widget(QWidget* parent) : content_widget("Graph", parent),
    m_graphics_widget(new graphics_widget(this)),
    m_context(nullptr),
    m_overlay(new dialog_overlay(this)),
    m_navigation_widget(new graph_navigation_widget(this)),
    m_progress_widget(new graph_layout_progress_widget(this)),
    m_current_expansion(0)
{
    connect(m_navigation_widget, &graph_navigation_widget::navigation_requested, this, &graph_widget::handle_navigation_jump_requested);
    connect(m_navigation_widget, &graph_navigation_widget::close_requested, m_overlay, &dialog_overlay::hide);

    connect(m_overlay, &dialog_overlay::clicked, m_overlay, &dialog_overlay::hide);

    connect(m_graphics_widget->view(), &graph_graphics_view::module_double_clicked, this, &graph_widget::handle_module_double_clicked);

    m_overlay->hide();
    m_overlay->set_widget(m_navigation_widget);

    m_content_layout->addWidget(m_graphics_widget);
}

void graph_widget::setup_toolbar(toolbar* toolbar)
{
    Q_UNUSED(toolbar)
    // DEPRECATED
    // DELETE THIS METHOD AFTER CONTENT WIDGET REFACTOR

    QToolButton* context_one_button = new QToolButton();
    context_one_button->setText("Context 1");

    QToolButton* create_context_button = new QToolButton();
    create_context_button->setText("Create Context");

    QToolButton* change_context_button = new QToolButton();
    change_context_button->setText("Change Context");

    QToolButton* update_context_button = new QToolButton();
    update_context_button->setText("Update Context");

    connect(context_one_button, &QToolButton::clicked, this, &graph_widget::debug_module_one);
    connect(create_context_button, &QToolButton::clicked, this, &graph_widget::debug_create_context);
    connect(change_context_button, &QToolButton::clicked, this, &graph_widget::debug_change_context);
    connect(update_context_button, &QToolButton::clicked, this, &graph_widget::debug_update_context);

    toolbar->addWidget(context_one_button);
    toolbar->addWidget(create_context_button);
    toolbar->addWidget(change_context_button);
    toolbar->addWidget(update_context_button);
}

void graph_widget::handle_scene_available()
{
    m_graphics_widget->view()->setScene(m_context->scene());

    connect(m_overlay, &dialog_overlay::clicked, m_overlay, &dialog_overlay::hide);

    m_overlay->hide();
    m_progress_widget->stop();
    m_overlay->set_widget(m_navigation_widget);

    //m_layouter->scene()->connect_all();

    if (hasFocus())
        m_graphics_widget->setFocus();

    // FIND BETTER WAY TO DO THIS
    g_selection_relay.m_selected_gates[0] = m_current_expansion;
    g_selection_relay.m_number_of_selected_gates = 1;
    g_selection_relay.m_focus_type = selection_relay::item_type::gate;
    g_selection_relay.m_focus_id = m_current_expansion;
    // TODO SET CORRECT SUBSELECTION
    g_selection_relay.m_subfocus = selection_relay::subfocus::none;
    g_selection_relay.relay_selection_changed(nullptr);

    // JUMP TO THE GATE
    // JUMP SHOULD BE HANDLED SEPARATELY
//    if (item)
    //        m_graphics_widget->view()->ensureVisible(item);
}

void graph_widget::handle_scene_unavailable()
{
    m_graphics_widget->view()->setScene(nullptr);

    disconnect(m_overlay, &dialog_overlay::clicked, m_overlay, &dialog_overlay::hide);

    m_progress_widget->set_direction(graph_layout_progress_widget::direction::right);
    //m_progress_widget->set_direction(graph_layout_progress_widget::direction::left);

    //m_layouter->scene()->disconnect_all();

    m_overlay->set_widget(m_progress_widget);
    m_progress_widget->start();

    if (m_overlay->isHidden())
        m_overlay->show();
}

void graph_widget::handle_context_deleted()
{
    m_graphics_widget->view()->setScene(nullptr);
    m_context = nullptr;

    // SHOW SOME KIND OF "NO CONTEXT SELECTED" WIDGET
    // UPDATE OTHER DATA, LIKE TOOLBUTTONS
}

void graph_widget::keyPressEvent(QKeyEvent* event)
{
    if (!m_context)
        return;

    if (!m_context->available())
        return;

    //if (m_context && m_context->available())

    switch (event->key())
    {
    case Qt::Key_Left:
    {
        handle_navigation_left_request();
        break;
    }
    case Qt::Key_Right:
    {
        handle_navigation_right_request();
        break;
    }
    case Qt::Key_Up:
    {
        handle_navigation_up_request();
        break;
    }
    case Qt::Key_Down:
    {
        handle_navigation_down_request();
        break;
    }
    case Qt::Key_Backspace:
    {
        handle_module_up_request();
        break;
    }
    default: break;
    }
}

void graph_widget::handle_navigation_jump_requested(const u32 from_gate, const u32 via_net, const u32 to_gate)
{
    // ASSERT INPUTS ARE VALID ?
    std::shared_ptr<gate> g = g_netlist->get_gate_by_id(from_gate);

    if (!g)
        return;

    std::shared_ptr<net> n = g_netlist->get_net_by_id(via_net);

    if (!n)
        return;

    g = g_netlist->get_gate_by_id(to_gate);

    if (!g)
        return;

    bool contains_net = false;
    bool contains_gate = false;

    if (m_context->nets().contains(via_net))
        contains_net = true;

    if (m_context->gates().contains(to_gate))
        contains_gate = true;

    bool update_necessary = false;

    if (!contains_net || !contains_gate)
    {
        // THIS HAS TO BE DELEGATED TO THE CONTEXT
//        if (m_context->scope())
//        {
//            // DRAW NON MEMBER NETS AND GATES AND SHADE THEM DIFFERENTLY ?
//            std::shared_ptr<module> m = g_netlist->get_module_by_id(m_context->scope());

//            if (!m)
//                return; // INVALID SCOPE

//            if (!m->contains_net(n, true))
//                return; // NET OUT OF SCOPE

//            if (!m->contains_gate(g, true))
//                return; // GATE OUT OF SCOPE
//        }

        QSet<u32> gates;
        QSet<u32> nets;

        if (!contains_net)
            nets.insert(via_net);

        if (!contains_gate)
            gates.insert(to_gate);

        // ADD TO CONTEXT
        m_context->add(QSet<u32>(), gates, nets); // EMPTY SET DEBUG CODE
        update_necessary = true;
    }
    else
    {
        // TODO FIX THIS MESS
        m_overlay->hide();
        //if (hasFocus())
            m_graphics_widget->setFocus();
    }

    // SELECT IN RELAY
    g_selection_relay.m_selected_gates[0] = to_gate;
    g_selection_relay.m_number_of_selected_gates = 1;
    g_selection_relay.m_focus_type = selection_relay::item_type::gate;
    g_selection_relay.m_focus_id = to_gate;
    g_selection_relay.m_subfocus = selection_relay::subfocus::left;
    g_selection_relay.m_subfocus_index = 0; // TODO

    g_selection_relay.relay_selection_changed(nullptr);

    // RELAYOUT
    if (update_necessary)
        m_context->update();

    // JUMP TO THE GATE
}

void graph_widget::handle_module_double_clicked(const u32 id)
{
    // CONNECT DIRECTLY TO HANDLE ???
    // MAYBE ADDITIONAL CODE NECESSARY HERE...
    handle_module_down_requested(id);
}

// TODO ADD SOUND OR ERROR MESSAGE TO FAILED NAVIGATION ATTEMPTS
void graph_widget::handle_navigation_left_request()
{
    switch (g_selection_relay.m_focus_type)
    {
    case selection_relay::item_type::none:
    {
        return;
    }
    case selection_relay::item_type::gate:
    {
        std::shared_ptr<gate> g = g_netlist->get_gate_by_id(g_selection_relay.m_focus_id);

        if (!g)
            return;

        if (g->get_input_pin_types().size())
        {
            if (g_selection_relay.m_subfocus == selection_relay::subfocus::left)
            {
                std::string pin_type = *std::next(g->get_input_pin_types().begin(), g_selection_relay.m_subfocus_index);
                std::shared_ptr<net> n = g->get_fan_in_net(pin_type);

                if (!n)
                    return;

                if (!n->get_src().gate)
                    return;

                bool contains_net = false;
                bool contains_gate = false;

                if (m_context->nets().contains(n->get_id()))
                    contains_net = true;

                if (m_context->gates().contains(n->get_src().get_gate()->get_id()))
                    contains_gate = true;

                bool update_necessary = false;

                if (!contains_net || !contains_gate)
                {
                    // THIS HAS TO BE DELEGATED TO THE CONTEXT
//                    if (m_context->scope())
//                    {
//                        // DRAW NON MEMBER NETS AND GATES AND SHADE THEM DIFFERENTLY ?
//                        std::shared_ptr<module> m = g_netlist->get_module_by_id(m_context->scope());

//                        if (!m)
//                            return; // INVALID SCOPE

//                        if (!m->contains_net(n, true))
//                            return; // NET OUT OF SCOPE

//                        if (!m->contains_gate(n->get_src().gate, true))
//                            return; // GATE OUT OF SCOPE
//                    }

                    QSet<u32> gates;
                    QSet<u32> nets;

                    if (!contains_net)
                        nets.insert(n->get_id());

                    if (!contains_gate)
                        gates.insert(n->get_src().get_gate()->get_id());

                    // ADD TO CONTEXT
                    m_context->add(QSet<u32>(), gates, nets); // EMPTY SET DEBUG CODE
                    update_necessary = true;
                }
                // SELECT IN RELAY
                g_selection_relay.m_selected_gates[0] = n->get_src().get_gate()->get_id();
                g_selection_relay.m_number_of_selected_gates = 1;
                g_selection_relay.m_focus_id = n->get_src().get_gate()->get_id();
                g_selection_relay.m_focus_type = selection_relay::item_type::gate;
                g_selection_relay.m_subfocus = selection_relay::subfocus::right;
                g_selection_relay.m_subfocus_index = 0; // TODO

                g_selection_relay.relay_selection_changed(nullptr);

                // RELAYOUT
                if (update_necessary)
                    m_context->update();
            }
            else
            {
                g_selection_relay.m_subfocus = selection_relay::subfocus::left;
                g_selection_relay.m_subfocus_index = 0;

                g_selection_relay.relay_subfocus_changed(nullptr);
            }
        }

        return;
    }
    case selection_relay::item_type::net:
    {
        std::shared_ptr<net> n = g_netlist->get_net_by_id(g_selection_relay.m_focus_id);

        if (!n)
            return;

        // TODO

        return;
    }
    case selection_relay::item_type::module:
    {
        return;
    }
    }
}

void graph_widget::handle_navigation_right_request()
{
    switch (g_selection_relay.m_focus_type)
    {
    case selection_relay::item_type::none:
    {
        return;
    }
    case selection_relay::item_type::gate:
    {
        std::shared_ptr<gate> g = g_netlist->get_gate_by_id(g_selection_relay.m_focus_id);

        if (!g)
            return;

        if (g_selection_relay.m_subfocus == selection_relay::subfocus::right)
        {
            m_navigation_widget->setup();
            m_navigation_widget->setFocus();
            m_overlay->show();
        }
        else
        {
            g_selection_relay.m_subfocus = selection_relay::subfocus::right;
            g_selection_relay.m_subfocus_index = 0;

            g_selection_relay.relay_subfocus_changed(nullptr);
        }

        return;
    }
    case selection_relay::item_type::net:
    {
        std::shared_ptr<net> n = g_netlist->get_net_by_id(g_selection_relay.m_focus_id);

        if (!n)
            return;

        // TODO

        return;
    }
    case selection_relay::item_type::module:
    {
        return;
    }
    }
}

void graph_widget::handle_navigation_up_request()
{
    if (g_selection_relay.m_focus_type == selection_relay::item_type::gate)
        if (m_context->gates().contains(g_selection_relay.m_focus_id))
            g_selection_relay.navigate_up();
}

void graph_widget::handle_navigation_down_request()
{
    if (g_selection_relay.m_focus_type == selection_relay::item_type::gate)
        if (m_context->gates().contains(g_selection_relay.m_focus_id))
            g_selection_relay.navigate_down();
}

void graph_widget::handle_module_up_request()
{
    // CHECK IF POSSIBLE
    // GO TO CONTEXT MANAGER
    // SET SCENE
    // DISPLAY OVERLAY IF NECESSARY
}

void graph_widget::handle_module_down_requested(const u32 id)
{
    // CHANGE CONTEXT
}

void graph_widget::debug_module_one()
{
    if (m_context)
    {
        // UNSUB FROM OLD CONTEXT
        //disconnect(m_context, &graph_context::updating_scene, this, &graph_widget::handle_updating_scene);
        disconnect(m_context, &graph_context::scene_available, this, &graph_widget::handle_scene_available);
    }

    // SUB TO NEW
    m_context = g_graph_context_manager.get_module_context(1);

    if (!m_context)
        return;

    //connect(m_context, &graph_context::updating_scene, this, &graph_widget::handle_updating_scene);
    connect(m_context, &graph_context::scene_available, this, &graph_widget::handle_scene_available);

    if (m_context->available())
        m_graphics_widget->view()->setScene(m_context->scene());
}

void graph_widget::debug_create_context()
{
    dynamic_context* context = g_graph_context_manager.add_dynamic_context("Debug", 1);

    QSet<u32> gates;
    QSet<u32> nets;

    for (u32 i = 0; i < g_selection_relay.m_number_of_selected_gates; ++i)
        gates.insert(g_selection_relay.m_selected_gates[i]);

    for (u32 i = 0; i < g_selection_relay.m_number_of_selected_nets; ++i)
        nets.insert(g_selection_relay.m_selected_nets[i]);

    context->add(QSet<u32>(), gates, nets); // EMPTY SET DEBUG CODE
    context->update();
}

void graph_widget::debug_change_context()
{
    bool ok;
    QString item = QInputDialog::getItem(this, "Debug", "Context:", g_graph_context_manager.dynamic_context_list(), 0, false, &ok);

    if (ok && !item.isEmpty())
    {
        // UNSUB FROM OLD CONTEXT
        //disconnect(m_context, &graph_context::updating_scene, this, &graph_widget::handle_updating_scene);
        disconnect(m_context, &graph_context::scene_available, this, &graph_widget::handle_scene_available);
        // SUB TO NEW
        m_context = g_graph_context_manager.get_dynamic_context(item);

        if (!m_context)
            return;

        //connect(m_context, &graph_context::updating_scene, this, &graph_widget::handle_updating_scene);
        connect(m_context, &graph_context::scene_available, this, &graph_widget::handle_scene_available);

        if (m_context->available())
            m_graphics_widget->view()->setScene(m_context->scene());
    }
}

void graph_widget::debug_update_context()
{
    if (m_context)
        m_context->update();
}

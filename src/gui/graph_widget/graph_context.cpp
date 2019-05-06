#include "graph_widget/graph_context.h"

#include "netlist/module.h"

#include "graph_widget/layouters/standard_graph_layouter.h"
#include "graph_widget/layouters/standard_graph_layouter_v3.h"
#include "graph_widget/shaders/module_shader.h"
#include "gui_globals.h"

#include <QtConcurrent>

graph_context::graph_context(const QString& name, const u32 scope, QObject* parent) : QObject(parent),
    m_name(name),
    m_scope(scope),
    m_watcher(new QFutureWatcher<void>(this)),
    m_layouter(new standard_graph_layouter_v3(this)),
    m_shader(new module_shader(this)),
    m_conform_to_grid(false),
    m_scene_available(true)
{
    connect(m_watcher, &QFutureWatcher<void>::finished, this, &graph_context::handle_layouter_finished);

    // DEBUG CODE
    Q_UNUSED(name)

    static int i = 1;
    m_name = "Context " + QString::number(i);
    ++i;

    // USE SEPARATE CLASSES FOR MODULE AND FREE CONTEXTS
    // SPLIT CONTEXT INTO LAYOUT, SHADING, ITEM TYPES ... AND CONTEXT TYPE
}

void graph_context::add(const QSet<u32>& gates, const QSet<u32>& nets)
{
    // SHOULD ID VALIDITY BE CHECKED HERE ?
    QSet<u32> new_gates = gates - m_gates;
    QSet<u32> new_nets = nets - m_nets;

    m_gates += new_gates;
    m_nets += new_nets;

    m_layouter->add(new_gates, new_nets);
    m_shader->added(new_gates, new_nets);
}

void graph_context::remove(const QSet<u32>& gates, const QSet<u32>& nets)
{
    // SHOULD ID VALIDITY BE CHECKED HERE ?
    QSet<u32> old_gates = gates & m_gates;
    QSet<u32> old_nets = nets & m_nets;

    m_gates -= old_gates;
    m_nets -= old_nets;

    m_layouter->remove(old_gates, old_nets);
    m_shader->removed(old_gates, old_nets);
}

void graph_context::add_module(const u32 id)
{
    std::shared_ptr<module> m = g_netlist->get_module_by_id(id);

    if (!m)
        return;

    QSet<u32> new_gates;
    QSet<u32> new_nets;

    for (std::shared_ptr<gate> g : m->get_gates())
    {
        if (!m_gates.contains(g->get_id()))
            new_gates.insert(g->get_id());
    }

    for (std::shared_ptr<net> n : m->get_nets())
    {
        if (!m_nets.contains(n->get_id()))
            new_nets.insert(n->get_id());
    }

    m_gates += new_gates;
    m_nets += new_nets;

    m_layouter->add(new_gates, new_nets);
    m_shader->added(new_gates, new_nets);
}

void graph_context::remove_module(const u32 id)
{
    Q_UNUSED(id)
    // TODO
}

QString graph_context::name() const
{
    return m_name;
}

u32 graph_context::scope() const
{
    return m_scope;
}

const QSet<u32>& graph_context::gates() const
{
    return m_gates;
}

const QSet<u32>& graph_context::nets() const
{
    return m_nets;
}

graph_layouter* graph_context::layouter()
{
    return m_layouter;
}

graph_shader* graph_context::shader()
{
    return m_shader;
}

bool graph_context::conform_to_grid() const
{
    return m_conform_to_grid;
}

bool graph_context::available() const
{
    return m_scene_available;
}

void graph_context::update()
{
    // TODO
    m_layouter->layout();
}

void graph_context::handle_netlist_event(netlist_event_handler::event ev, std::shared_ptr<netlist> netlist, u32 associated_data)
{
    Q_UNUSED(ev);
    Q_UNUSED(netlist);
    Q_UNUSED(associated_data);
}

void graph_context::handle_gate_event(gate_event_handler::event ev, std::shared_ptr<gate> gate, u32 associated_data)
{
    Q_UNUSED(ev);
    Q_UNUSED(gate);
    Q_UNUSED(associated_data);
}

void graph_context::handle_net_event(net_event_handler::event ev, std::shared_ptr<net> net, u32 associated_data)
{
    Q_UNUSED(ev);
    Q_UNUSED(net);
    Q_UNUSED(associated_data);
}

void graph_context::handle_module_event(module_event_handler::event ev, std::shared_ptr<module> module, u32 associated_data)
{
    Q_UNUSED(ev);
    Q_UNUSED(module);
    Q_UNUSED(associated_data);
}

void graph_context::handle_layouter_finished()
{
    // CALL SHADER

    m_scene_available = true;
    Q_EMIT scene_available();
}

void graph_context::update_scene()
{
    Q_EMIT updating_scene();
    m_watcher->setFuture(QtConcurrent::run(m_layouter, &graph_layouter::layout));
}

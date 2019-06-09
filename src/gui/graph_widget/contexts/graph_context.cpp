#include "gui/graph_widget/contexts/graph_context.h"

#include "netlist/module.h"

#include "gui/graph_widget/contexts/graph_context_subscriber.h"
#include "gui/graph_widget/graphics_scene.h"
#include "gui/graph_widget/layouters/orthogonal_graph_layouter.h"
#include "gui/graph_widget/layouters/standard_graph_layouter.h"
#include "gui/graph_widget/layouters/standard_graph_layouter_v3.h"
#include "gui/graph_widget/shaders/module_shader.h"
#include "gui/gui_globals.h"

#include <QtConcurrent>

static const bool lazy_update = false; // USE SETTINGS FOR THIS

graph_context::graph_context(QObject* parent) : QObject(parent),
    //m_layouter(new orthogonal_graph_layouter(this)),
    m_layouter(new standard_graph_layouter_v3(this)),
    m_shader(new module_shader(this)),
    m_conform_to_grid(false),
    m_watcher(new QFutureWatcher<void>(this)),
    m_scene_available(true)
{
    connect(m_watcher, &QFutureWatcher<void>::finished, this, &graph_context::handle_layouter_finished);
}

graph_context::~graph_context()
{
    for (graph_context_subscriber* subscriber : m_subscribers)
        subscriber->handle_context_deleted();

}

void graph_context::subscribe(graph_context_subscriber* const subscriber)
{
    if (!subscriber || m_subscribers.contains(subscriber))
        return;

    m_subscribers.append(subscriber);
}

void graph_context::unsubscribe(graph_context_subscriber* const subscriber)
{
    m_subscribers.removeOne(subscriber);
}

void graph_context::add(const QSet<u32>& modules, const QSet<u32>& gates, const QSet<u32>& nets)
{
    QSet<u32> new_modules = modules - m_modules;
    QSet<u32> new_gates = gates - m_gates;
    QSet<u32> new_nets = nets - m_nets;

    m_modules += new_modules;
    m_gates += new_gates;
    m_nets += new_nets;

    m_layouter->add(new_modules, new_gates, new_nets);
    m_shader->add(new_modules, new_gates, new_nets);
}

void graph_context::remove(const QSet<u32>& modules, const QSet<u32>& gates, const QSet<u32>& nets)
{
    QSet<u32> old_modules = modules & m_modules;
    QSet<u32> old_gates = gates & m_gates;
    QSet<u32> old_nets = nets & m_nets;

    m_modules -= old_modules;
    m_gates -= old_gates;
    m_nets -= old_nets;

    m_layouter->remove(old_modules, old_gates, old_nets);
    m_shader->remove(old_modules, old_gates, old_nets);
}

const QSet<u32>& graph_context::gates() const
{
    return m_gates;
}

const QSet<u32>& graph_context::modules() const
{
    return m_modules;
}

const QSet<u32>& graph_context::nets() const
{
    return m_nets;
}

graphics_scene* graph_context::scene()
{
    return m_layouter->scene();
}

//graph_layouter* graph_context::layouter()
//{
//    return m_layouter;
//}

//graph_shader* graph_context::shader()
//{
//    return m_shader;
//}

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
    //m_layouter->layout();
    update_scene();
}

void graph_context::handle_layouter_finished()
{
    // SHADER MIGHT HAS TO BE THREADED ASWELL
    m_shader->update();
    m_layouter->scene()->update_visuals(m_shader->get_shading());

    m_scene_available = true;
    Q_EMIT scene_available();
}

void graph_context::update_scene()
{
    Q_EMIT scene_unavailable();
    m_watcher->setFuture(QtConcurrent::run(m_layouter, &graph_layouter::layout));
}

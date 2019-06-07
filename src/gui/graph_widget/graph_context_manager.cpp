#include "gui/graph_widget/graph_context_manager.h"

#include "gui/graph_widget/contexts/dynamic_context.h"
#include "gui/graph_widget/contexts/module_context.h"
#include "gui/gui_globals.h"

static const int max_module_contexts = 10; // USE SETTINGS FOR THIS

graph_context_manager::graph_context_manager()
{

}

module_context* graph_context_manager::get_module_context(const u32 id)
{
    for (module_context* c : m_module_contexts)
        if (c->get_id() == id)
            return c;

    std::shared_ptr<module> m = g_netlist->get_module_by_id(id);
    if (!m)
        return nullptr;

    module_context* c = new module_context(m);
    m_module_contexts.append(c); // USE LRU
    return c;
}

dynamic_context* graph_context_manager::add_dynamic_context(const QString& name, const u32 scope)
{
    dynamic_context* context = new dynamic_context(name, scope);
    m_dynamic_contexts.append(context);
    return context;
}

dynamic_context* graph_context_manager::get_dynamic_context(const QString& name)
{
    for (dynamic_context* context : m_dynamic_contexts)
        if (context->name() == name)
            return context;

    return nullptr;
}

QStringList graph_context_manager::dynamic_context_list() const
{
    QStringList list;

    for (dynamic_context* context : m_dynamic_contexts)
        list.append(context->name());

    return list;
}

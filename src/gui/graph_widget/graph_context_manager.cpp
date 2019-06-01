#include "gui/graph_widget/graph_context_manager.h"

#include "gui/graph_widget/contexts/graph_context.h"

static const int max_contexts = 10; // USE SETTINGS FOR THIS

graph_context_manager* graph_context_manager::debug_instance()
{
    static graph_context_manager instance;
    return &instance;
}

graph_context* graph_context_manager::debug_add_context(const QString& name, const u32 scope)
{
    graph_context* context = new graph_context(name, scope);
    m_contexts.append(context);
    return context;
}

graph_context* graph_context_manager::debug_get_context(const QString& name)
{
    for (graph_context* context : m_contexts)
        if (context->name() == name)
            return context;

    return nullptr;
}

QStringList graph_context_manager::debug_context_list()
{
    QStringList list;

    for (graph_context* context : m_contexts)
        list.append(context->name());

    return list;
}

graph_context_manager::graph_context_manager()
{
    // DEBUG CODE
    // CREATE TEST CONTEXT
    graph_context* context = new graph_context("Top", 1);
    context->add_module(1);
    m_contexts.append(context);
    context->update();
}

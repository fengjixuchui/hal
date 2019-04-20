#ifndef GRAPH_CONTEXT_MANAGER_H
#define GRAPH_CONTEXT_MANAGER_H

#include "def.h"

#include "graph_widget/graph_context.h"

#include "QStringList"

class graph_context_manager
{
public:
    static graph_context_manager* debug_instance();

    graph_context* debug_add_context(const QString& name, const u32 scope);

    graph_context* debug_get_context(const QString& name);

    QStringList debug_context_list();

private:
    graph_context_manager();

    QVector<graph_context*> m_contexts;
};

#endif // GRAPH_CONTEXT_MANAGER_H

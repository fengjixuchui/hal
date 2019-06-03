#ifndef GRAPH_CONTEXT_MANAGER_H
#define GRAPH_CONTEXT_MANAGER_H

#include "def.h"

#include <QStringList>
#include <QVector>

class dynamic_context;
class module_context;

class graph_context_manager
{
public:
    static graph_context_manager* debug_instance();

    module_context* get_module_context(const u32 id);

    dynamic_context* add_dynamic_context(const QString& name, const u32 scope);

    dynamic_context* get_dynamic_context(const QString& name);

    QStringList dynamic_context_list();

private:
    graph_context_manager();

    QVector<module_context*> m_module_contexts;
    QVector<dynamic_context*> m_dynamic_contexts;
};

#endif // GRAPH_CONTEXT_MANAGER_H

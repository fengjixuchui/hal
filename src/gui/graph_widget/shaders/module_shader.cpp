#include "graph_widget/shaders/module_shader.h"

module_shader::module_shader(graph_context* context) : graph_shader(context)
{

}

void module_shader::added(const QSet<u32> gates, const QSet<u32> nets)
{
    Q_UNUSED(gates)
    Q_UNUSED(nets)
}

void module_shader::removed(const QSet<u32> gates, const QSet<u32> nets)
{
    Q_UNUSED(gates)
    Q_UNUSED(nets)
}

graph_shader::output module_shader::calculate() const
{
    output o;

    return o;
}

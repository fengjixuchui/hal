#ifndef GRAPH_SHADER_H
#define GRAPH_SHADER_H

#include "def.h"

#include "graph_widget/graphics_items/graphics_net.h"
#include "graph_widget/graphics_items/graphics_node.h"

#include <QColor>
#include <QMap>
#include <QSet>
#include <QVector>

class graph_context;

class graph_shader
{
public:
    struct output // TODO FIND A BETTER NAME
    {
        QMap<u32, graphics_node::visuals> node_visuals;
        QMap<u32, graphics_net::visuals> net_visuals;
    };

    graph_shader(graph_context* context);

    virtual void added(const QSet<u32> gates, const QSet<u32> nets) = 0;
    virtual void removed(const QSet<u32> gates, const QSet<u32> nets) = 0;

    virtual output calculate() const = 0;

private:
    const graph_context* const m_context;
};

#endif // GRAPH_SHADER_H

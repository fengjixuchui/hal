#ifndef MODULE_SHADER_H
#define MODULE_SHADER_H

#include "gui/graph_widget/shaders/graph_shader.h"

class module_shader : public graph_shader
{
public:
    module_shader(const graph_context* const context);

    virtual void added(const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;
    virtual void removed(const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;

    virtual void update() Q_DECL_OVERRIDE;
};

#endif // MODULE_SHADER_H

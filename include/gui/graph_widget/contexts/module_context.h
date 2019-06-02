#ifndef MODULE_CONTEXT_H
#define MODULE_CONTEXT_H

#include "netlist/module.h"

#include "gui/graph_widget/contexts/graph_context.h"

class module_context : public graph_context
{
public:
    module_context(const std::shared_ptr<module> m);
};

#endif // MODULE_CONTEXT_H

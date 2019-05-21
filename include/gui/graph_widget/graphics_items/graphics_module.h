#ifndef GRAPHICS_MODULE_H
#define GRAPHICS_MODULE_H

#include "graph_widget/graphics_items/graphics_node.h"

#include <memory>

class module;

class graphics_module : public graphics_node
{
    struct module_pin
    {
        QString name;
        u32 net_id;
    };

public:
    graphics_module(std::shared_ptr<module> m);

private:
    QVector<module_pin> m_input_pins;
    QVector<module_pin> m_output_pins;
};

#endif // GRAPHICS_MODULE_H

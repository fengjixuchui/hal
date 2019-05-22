#ifndef GRAPHICS_MODULE_H
#define GRAPHICS_MODULE_H

#include "gui/graph_widget/graphics_items/graphics_node.h"

#include <memory>

class module;

class graphics_module : public graphics_node
{
    struct module_pin
    {
        u32 net_id;
        QString pin_type;
    };

public:
    graphics_module(std::shared_ptr<module> m);

    virtual void set_visuals(const visuals& v) Q_DECL_OVERRIDE;

private:
    QVector<module_pin> m_input_pins;
    QVector<module_pin> m_output_pins;
};

#endif // GRAPHICS_MODULE_H

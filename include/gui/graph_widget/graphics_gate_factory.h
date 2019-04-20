#ifndef GRAPHICS_GATE_FACTORY_H
#define GRAPHICS_GATE_FACTORY_H

#include <QStringList>

#include <memory>

class gate;

class graphics_gate;

namespace graphics_gate_factory
{
QStringList graphics_gate_types();
graphics_gate* create_graphics_gate(std::shared_ptr<gate> g, int type);
}

#endif // GRAPHICS_GATE_FACTORY_H

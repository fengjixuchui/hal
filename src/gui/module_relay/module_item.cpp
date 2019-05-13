#include "module_relay/module_item.h"

module_item::module_item(const QString& name, const int id, module_item* parent_item) :
    m_parent_item(parent_item),
    m_name(name),
    m_id(id)
{
}

module_item::~module_item()
{
    for (const module_item* const& item : m_child_items)
        delete item;
}

void module_item::insert_child(int row, module_item* child)
{   
    m_child_items.insert(row, child);
}

void module_item::remove_child(module_item* child)
{
    m_child_items.removeOne(child);
}

void module_item::append_child(module_item* child)
{
    // PROBABLY OBSOLETE
    m_child_items.append(child);
}

void module_item::prepend_child(module_item* child)
{
    // PROBABLY OBSOLETE
    m_child_items.prepend(child);
}

module_item* module_item::child(int row)
{
    return m_child_items.value(row);
}

int module_item::childCount() const
{
    return m_child_items.count();
}

int module_item::row() const
{
    if (m_parent_item)
        return m_parent_item->m_child_items.indexOf(const_cast<module_item*>(this));
    else
        return 0;
}

QVariant module_item::data(int column) const
{
    // DEBUG CODE, TODO USE STYLED DELEGATES OR SOMETHING
    if (column != 0)
        return QVariant();
    return m_name;
}

module_item* module_item::parent_item()
{
    return m_parent_item;
}

QString module_item::name() const
{
    return m_name;
}

u32 module_item::id() const
{
    return m_id;
}

void module_item::set_name(QString& name)
{
    m_name = name;
}

void module_item::set_color(QColor& color)
{
    m_color = color;
}

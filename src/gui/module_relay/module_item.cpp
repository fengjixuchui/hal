#include "module_relay/module_item.h"

module_item::module_item(QString name, int id, module_item* parent_item) :
    m_parent_item(parent_item),
    m_name(name),
    m_id(id)
{
    if (parent_item)
        parent_item->append_child(this);
}

module_item::~module_item()
{
    for (module_item* item : m_child_items)
        delete item;

    // REMOVE ITEM FROM PARENT BEFORE DELETING MANUALLY ?
    if (m_parent_item)
        m_parent_item->remove_child(this);
}

void module_item::append_child(module_item* child)
{
    m_child_items.append(child);
}

void module_item::prepend_child(module_item* child)
{
    m_child_items.prepend(child);
}

void module_item::insert_child(int row, module_item* child)
{
    m_child_items.insert(row, child);
}

void module_item::remove_child(module_item* child)
{
    m_child_items.removeOne(child);
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

QString module_item::name()
{
    return m_name;
}

u32 module_item::id()
{
    return m_id;
}

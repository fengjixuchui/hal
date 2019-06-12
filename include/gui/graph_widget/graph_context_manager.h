#ifndef GRAPH_CONTEXT_MANAGER_H
#define GRAPH_CONTEXT_MANAGER_H

#include "def.h"

#include <QStringList>
#include <QVector>

class gate;
class module;
class net;

class dynamic_context;
class module_context;

class graph_context_manager
{
public:
    graph_context_manager();

    module_context* get_module_context(const u32 id);

    dynamic_context* add_dynamic_context(const QString& name, const u32 scope);
    dynamic_context* get_dynamic_context(const QString& name);
    QStringList dynamic_context_list() const; // PROBABLY DEBUG METHOD, WILL BE REPLACED BY DIFFERENT SELECTION INTERFACE

    //void handle_module_created(const std::shared_ptr<module> m) const; // PRECACHING ???
    void handle_module_removed(const std::shared_ptr<module> m) const;
    void handle_module_name_changed(const std::shared_ptr<module> m) const;
    void handle_module_parent_changed(const std::shared_ptr<module> m) const;
    void handle_module_submodule_added(const std::shared_ptr<module> m, const u32 added_module) const;
    void handle_module_submodule_removed(const std::shared_ptr<module> m, const u32 removed_module) const;
    void handle_module_gate_inserted(const std::shared_ptr<module> m, const u32 inserted_gate) const;
    void handle_module_gate_removed(const std::shared_ptr<module> m, const u32 removed_gate) const;
    void handle_module_net_inserted(const std::shared_ptr<module> m, const u32 inserted_net) const;
    void handle_module_net_removed(const std::shared_ptr<module> m, const u32 removed_net) const;

    //void handle_gate_created(const std::shared_ptr<gate> g) const;
    //void handle_gate_removed(const std::shared_ptr<gate> g) const;
    void handle_gate_name_changed(const std::shared_ptr<gate> g) const;

    //void handle_created(const std::shared_ptr<net> n) const;
    //void handle_removed(const std::shared_ptr<net> n) const;
    void handle_name_changed(const std::shared_ptr<net> n) const;
    void handle_src_changed(const std::shared_ptr<net> n) const;
    void handle_dst_added(const std::shared_ptr<net> n, const u32 dst_gate_id) const;
    void handle_dst_removed(const std::shared_ptr<net> n, const u32 dst_gate_id) const;

private:
    // TODO REPLACE WITH RAW ARRAYS ???
    QVector<module_context*> m_module_contexts;
    QVector<dynamic_context*> m_dynamic_contexts;
};

#endif // GRAPH_CONTEXT_MANAGER_H

#ifndef GRAPH_CONTEXT_H
#define GRAPH_CONTEXT_H

#include "def.h"

#include "gui/graph_widget/layouters/graph_layouter.h"
#include "gui/graph_widget/shaders/graph_shader.h"

#include <QFutureWatcher>
#include <QObject>
#include <QSet>

class graph_context : public QObject
{
    Q_OBJECT

public:
    explicit graph_context(QObject* parent = nullptr);

    void add(const QSet<u32>& gates, const QSet<u32>& nets);
    void remove(const QSet<u32>& gates, const QSet<u32>& nets);

    // NAME AMBIGUOUS, TODO RENAME
    void add_module(const u32 id);
    void remove_module(const u32 id);

    void debug_show_module(const u32 id);

    const QSet<u32>& modules() const;
    const QSet<u32>& gates() const;
    const QSet<u32>& nets() const;

    graph_layouter* layouter();
    graph_shader* shader();

    bool conform_to_grid() const;

    bool available() const; // TODO FIX BAD NAMING

    void update();

Q_SIGNALS:
    void updating_scene();
    void scene_available();
    // ABOUT TO BE DELETED SIGNAL ?

private Q_SLOTS:
    void handle_layouter_finished();

protected:
    // REPLACE WITH VECTORS ???
    QSet<u32> m_modules;
    QSet<u32> m_gates;
    QSet<u32> m_nets;

    graph_layouter* m_layouter;
    graph_shader* m_shader;

private:
    void update_scene();
    void apply_shading();

    bool m_conform_to_grid;

    QFutureWatcher<void>* m_watcher;
    bool m_scene_available;

    // GATE TYPE

    bool m_separate_gnd;
    bool m_separate_vcc;
};

#endif // GRAPH_CONTEXT_H

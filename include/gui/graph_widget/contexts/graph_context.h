#ifndef GRAPH_CONTEXT_H
#define GRAPH_CONTEXT_H

#include "def.h"

#include "gui/graph_widget/layouters/graph_layouter.h"
#include "gui/graph_widget/shaders/graph_shader.h"

#include <QFutureWatcher>
#include <QObject>
#include <QSet>

class graph_context_subscriber;

class graph_context : public QObject
{
    Q_OBJECT

public:
    explicit graph_context(QObject* parent = nullptr);
    ~graph_context();

    void subscribe(graph_context_subscriber* const subscriber);
    void unsubscribe(graph_context_subscriber* const subscriber);

    void add(const QSet<u32>& modules, const QSet<u32>& gates, const QSet<u32>& nets);
    void remove(const QSet<u32>& modules, const QSet<u32>& gates, const QSet<u32>& nets);

    const QSet<u32>& modules() const;
    const QSet<u32>& gates() const;
    const QSet<u32>& nets() const;

    graphics_scene* scene();

    // PROBABLY OBSOLETE
    //graph_layouter* layouter();
    //graph_shader* shader();

    // MOVE THIS SOMEWHERE ELSE; MAYBE ADD DEDICATED LAYOUTER SETTINGS CLASS ???
    bool conform_to_grid() const;

    bool available() const; // TODO FIX BAD NAMING
    bool update_in_progress() const;

//Q_SIGNALS:
//    void scene_available();
//    void scene_unavailable();
//    void about_to_be_deleted();

private Q_SLOTS:
    void handle_layouter_finished();

protected:
    QSet<u32> m_modules;
    QSet<u32> m_gates;
    QSet<u32> m_nets;

    graph_layouter* m_layouter;
    graph_shader* m_shader; // MOVE SHADER TO VIEW ? USE BASE SHADER AND ADDITIONAL SHADERS ?

private:
    void update_if_necessary();
    void update();
    void conditional_update();
    void apply_changes();
    void update_scene();

    QSet<u32> m_added_modules;
    QSet<u32> m_added_gates;
    QSet<u32> m_added_nets;

    QSet<u32> m_removed_modules;
    QSet<u32> m_removed_gates;
    QSet<u32> m_removed_nets;

    QList<graph_context_subscriber*> m_subscribers;

    bool m_unhandled_changes;
    bool m_scene_update_required;

    bool m_conform_to_grid;

    QFutureWatcher<void>* m_watcher;
    bool m_scene_available;
    bool m_update_in_progress;

    // GATE TYPE

    bool m_separate_gnd;
    bool m_separate_vcc;
};

#endif // GRAPH_CONTEXT_H

#ifndef GRAPH_SCENE_H
#define GRAPH_SCENE_H

#include "def.h"

#include <QGraphicsScene>
#include <QPair>
#include <QVector>

class gate_navigation_popup;
class graphics_gate;
class graphics_item;
class graphics_net;

class graphics_scene : public QGraphicsScene
{
    Q_OBJECT

    enum class grid_type
    {
        lines,
        dots
    };

    // TODO USE THESE TO IMPROVE READABILITY
//    struct gate_data
//    {
//        u32 id;
//        graphics_gate* item;
//    };

//    struct net_data
//    {
//        u32 id;
//        graphics_net* item;
//    };

public:
    static void set_lod(qreal lod);

    // HACK
    // ALTERNATIVELY ADRESS ADAPTER MEMBERS DIRECTLY
    static void set_grid_base_line_color(const QColor& color);
    static void set_grid_cluster_line_color(const QColor& color);
    static void set_grid_base_dot_color(const QColor& color);
    static void set_grid_cluster_dot_color(const QColor& color);
    // END OF HACK

    graphics_scene(QObject* parent = nullptr);

    void addItem(graphics_item* item);
    void removeItem(graphics_item* item);

    const graphics_gate* get_gate_item(const u32 id) const;

    void update_utility_items();

    bool grid_enabled() const;
    void set_grid_enabled(const bool value);

    // EXPERIMENTAL METHODS
    void connect_all();
    void disconnect_all();

    void delete_all_items();

public Q_SLOTS:
    void handle_intern_selection_changed();
    void handle_extern_selection_changed(void* sender);
    void handle_extern_subfocus_changed(void* sender);
//    void handle_focus_item_changed(QGraphicsItem* new_focus_item, QGraphicsItem* old_focus_item, Qt::FocusReason reason);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;

private:
    static qreal s_lod;

    static const qreal s_grid_fade_start;
    static const qreal s_grid_fade_end;

    static grid_type s_grid_type;
    static bool s_grid_clusters_enabled;

    static QColor s_grid_base_line_color;
    static QColor s_grid_cluster_line_color;

    static QColor s_grid_base_dot_color;
    static QColor s_grid_cluster_dot_color;

    using QGraphicsScene::addItem;
    using QGraphicsScene::removeItem;

    void drawBackground(QPainter* painter, const QRectF& rect) Q_DECL_OVERRIDE;

    bool m_grid_enabled;

    QVector<QPair<u32, graphics_gate*>> m_gate_vector;
    QVector<QPair<u32, graphics_net*>> m_net_vector;

    gate_navigation_popup* m_left_gate_navigation_popup;
    gate_navigation_popup* m_right_gate_navigation_popup;
};

#endif // GRAPH_SCENE_H

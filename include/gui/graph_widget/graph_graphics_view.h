//  MIT License
//
//  Copyright (c) 2019 Ruhr-University Bochum, Germany, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2019 Marc Fyrbiak, Sebastian Wallat, Max Hoffmann ("ORIGINAL AUTHORS"). All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef GRAPH_GRAPHICS_VIEW_H
#define GRAPH_GRAPHICS_VIEW_H

#include "def.h"

#include <QGraphicsView>

class graphics_item;

namespace graph_widget_constants
{
enum class grid_type;
}

class graph_graphics_view : public QGraphicsView
{
    Q_OBJECT

public:
    graph_graphics_view(QWidget* parent = nullptr);

Q_SIGNALS:
    void module_double_clicked(u32 id);
    void zoomed_in(int value);
    void zoomed_out(int value);

private Q_SLOTS:
    void conditional_update();
    void handle_change_color_action();    //TODO REPLACE WITH RELAY
    void handle_cone_view_action();       //DEBUG CODE, TODO REPLACE
    void adjust_min_scale();

private:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void drawForeground(QPainter* painter, const QRectF& rect) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

    void show_context_menu(const QPoint& pos);

    void update_matrix(const int delta);

    void toggle_antialiasing();

    //TODO REPLACE WITH RELAY
    graphics_item* m_item;

    bool m_minimap_enabled;

    bool m_antialiasing_enabled;
    bool m_cosmetic_nets_enabled;

    bool m_grid_enabled;
    bool m_grid_clusters_enabled;
    graph_widget_constants::grid_type m_grid_type;

    QPoint m_move_position;
    QPoint m_zoom_position;
    QPointF m_zoom_scene_position;
    qreal m_min_scale;
};

#endif // GRAPH_GRAPHICS_VIEW_H

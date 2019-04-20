//  MIT License
//
//  Copyright (c) 2019 Marc Fyrbiak
//  Copyright (c) 2019 Sebastian Wallat
//  Copyright (c) 2019 Max Hoffmann
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

#include <QGraphicsView>

class graphics_item;

class graph_graphics_view : public QGraphicsView
{
    Q_OBJECT

public:
    graph_graphics_view(QWidget* parent = nullptr);

Q_SIGNALS:
    void zoomed_in(int value);
    void zoomed_out(int value);

private Q_SLOTS:
    void conditional_update();
    void handle_change_color_action();    //TODO REPLACE WITH RELAY
    void handle_cone_view_action();       //DEBUG CODE, TODO REPLACE

private:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    //    void drawForeground(QPainter* painter, const QRectF& rect) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

    void show_context_menu(const QPoint& pos);

    //TODO REPLACE WITH RELAY
    graphics_item* m_item;
};

#endif // GRAPH_GRAPHICS_VIEW_H

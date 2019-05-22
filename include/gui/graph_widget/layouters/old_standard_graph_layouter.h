/*
 * MIT License
 *
 * Copyright (c) 2019 Chair for Embedded Security, Ruhr-University Bochum
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
#ifndef OLD_STANDARD_GRAPH_LAYOUTER_H
#define OLD_STANDARD_GRAPH_LAYOUTER_H

#include "graph_widget/graphics_items/graphics_gate.h"
#include "graph_widget/layouters/graph_layouter.h"
#include "netlist/gate.h"
#include "netlist/net.h"
#include "netlist/netlist.h"

#include "graph_widget/graphics_items/graphics_gate.h"
#include "graph_widget/layouters/graph_layouter.h"

#include <QGraphicsScene>
#include <QMap>
#include <QObject>
#include <QTime>

#include <list>
#include <vector>

class graphics_gate;
class standard_graphics_net;

class old_standard_graph_layouter : public graph_layouter
{
    Q_OBJECT

public:
    old_standard_graph_layouter(graph_context* context, QObject* parent = nullptr);

    virtual void layout() Q_DECL_OVERRIDE;
    virtual void reset() Q_DECL_OVERRIDE;

    virtual void add(const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;
    virtual void remove(const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;

    virtual void expand(const u32 from_gate, const u32 via_net, const u32 to_gate) Q_DECL_OVERRIDE;

    virtual const QString name() const Q_DECL_OVERRIDE;
    virtual const QString description() const Q_DECL_OVERRIDE;

private:
    static int s_max_level_size;

    QMap<std::shared_ptr<gate>, graphics_gate*> m_gate_map;
    QMap<std::shared_ptr<net>, graphics_net*> m_net_map;

    qreal m_max_gate_width;
    qreal m_max_gate_height;
    //    qreal m_min_gate_width = 0;
    //    qreal m_min_gate_height = 0;

    QMap<std::shared_ptr<gate>, int> m_gate_level;
    int m_highest_level;

    std::list<std::shared_ptr<net>> m_separated_nets;
    std::list<std::list<graphics_gate*>> m_gate_layout;

    int m_channel_width     = 50;    //temp
    int m_sub_channel_count = 10;    //temp

    void compute_gate_levels();
    void place_gates();
    void draw_global_nets();
    void draw_separated_nets();

    void old_compute_gate_levels();
    void compress_gate_levels();
    void spin_gate_levels();
    void old_place_gates();
    void draw_standard_nets();
    void compute_channel_width();

    void calculate_net(std::shared_ptr<net> n, int sub_channel);    //temp
};

#endif // OLD_STANDARD_GRAPH_LAYOUTER_H
*/

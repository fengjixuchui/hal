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
#ifndef STANDARD_CONE_LAYOUTER_H
#define STANDARD_CONE_LAYOUTER_H

#include "def.h"

#include "graph_widget/layouters/graph_layouter.h"
#include "graph_widget/graphics_items/graphics_gate.h"

#include <QSet>
#include <QVector>

class standard_graph_layouter : public graph_layouter
{
    Q_OBJECT

    struct gate_level
    {
        u32 gate;
        int level;
    };

    struct gate_box
    {
        u32 id;
        graphics_gate* item;

        int x;
        int y;

        qreal input_padding;
        qreal output_padding;
    };

    // ROADS AND JUNCTIONS FILLED LEFT TO RIGHT, TOP TO BOTTOM
    struct road
    {
        road(const int x_coordinate, const int y_coordinate) :
            x(x_coordinate),
            y(y_coordinate),
            lanes(0)
        {}

        int x;
        int y;

        unsigned int lanes = 0;

        qreal vertical_width() const;
        qreal horizontal_height() const;
    };

    struct junction
    {
        junction(const int x_coordinate, const int y_coordinate) :
            x(x_coordinate),
            y(y_coordinate),
            h_lanes(0),
            v_lanes(0),
            left_lane_changes(0),
            right_lane_changes(0),
            top_lane_changes(0),
            bottom_lane_changes(0)
        {}

        int x;
        int y;

        unsigned int h_lanes = 0;
        unsigned int v_lanes = 0;

        unsigned int left_lane_changes = 0;
        unsigned int right_lane_changes = 0;
        unsigned int top_lane_changes = 0;
        unsigned int bottom_lane_changes = 0;

        qreal width() const;
        qreal height() const;
    };

    struct used_paths
    {
        QSet<road*> h_roads;
        QSet<road*> v_roads;

        QSet<junction*> h_junctions;
        QSet<junction*> v_junctions;

        QSet<junction*> left_junctions;
        QSet<junction*> right_junctions;
        QSet<junction*> top_junctions;
        QSet<junction*> bottom_junctions;
    };

public:
    standard_graph_layouter(graph_context* context, QObject* parent = nullptr);

    void layout() Q_DECL_OVERRIDE;
    void reset() Q_DECL_OVERRIDE;

    virtual void add(const QSet<u32> modules, const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;
    virtual void remove(const QSet<u32> modules, const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;

    virtual const QString name() const Q_DECL_OVERRIDE;
    virtual const QString description() const Q_DECL_OVERRIDE;

//    bool contains_gate(const u32 gate) const Q_DECL_OVERRIDE;
//    bool contains_net(const u32 net) const Q_DECL_OVERRIDE;

//public Q_SLOTS:
    void expand(const u32 from_gate, const u32 via_net, const u32 to_gate) Q_DECL_OVERRIDE;

//    void expand_to_dst(const u32 net, const u32 gate) Q_DECL_OVERRIDE;
//    void expand_to_src(const u32 gate, const u32 net) Q_DECL_OVERRIDE;

private:
    void create_boxes();
    void calculate_nets();
    void find_max_box_dimensions();
    void find_max_channel_lanes();
    void calculate_max_junction_spacing();
    void calculate_channel_spacing();
    void calculate_max_channel_dimensions();
    void calculate_gate_offsets();
    void place_gates();
    void update_scene_rect();
    void reset_roads_and_junctions();
    void draw_nets();

    void clear_net_layout_data();

    gate_box make_box(const u32 gate, const int x, const int y) const;

    void add_gate(const u32 gate_id, const int level);

    // USE POINTERS ???
    bool box_exists(const int x, const int y) const;
    // RENAME THESE ???
    bool h_road_jump_possible(const int x, const int y1, const int y2) const;
    bool h_road_jump_possible(const road* const r1, const road* const r2) const;

    bool v_road_jump_possible(const int x1, const int x2, const int y) const;
    bool v_road_jump_possible(const road* const r1, const road* const r2) const;

    road* get_h_road(const int x, const int y);
    road* get_v_road(const int x, const int y);
    junction* get_junction(const int x, const int y);

    qreal h_road_height(const unsigned int lanes) const;
    qreal v_road_width(const unsigned int lanes) const;

    qreal scene_y_for_h_channel_lane(const int y, const unsigned int lane) const;
    qreal scene_x_for_v_channel_lane(const int x, const unsigned int lane) const;

    qreal left_lane_change_x_for(const int channel_x, unsigned int lane_change) const;
    qreal left_lane_change_x_for(const junction* const j) const;

    qreal right_lane_change_x_for(const int channel_x, unsigned int lane_change) const;
    qreal right_lane_change_x_for(const junction* const j) const;

    qreal top_lane_change_y_for(const int channel_y, unsigned int lane_change) const;
    qreal top_lane_change_y_for(const junction* const j) const;

    qreal bottom_lane_change_y_for(const int channel_y, unsigned int lane_change) const;
    qreal bottom_lane_change_y_for(const junction* const j) const;

    // TEST
    qreal scene_x_for_close_left_lane_change(const int channel_x, unsigned int lane_change) const;
    qreal scene_x_for_far_left_lane_change(const int channel_x, unsigned int lane_change) const;

    qreal scene_x_for_close_right_lane_change(const int channel_x, unsigned int lane_change) const;
    qreal scene_x_for_far_right_lane_change(const int channel_x, unsigned int lane_change) const;

    qreal scene_y_for_close_top_lane_change(const int channel_y, unsigned int lane_change) const;
    qreal scene_y_for_far_top_lane_change(const int channel_y, unsigned int lane_change) const;

    qreal scene_y_for_close_bottom_lane_change(const int channel_y, unsigned int lane_change) const;
    qreal scene_y_for_far_bottom_lane_change(const int channel_y, unsigned int lane_change) const;

    template<typename T1, typename T2>
    void store_max(QMap<T1, T2>& map, T1 key, T2 value);

    void commit_used_paths(const used_paths& used);

    QVector<u32> m_gates;
    QVector<u32> m_nets;

    QMap<u32, int> m_gate_levels;

    // COMBINE VECTORS ???
    QVector<u32> m_zero_gates;

    QVector<QVector<u32>> m_positive_gates;
    QVector<QVector<u32>> m_negative_gates;

    QVector<gate_box> m_boxes;

    QVector<road*> m_h_roads;
    QVector<road*> m_v_roads;
    QVector<junction*> m_junctions;

    QMap<int, qreal> m_max_gate_width_for_x;
    QMap<int, qreal> m_max_gate_height_for_y;

    QMap<int, qreal> m_max_v_channel_width_for_x;
    QMap<int, qreal> m_max_h_channel_height_for_y;

    QMap<int, qreal> m_max_v_channel_left_spacing_for_x;
    QMap<int, qreal> m_max_v_channel_right_spacing_for_x;
    QMap<int, qreal> m_max_h_channel_top_spacing_for_y;
    QMap<int, qreal> m_max_h_channel_bottom_spacing_for_y;

    QMap<int, qreal> m_gate_offset_for_x;
    QMap<int, qreal> m_gate_offset_for_y;

    QMap<int, qreal> m_max_left_junction_spacing_for_x;
    QMap<int, qreal> m_max_right_junction_spacing_for_x;

    QMap<int, qreal> m_max_top_junction_spacing_for_y;
    QMap<int, qreal> m_max_bottom_junction_spacing_for_y;

    QMap<int, unsigned int> m_max_v_channel_lanes_for_x;
    QMap<int, unsigned int> m_max_h_channel_lanes_for_y;

    QMap<int, qreal> m_max_left_io_padding_for_channel_x;
    QMap<int, qreal> m_max_right_io_padding_for_channel_x;
};

#endif // STANDARD_CONE_LAYOUTER_H

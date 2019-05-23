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

#ifndef PARALLEL_GRAPH_LAYOUTER_H
#define PARALLEL_GRAPH_LAYOUTER_H

#include "graph_widget/layouters/graph_layouter.h"

class orthogonal_graph_layouter : public graph_layouter
{
    Q_OBJECT

public:
    orthogonal_graph_layouter(graph_context* context, QObject* parent = nullptr);

    virtual void layout() Q_DECL_OVERRIDE;
    virtual void reset() Q_DECL_OVERRIDE;

    virtual void add(const QSet<u32> modules, const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;
    virtual void remove(const QSet<u32> modules, const QSet<u32> gates, const QSet<u32> nets) Q_DECL_OVERRIDE;

    virtual void expand(const u32 from_gate, const u32 via_net, const u32 to_gate) Q_DECL_OVERRIDE;

    virtual const QString name() const Q_DECL_OVERRIDE;
    virtual const QString description() const Q_DECL_OVERRIDE;
};

#endif // PARALLEL_GRAPH_LAYOUTER_H

#include "graph_widget/old_graph_widget.h"

#include "graph_widget/graph_graphics_view.h"
#include "graph_widget/graph_layouter_manager.h"
#include "graph_widget/graph_layouter_selection_widget.h"
#include "graph_widget/graphics_scene.h"
#include "gui_globals.h"
#include "gui_utility.h"
#include "toolbar/toolbar.h"

#include <QGraphicsScene>
#include <qmath.h>
#include <QStackedWidget>
#include <QToolButton>

#include <QDebug>

old_graph_widget::old_graph_widget(QWidget* parent) : content_widget("Graph", parent),
    m_stacked_widget(new QStackedWidget()),
    m_selection_widget(new graph_layouter_selection_widget(this, nullptr)),
    m_view_container(new QWidget()),
    m_container_layout(new QHBoxLayout()),
    m_view(new graph_graphics_view(this)),
    m_zoom_slider(new QSlider()),
    m_layouter("")
{
    connect(m_view, &graph_graphics_view::zoomed_in, this, &old_graph_widget::zoom_in);
    connect(m_view, &graph_graphics_view::zoomed_out, this, &old_graph_widget::zoom_out);
    connect(m_zoom_slider, &QSlider::valueChanged, this, &old_graph_widget::update_matrix);

    m_view->setFrameStyle(QFrame::NoFrame);
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->setRenderHint(QPainter::Antialiasing, false);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);

    m_container_layout->setContentsMargins(0, 0, 0, 0);

    m_zoom_slider->setMinimum(0);
    m_zoom_slider->setMaximum(1000);

    update_matrix();

    m_content_layout->addWidget(m_stacked_widget);
    m_stacked_widget->addWidget(m_selection_widget);
    m_view_container->setLayout(m_container_layout);
    m_container_layout->addWidget(m_view);
    m_container_layout->addWidget(m_zoom_slider);
    m_stacked_widget->addWidget(m_view_container);
}

void old_graph_widget::setup_toolbar(toolbar* toolbar)
{
    QToolButton* layout_button = new QToolButton();
    layout_button->setText("Layout");

    QToolButton* antialias_button = new QToolButton();
    antialias_button->setText("Antialiasing");
    antialias_button->setCheckable(true);
    antialias_button->setChecked(m_view->renderHints() & QPainter::Antialiasing);

    connect(layout_button, &QToolButton::clicked, this, &old_graph_widget::show_layout_selection);
    connect(antialias_button, &QToolButton::toggled, this, &old_graph_widget::toggle_antialiasing);

    toolbar->addSeparator();
    toolbar->addWidget(layout_button);
    toolbar->addSeparator();
    toolbar->addWidget(antialias_button);
    toolbar->addSeparator();
}

void old_graph_widget::subscribe(QString layouter)
{
    if (m_layouter == layouter)
        return;

    graphics_scene* scene = graph_layouter_manager::get_instance().subscribe(layouter);

    if (scene)
        m_view->setScene(scene);

    if (m_layouter != "")
        graph_layouter_manager::get_instance().unsubscribe(m_layouter);

    m_layouter = layouter;
}

graph_graphics_view* old_graph_widget::view() const
{
    return m_view;
}

QString old_graph_widget::get_layouter()
{
    return m_layouter;
}

void old_graph_widget::show_view()
{
    m_stacked_widget->setCurrentWidget(m_view_container);
}

void old_graph_widget::update_matrix()
{
    qreal scale = qPow(qreal(2), (m_zoom_slider->value() -500) / qreal(100));

    QMatrix matrix;
    matrix.scale(scale, scale);
    m_view->setMatrix(matrix);
}

void old_graph_widget::show_layout_selection(bool checked)
{
    Q_UNUSED(checked)

    m_stacked_widget->setCurrentWidget(m_selection_widget);

    //TODO CLEAR TOOLBAR
}

void old_graph_widget::toggle_antialiasing()
{
    m_view->setRenderHint(QPainter::Antialiasing, !(m_view->renderHints() & QPainter::Antialiasing));
}

void old_graph_widget::zoom_in(int level)
{
    m_zoom_slider->setValue(m_zoom_slider->value() + level);
}

void old_graph_widget::zoom_out(int level)
{
    m_zoom_slider->setValue(m_zoom_slider->value() - level);
}

void old_graph_widget::keyPressEvent(QKeyEvent* event)
{
    // UNCERTAIN WHERE TO HANDLE THIS STUFF, WIDGET OR VIEW ?
    // GOING WITH WIDGET FOR NOW, MIGHT CHANGE LATER
    switch (event->key())
    {
    case Qt::Key_Up:
    {
        qDebug() << "Up pressed";
        g_selection_relay.navigate_up();
        break;
    }
    case Qt::Key_Down:
    {
        qDebug() << "Down pressed";
        g_selection_relay.navigate_down();
        break;
    }
    case Qt::Key_Left:
    {
        qDebug() << "Left pressed";
        g_selection_relay.navigate_left();
        break;
    }
    case Qt::Key_Right:
    {
        qDebug() << "Right pressed";
        g_selection_relay.navigate_right();
        break;
    }
    default: break;
    }
}

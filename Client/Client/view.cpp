/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include "callout.h"
#include <QtGui/QMouseEvent>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts>

View::View(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      chart(0),
      m_tooltip(0)
{
    setFixedSize(600, 300);
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContentsMargins(0, 0, 0, 0);
    setFrameStyle(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    chart = new QChart;
    chart->setTitle("App Usage");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setDropShadowEnabled(true);
    chart->setContentsMargins(0, 0, 0, 0);
    chart->setMinimumSize(600, 300);

    QHorizontalStackedBarSeries *series = new QHorizontalStackedBarSeries();
    series->setLabelsVisible(true); //make sure the bar set value is visiable
    for(int i = 0; i < 5; ++i) {
        QBarSet *barSet = new QBarSet("Bar " + QString::number(i)); //name the bar set
        barSet->append(i + 1); //append value to bar set
        series->append(barSet);

        connect(barSet, &QBarSet::hovered, this, &View::getName);
    }

    chart->addSeries(series);

    chart->createDefaultAxes();

    scene()->addItem(chart);
}

void View::getName(bool status, int)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(chart);

    if (status) {
        QBarSet *barSender = qobject_cast<QBarSet *>(sender());
        m_tooltip->setText(QString(barSender->label()));

        QPointF pf(6.6, 0.15);

        m_tooltip->setAnchor(pf);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    }
    else {
        m_tooltip->hide();
    }
}

#include "callout.h"
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>
#include <QDebug>

Callout::Callout(QChart *chart):
    QGraphicsItem(chart),
    m_chart(chart)
{
}

QRectF Callout::boundingRect() const
{
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect;
    rect.setLeft(qMin(m_rect.left(), anchor.x()));
    rect.setRight(qMax(m_rect.right(), anchor.x()));
    rect.setTop(qMin(m_rect.top(), anchor.y()));
    rect.setBottom(qMax(m_rect.bottom(), anchor.y()));
    return rect;
}

void Callout::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPainterPath path;
    path.addRoundedRect(m_rect, 5, 5); //set the rectangle round corners
    painter->setBrush(QColor(255, 192, 203));
    painter->drawPath(path);
    painter->drawText(m_textRect, m_text);

    qDebug() << m_rect;
}

void Callout::setText(const QString &text)
{
    m_text = text;
    QFontMetrics metrics(m_font);
    m_textRect = metrics.boundingRect(QRect(30, 0, 15, 15), Qt::AlignCenter, m_text); //draw a 15 * 15 area and the text is align in the central area
    m_textRect.translate(5, 5);
    prepareGeometryChange();
    m_rect = m_textRect.adjusted(-5, -5, 5, 5); //this line is kind of like to make sure that the text in the rectangle has margins
}

void Callout::setAnchor(QPointF point)
{
    m_anchor = point;
}

//this function is used to adjust callout position
void Callout::setPosition(QPoint p)
{
    custom_p = p;
}

void Callout::updateGeometry()
{
    prepareGeometryChange();
    setPos(m_chart->mapToPosition(m_anchor) + custom_p);
}

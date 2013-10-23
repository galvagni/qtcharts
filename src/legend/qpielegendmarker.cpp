/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Enterprise Charts Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qpielegendmarker.h"
#include "qpielegendmarker_p.h"
#include <QPieSeries>
#include <QPieSlice>

QTCOMMERCIALCHART_BEGIN_NAMESPACE

/*!
    \class QPieLegendMarker
    \inmodule Qt Charts
    \brief LegendMarker object.
    \mainclass

    QPieLegendMarker is related to QPieSeries. With QPieSeries, each slice of pie is related to one marker in QLegend.

    \sa QLegend, QPieSeries, QPieSlice
*/

/*!
  \fn virtual LegendMarkerType QPieLegendMarker::type()
  Returns QLegendMarker::LegendMarkerTypePie
*/

/*!
  \internal
*/
QPieLegendMarker::QPieLegendMarker(QPieSeries *series, QPieSlice *slice, QLegend *legend, QObject *parent) :
    QLegendMarker(*new QPieLegendMarkerPrivate(this,series,slice,legend), parent)
{
    d_ptr->updated();
}

/*!
  Destructor
*/
QPieLegendMarker::~QPieLegendMarker()
{
}

/*!
    \internal
*/
QPieLegendMarker::QPieLegendMarker(QPieLegendMarkerPrivate &d, QObject *parent) :
    QLegendMarker(d, parent)
{
}

/*!
  Returns the related series of marker.
*/
QPieSeries* QPieLegendMarker::series()
{
    Q_D(QPieLegendMarker);
    return d->m_series;
}

/*!
  Returns the related slice of marker.
*/
QPieSlice* QPieLegendMarker::slice()
{
    Q_D(QPieLegendMarker);
    return d->m_slice;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPieLegendMarkerPrivate::QPieLegendMarkerPrivate(QPieLegendMarker *q, QPieSeries *series, QPieSlice *slice, QLegend *legend) :
    QLegendMarkerPrivate(q,legend),
    q_ptr(q),
    m_series(series),
    m_slice(slice)
{
    QObject::connect(m_slice, SIGNAL(labelChanged()), this, SLOT(updated()));
    QObject::connect(m_slice, SIGNAL(brushChanged()), this, SLOT(updated()));
    QObject::connect(m_slice, SIGNAL(penChanged()), this, SLOT(updated()));
}

QPieLegendMarkerPrivate::~QPieLegendMarkerPrivate()
{
}

QPieSeries* QPieLegendMarkerPrivate::series()
{
    return m_series;
}

QObject* QPieLegendMarkerPrivate::relatedObject()
{
    return m_slice;
}

void QPieLegendMarkerPrivate::updated()
{
    bool labelChanged = false;
    bool brushChanged = false;
    bool penChanged = false;

    if (!m_customPen && (m_item->pen() != m_slice->pen())) {
        m_item->setPen(m_slice->pen());
        penChanged = true;
    }
    if (!m_customBrush && (m_item->brush() != m_slice->brush())) {
        m_item->setBrush(m_slice->brush());
        brushChanged = true;
    }
    if (!m_customLabel && (m_item->label() != m_slice->label())) {
        m_item->setLabel(m_slice->label());
        labelChanged = true;
    }
    invalidateLegend();

    if (labelChanged)
        emit q_ptr->labelChanged();
    if (brushChanged)
        emit q_ptr->brushChanged();
    if (penChanged)
        emit q_ptr->penChanged();
}

#include "moc_qpielegendmarker.cpp"
#include "moc_qpielegendmarker_p.cpp"

QTCOMMERCIALCHART_END_NAMESPACE

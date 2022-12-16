/******************************************************************************
 * Copyright (C) 2022 by Gschwind, Weber, Schoch, Niederberger                *
 *                                                                            *
 * This file is part of linescaleGUI.                                         *
 *                                                                            *
 * LinescaleGUI is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * LinescaleGUI is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with linescaleGUI. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/
/**
 * @file plotWidget.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 */

#include "plotWidget.h"
#include <QDebug>

Plot::Plot(QWidget* parent) : QWidget(parent) {
    updateTimer = new QTimer(this);
    updateTimer->setInterval(16);
    updateTimer->setTimerType(Qt::TimerType::PreciseTimer);
    connect(updateTimer, &QTimer::timeout, this, &Plot::updatePlot);

    disableReplotTimer = new QTimer(this);
    disableReplotTimer->setInterval(1000);
    disableReplotTimer->setTimerType(Qt::TimerType::VeryCoarseTimer);
    connect(disableReplotTimer, &QTimer::timeout, this, &Plot::disableUpdating);

    customPlot = new QCustomPlot(parent);
    // customPlot->setOpenGl(true); @todo Fix HiDPI bug in QCustomPlot library.
    customPlot->setNoAntialiasingOnDrag(true);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->xAxis->setLabel("");
    customPlot->yAxis->setLabel("");
    customPlot->legend->setVisible(false);

    customPlot->addGraph();
    customPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);

    QPen graphPen;
    graphPen.setColor(QColor(78, 50, 168, 255));
    graphPen.setWidthF(1.5);
    customPlot->graph()->setPen(graphPen);
    customPlot->replot();
    customPlot->rescaleAxes();

    // connect slot that ties some axis selections together (especially opposite axes):
    connect(customPlot, &QCustomPlot::selectionChangedByUser, this, &Plot::selectionChanged);
    // connect slots that takes care that when an axis is selected, only that direction can be
    // dragged and zoomed:
    connect(customPlot, &QCustomPlot::mousePress, this, &Plot::mousePress);
    connect(customPlot, &QCustomPlot::mouseWheel, this, &Plot::mouseWheel);
    // @todo Allow to zoom with panning (touchscreen).

    // @todo Maybe implement custom range dialog.
    // connect(customPlot, &QCustomPlot::axisDoubleClick, this, &Plot::axisLabelDoubleClick);

    // connect slot that shows a message in the status bar when a graph is clicked:
    // connect(customPlot, &QCustomPlot::plottableClick, this, &Plot::graphClicked);

    // @todo Implement saving the graph as image.
    // customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(customPlot, &QCustomPlot::customContextMenuRequested, this,
    //         &Plot::contextMenuRequest);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);
    this->setLayout(layout);
}

void Plot::selectionChanged() {
    // make top and bottom axes be selected synchronously, and handle axis and tick labels as one
    // selectable object:
    if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
        customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
        customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels)) {
        customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
        customPlot->xAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
    }
    // make left and right axes be selected synchronously, and handle axis and tick labels as one
    // selectable object:
    if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
        customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
        customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels)) {
        customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
        customPlot->yAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spTickLabels);
    }
}

void Plot::mousePress() {
    // if an axis is selected, only allow the direction of that axis to be dragged
    // if no axis is selected, both directions may be dragged

    if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        customPlot->axisRect()->setRangeDrag(customPlot->xAxis->orientation());
    else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        customPlot->axisRect()->setRangeDrag(customPlot->yAxis->orientation());
    else
        customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
}

void Plot::mouseWheel() {
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        customPlot->axisRect()->setRangeZoom(customPlot->xAxis->orientation());
    else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        customPlot->axisRect()->setRangeZoom(customPlot->yAxis->orientation());
    else
        customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}

void Plot::addData(double x, double y) {
    minValue = (y < minValue) ? y : minValue;
    maxValue = (y > maxValue) ? y : maxValue;
    lastTime = x;
    customPlot->graph()->addData(x, y);
    if (!updateTimer->isActive()) {
        updatePlot();
        updateTimer->start();
        disableReplotTimer->start();
    } else {
        hadNewData = true;
    }
}

// void Plot::removeSelectedGraph() {
//     if (customPlot->selectedGraphs().size() > 0) {
//         customPlot->removeGraph(customPlot->selectedGraphs().first());
//         customPlot->replot();
//     }
// }

void Plot::contextMenuRequest(QPoint pos) {
    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    if (customPlot->legend->selectTest(pos, false) >= 0)  // context menu on legend requested
    {
        menu->addAction("Move to top left", this, SLOT(moveLegend()))
            ->setData((int)(Qt::AlignTop | Qt::AlignLeft));
        menu->addAction("Move to top center", this, SLOT(moveLegend()))
            ->setData((int)(Qt::AlignTop | Qt::AlignHCenter));
        menu->addAction("Move to top right", this, SLOT(moveLegend()))
            ->setData((int)(Qt::AlignTop | Qt::AlignRight));
        menu->addAction("Move to bottom right", this, SLOT(moveLegend()))
            ->setData((int)(Qt::AlignBottom | Qt::AlignRight));
        menu->addAction("Move to bottom left", this, SLOT(moveLegend()))
            ->setData((int)(Qt::AlignBottom | Qt::AlignLeft));
    } else  // general context menu on graphs requested
    {
        menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
        if (customPlot->selectedGraphs().size() > 0)
            menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
        if (customPlot->graphCount() > 0)
            menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
    }

    menu->popup(customPlot->mapToGlobal(pos));
}

void Plot::graphClicked(QCPAbstractPlottable* plottable, int dataIndex) {
    Q_UNUSED(plottable)
    Q_UNUSED(dataIndex)
    // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
    // usually it's better to first check whether interface1D() returns non-zero, and only then use
    // it.
    //   double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    //   QString message = QString("Clicked on graph '%1' at data point #%2 with value
    //   %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue); statusBar->showMessage(message,
    //   2500);
}

void Plot::updatePlot() {
    auto lowerBound = lastTime - customPlot->xAxis->range().size();
    customPlot->xAxis->setRange(lowerBound, lastTime);
    customPlot->yAxis->setRange(minValue, maxValue);

    customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void Plot::disableUpdating() {
    if (!hadNewData) {
        updateTimer->stop();
        disableReplotTimer->stop();
    }
    hadNewData = false;
}

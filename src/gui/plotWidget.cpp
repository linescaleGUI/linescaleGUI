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
#include <QBoxLayout>
#include <QDebug>
#include <QGraphicsLayout>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

Plot::Plot(QWidget* parent) : QWidget(parent) {
    plot = new JKQTFastPlotter(parent);
    plot->setContentsMargins(0, 0, 0, 0);

    graph =
        new JKQTFPLinePlot(plot, 0, xData.data(), yData.data(), QColor{78, 50, 168});
    plot->addPlot(graph);
    // plot->setPlotUpdateEnabled(false);

    auto layout = new QVBoxLayout(parent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(plot);
    setLayout(layout);
}

void Plot::selectionChanged() {}

void Plot::mousePress() {}

void Plot::mouseWheel() {}

void Plot::addData(double x, double y) {
    minValue = (y < minValue) ? y : minValue;
    maxValue = (y > maxValue) ? y : maxValue;
    
    xData.push_back(lastTime);
    lastTime = x;
    yData.push_back(y);
    graph->setData(xData.data(), yData.data(), xData.length());
}

void Plot::replot() {
    // graph->replot();
}

void Plot::contextMenuRequest(QPoint pos) {
    Q_UNUSED(pos)
    // QMenu* menu = new QMenu(this);
    // menu->setAttribute(Qt::WA_DeleteOnClose);

    // if (customPlot->legend->selectTest(pos, false) >= 0)  // context menu on legend requested
    // {
    //     menu->addAction("Move to top left", this, SLOT(moveLegend()))
    //         ->setData((int)(Qt::AlignTop | Qt::AlignLeft));
    //     menu->addAction("Move to top center", this, SLOT(moveLegend()))
    //         ->setData((int)(Qt::AlignTop | Qt::AlignHCenter));
    //     menu->addAction("Move to top right", this, SLOT(moveLegend()))
    //         ->setData((int)(Qt::AlignTop | Qt::AlignRight));
    //     menu->addAction("Move to bottom right", this, SLOT(moveLegend()))
    //         ->setData((int)(Qt::AlignBottom | Qt::AlignRight));
    //     menu->addAction("Move to bottom left", this, SLOT(moveLegend()))
    //         ->setData((int)(Qt::AlignBottom | Qt::AlignLeft));
    // } else  // general context menu on graphs requested
    // {
    //     menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    //     if (customPlot->selectedGraphs().size() > 0)
    //         menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    //     if (customPlot->graphCount() > 0)
    //         menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
    // }

    // menu->popup(customPlot->mapToGlobal(pos));
}

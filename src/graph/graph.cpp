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
 * along with LinescaleGUI.  If not, see <http://www.gnu.org/licenses/>.      *
 ******************************************************************************/
/**
 * @file graph.cpp
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Initial implementation of graphing logic.
 *
 */

#include "graph.h"
#include <algorithm>

namespace gui {

Graph::Graph() : QtCharts::QChartView() {
    series = new QtCharts::QSplineSeries();
    chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    
    x_axis = chart->axes(Qt::Horizontal, series).back();
    y_axis = chart->axes(Qt::Vertical, series).back();
    
    setRenderHint(QPainter::Antialiasing);
    setChart(chart);
}

void Graph::addDataPoint(float y, float t) {
    maxVal = std::max(y*1.1f, maxVal);
    
    series->append(t, y);
    x_axis->setRange(t-1000, t);
    y_axis->setRange(0, maxVal);
}

void Graph::newReading(float y) {
    ++timePoint;
    addDataPoint(y, timePoint);
}

}  // namespace gui

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
 * @file graph.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Initial implementation of graphing logic.
 */

#pragma once
#ifndef GRAPH_H_
#define GRAPH_H_

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

namespace gui {

class Graph : public QtCharts::QChartView {
    Q_OBJECT
   private:
    QtCharts::QSplineSeries* series;
    QtCharts::QChart* chart;
    QtCharts::QAbstractAxis* x_axis;
    QtCharts::QAbstractAxis* y_axis;
    int timePoint = 0;

    // Maximum value in the graph.
    float maxVal = 0;
    // Minumum value in the graph.
    float minVal = 0;

   public:
    Graph();

    void addDataPoint(float y, float t);
    void newReading(float y);
};

}  // namespace gui

#endif  // GRAPH_H_

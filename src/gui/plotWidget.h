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
 * @file plotWidget.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Plotting functionality based on QCustomPlot
 *
 */

#pragma once
#ifndef PLOTWIDGET_H_
#define PLOTWIDGET_H_

// #include <QCustomPlot/qcustomplot.h>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QWidget>

class Plot : public QWidget {
    Q_OBJECT
   public:
    /**
     * @brief Create a new Plot widget.
     * @param parent The parent widget or `nullptr`.
     */
    Plot(QWidget* parent = nullptr);

    /**
     * @brief Add a single data point without repainting.
     * @param time Horizontal value (time).
     * @param force Vertical value (force).
     */
    void addData(double time, double force);

    /**
     * @brief
     */
    void replot();

   private slots:
    // void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    // void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void contextMenuRequest(QPoint pos);
    // void graphClicked(QCPAbstractPlottable* plottable, int dataIndex);
   private:
    float minValue = 0.0, maxValue = 0.0;
    float lastTime = 0.0;

    QtCharts::QChart* chart;
    QtCharts::QChartView* chartView;
    QtCharts::QValueAxis *xAxis, *yAxis;
};

#endif  // PLOTWIDGET_H_

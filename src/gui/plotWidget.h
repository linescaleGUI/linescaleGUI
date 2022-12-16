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

// @TODO Better way to disable this warning for MSVC.
#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 4127)
#include <QCustomPlot/qcustomplot.h>
#pragma warning(pop)
#else
#include <QCustomPlot/qcustomplot.h>
#endif

#include <QTimer>
#include <QVector>
#include <QWidget>

/**
 * @brief A dynamic line graph chart.
 *
 * @todo Allow to zoom with panning (touchscreen).
 * @todo Maybe implement custom range dialog.
 * @todo Implement saving the graph as image.
 */
class Plot : public QWidget {
   public:
    /**
     * @brief Create a new Plot widget.
     * @param parent The parent widget or `nullptr`.
     */
    Plot(QWidget* parent = nullptr);

    /**
     * @brief Add a single data point.
     *
     * Once the data point is added the plot is updated automatically with a maximum
     * update rate of 60 Hz.
     *
     * @param time Horizontal value (time).
     * @param force Vertical value (force).
     */
    void addData(double time, double force);

    /**
     * @brief Add a new graph to the plot and use it for all new points added.
     */
    void beginNewGraph();

   private slots:
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void mouseMove();
    void contextMenuRequest(QPoint pos);
    void graphClicked(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event);

    void updatePlot();
    void disableUpdating();

   private:
    void clearSelection();

   private:
    QCustomPlot* customPlot;
    double minValue = 0.0, maxValue = 0.0;
    double lastTime = 0.0;
    bool hadNewData = false;

    QTimer* updateTimer;
    QTimer* disableReplotTimer;

    QAction* deleteGraphAction;
    QAction* autoRangeAction;
    QAction* autoShowNewestAction;
    QAction* clearSelectionAction;
};

#endif  // PLOTWIDGET_H_

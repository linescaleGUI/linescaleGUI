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
 * @brief `Plot` declaration
 *
 */

#pragma once
#ifndef PLOTWIDGET_H_
#define PLOTWIDGET_H_

#include <QTimer>
#include <QVector>
#include <QWidget>

#include "../notification/notification.h"
#include "../parser/parser.h"

// @todo Better way to disable this warning for MSVC.
#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 4127)
#include <QCustomPlot/qcustomplot.h>
#pragma warning(pop)
#else
#include <QCustomPlot/qcustomplot.h>
#endif

/**
 * @brief Widget to display a dynamic line graph chart.
 *
 * @todo Allow to zoom with panning (touchscreen).
 * @todo Maybe implement custom range dialog.
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
     * @brief Add a single sample to the data.
     *
     * The sample is appended to the data; its time value will be set as
     * `lastTime + 1/sample.frequency` where `lastTime` is the time of the last data point
     * added to the current graph. `sample.measuredValue` will be used as the force value.
     *
     * If the current unit differs from the previous, the existing plot will be recalculated.
     *
     * @param sample The sample to add.
     */
    inline void addConsecutiveSample(const Sample& sample) {
        if (currentUnit != sample.unitValue) {
            convertToNewUnit(sample.unitValue);
        }
        addData(lastTime + 1.0 / (double)sample.frequency, sample.measuredValue);
    }

    /**
     * @brief Add a new graph to the plot and use it for all new points added.
     *
     * @param startFromOrigin Whether the new graph should begin at `time = 0` or from the
     *                        time of the previous graph.
     */
    void beginNewGraph(bool startFromOrigin = true);

    /**
     * @brief Save the current plot window as png to the local machine
     *
     * @param notification Pointer to send some outputs to the user
     */
    void saveImage(Notification* notification);

   private slots:
    /**
     * @brief Handle a selection change inside the plot.
     */
    void selectionChanged();

    /**
     * @brief Handle a mouse press inside the plot.
     * @param evt Mouse event from qt.
     */
    void mousePress(QMouseEvent* evt);

    /**
     * @brief Handle mouse wheel events inside the plot.
     */
    void mouseWheel();

    /**
     * @brief Handle mouse move events inside the plot.
     */
    void mouseMove();

    /**
     * @brief Show a context menu for the plot.
     * @param pos Mouse position of the right-click relative to the plot.
     */
    void contextMenuRequest(QPoint pos);

    /**
     * @brief Handle a click on a graph.
     * @param plottable The plottable that was clicked.
     * @param dataIndex The nearest data point index to the click position.
     * @param event The actual mouse event of the click.
     */
    void graphClicked(QCPAbstractPlottable* plottable, int dataIndex, QMouseEvent* event);

    /**
     * @brief Replot the graph possibly changing the axis ranges.
     */
    void updatePlot();

    /**
     * @brief Disable the auto updating timer.
     */
    void disableUpdating();

    /**
     * @brief Delete all selected graphs.
     */
    void deleteSelectedGraphs();

   private:
    /**
     * @brief Clear the current selection inside the plot.
     */
    void clearSelection();

    /**
     * @brief Update the plot after a unit change was detected
     *
     * To go from the current unit to the `next`, all values in the plot are
     * multiplied by a conversion factor.
     * The minimum and maximum values are update according to the new values.
     * The initial maximum value is slightly above zero to hide the sensor noise.
     *
     * @param next The next unit
     */
    void convertToNewUnit(UnitValue next);

   private:
    QCustomPlot* customPlot;
    double minValue = 0.0, maxValue = 0.0;
    double lastTime = 0.0;
    bool hadNewData = false;

    UnitValue currentUnit;

    QTimer* updateTimer;
    QTimer* disableReplotTimer;

    QAction* deleteGraphAction;
    QAction* autoRangeAction;
    QAction* autoShowNewestAction;
    QAction* clearSelectionAction;

    static constexpr double factorKnToLbf = 224.8089431;  ///< Convert from kN to lbf
    static constexpr double factorKnToKgf = 101.9716213;  ///< Convert from kN to kgf
};

#endif  // PLOTWIDGET_H_

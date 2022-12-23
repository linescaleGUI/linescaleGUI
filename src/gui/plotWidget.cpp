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
 *
 * @brief `Plot` implementation
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
                                QCP::iSelectPlottables | QCP::iMultiSelect);
    customPlot->xAxis->setLabel("");
    customPlot->yAxis->setLabel("");
    customPlot->legend->setVisible(false);

    customPlot->addGraph();
    customPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);

    QPen graphPen;
    graphPen.setColor(QColor(78, 50, 168, 255));
    graphPen.setWidthF(1);
    customPlot->graph()->setPen(graphPen);
    customPlot->replot();
    customPlot->rescaleAxes();

    // connect slot that ties some axis selections together (especially opposite axes):
    connect(customPlot, &QCustomPlot::selectionChangedByUser, this, &Plot::selectionChanged);
    // connect slots that takes care that when an axis is selected, only that direction can be
    // dragged and zoomed:
    connect(customPlot, &QCustomPlot::mousePress, this, &Plot::mousePress);
    connect(customPlot, &QCustomPlot::mouseWheel, this, &Plot::mouseWheel);
    connect(customPlot, &QCustomPlot::mouseMove, this, &Plot::mouseMove);
    customPlot->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(customPlot, &QCustomPlot::customContextMenuRequested, this, &Plot::contextMenuRequest);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(customPlot);
    this->setLayout(layout);
    this->setContentsMargins(0, 0, 0, 0);

    deleteGraphAction = new QAction("Delete graph", this);
    deleteGraphAction->setShortcuts({QKeySequence::Delete, QKeySequence::Backspace});
    connect(deleteGraphAction, &QAction::triggered, this, &Plot::deleteSelectedGraphs);
    addAction(deleteGraphAction);

    autoRangeAction = new QAction("Auto-range y-axis", this);
    autoRangeAction->setCheckable(true);
    autoRangeAction->setChecked(true);
    addAction(autoRangeAction);

    autoShowNewestAction = new QAction("Auto-show newest data", this);
    autoShowNewestAction->setCheckable(true);
    autoShowNewestAction->setChecked(true);
    addAction(autoShowNewestAction);

    clearSelectionAction = new QAction("Clear selection", this);
    clearSelectionAction->setShortcut(QKeySequence::Cancel);
    connect(clearSelectionAction, &QAction::triggered, this, &Plot::clearSelection);
    addAction(clearSelectionAction);
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

void Plot::beginNewGraph(bool startFromOrigin) {
    customPlot->addGraph();
    if (startFromOrigin) {
        lastTime = 0;
        maxValue = 0;
        minValue = 0;
    }
}

inline bool testAxisSelected(QCPAxis* axis, QPoint pos, int tolerance) {
    auto val = axis->selectTest(pos, true);
    return val > 0 && val < (double)tolerance;
}
void Plot::mousePress(QMouseEvent* evt) {
    // if an axis is selected, only allow the direction of that axis to be dragged
    // if no axis is selected, both directions may be dragged
    auto tolerance = customPlot->selectionTolerance();

    if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
        testAxisSelected(customPlot->xAxis, evt->pos(), tolerance)) {
        customPlot->axisRect()->setRangeDrag(customPlot->xAxis->orientation());
    } else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) ||
               testAxisSelected(customPlot->yAxis, evt->pos(), tolerance)) {
        customPlot->axisRect()->setRangeDrag(customPlot->yAxis->orientation());
    } else {
        customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    }
}

void Plot::mouseWheel() {
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    if (customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        customPlot->axisRect()->setRangeZoom(customPlot->xAxis->orientation());
    else if (customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)) {
        autoRangeAction->setChecked(false);
        customPlot->axisRect()->setRangeZoom(customPlot->yAxis->orientation());
    } else {
        autoRangeAction->setChecked(false);
        customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    }
}

void Plot::mouseMove() {
    if (QApplication::mouseButtons() & Qt::MouseButton::LeftButton) {
        auto rangeDrag = customPlot->axisRect()->rangeDrag();
        if (rangeDrag.testFlag(Qt::Horizontal)) {
            autoShowNewestAction->setChecked(false);
        }
        if (rangeDrag.testFlag(Qt::Vertical)) {
            autoRangeAction->setChecked(false);
        }
    }
}

void Plot::addData(double time, double force) {
    minValue = (force < minValue) ? force : minValue;
    maxValue = (force > maxValue) ? force : maxValue;
    lastTime = time;
    if (customPlot->graphCount() == 0) {
        beginNewGraph();
        // Enable auto range and show newest when the first graph.
        autoRangeAction->setChecked(true);
        autoShowNewestAction->setChecked(true);
    }
    customPlot->graph()->addData(time, force);
    if (!updateTimer->isActive()) {
        updatePlot();
        updateTimer->start();
        disableReplotTimer->start();
    } else {
        hadNewData = true;
    }
}

void Plot::contextMenuRequest(QPoint pos) {
    auto selectionTol = customPlot->selectionTolerance();

    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    bool clickedOnAxis = false;
    if (auto val = customPlot->yAxis->selectTest(pos, true); val > 0 && val < selectionTol) {
        clearSelection();
        customPlot->yAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spAxisLabel |
                                            QCPAxis::spTickLabels);
        clickedOnAxis = true;
    } else if (auto val2 = customPlot->xAxis->selectTest(pos, true);
               val2 > 0 && val2 < selectionTol) {
        clearSelection();
        customPlot->xAxis->setSelectedParts(QCPAxis::spAxis | QCPAxis::spAxisLabel |
                                            QCPAxis::spTickLabels);
        clickedOnAxis = true;
    }
    if (!clickedOnAxis && customPlot->selectedGraphs().length() > 0) {
        menu->addAction(deleteGraphAction);
        menu->addSeparator();
    }

    if (!clickedOnAxis || customPlot->yAxis->selectedParts()) {
        menu->addAction(autoRangeAction);
    }
    if (!clickedOnAxis || customPlot->xAxis->selectedParts()) {
        menu->addAction(autoShowNewestAction);
    }

    menu->popup(customPlot->mapToGlobal(pos));
}

void Plot::updatePlot() {
    auto lowerBound = lastTime - customPlot->xAxis->range().size();

    // @todo Scale to what data-range is in the viewport as option (default?).
    if (autoShowNewestAction->isChecked()) {
        customPlot->xAxis->setRange(lowerBound, lastTime);
    }
    if (autoRangeAction->isChecked()) {
        customPlot->yAxis->setRange(minValue, maxValue);
    }

    customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void Plot::disableUpdating() {
    if (!hadNewData) {
        updateTimer->stop();
        disableReplotTimer->stop();
    }
    hadNewData = false;
}

void Plot::deleteSelectedGraphs() {
    for (auto g : customPlot->selectedGraphs()) {
        customPlot->removeGraph(g);
    }
    customPlot->replot();
}

void Plot::clearSelection() {
    for (auto axis : customPlot->selectedAxes()) {
        axis->setSelectedParts(QCPAxis::spNone);
    }
    for (auto graph : customPlot->selectedPlottables()) {
        graph->setSelection(QCPDataSelection{});
    }
}

void Plot::convertToNewUnit(UnitValue next) {
    double factorBack = 1;
    double factorForward = 1;

    switch (lastUnit) {
        case UnitValue::LBF:
            factorBack = 1 / factorKnToLbf;
            break;

        case UnitValue::KGF:
            factorBack = 1 / factorKnToKgf;
            break;

        default:
            break;
    }

    switch (next) {
        case UnitValue::LBF:
            factorForward = factorKnToLbf;
            break;

        case UnitValue::KGF:
            factorForward = factorKnToKgf;
            break;

        default:
            break;
    }

    double factor = factorBack * factorForward;

    maxValue = 0.5 * factorForward;  // Hide sensor noise (threshold in kN)
    minValue = 0;

    int graphCount = customPlot->graphCount();
    for (int i = 0; i < graphCount; ++i) {
        auto plotData = customPlot->graph(i)->data();
        QCPGraphData* dataPoint;
        double newValue;
        for (dataPoint = plotData->begin(); dataPoint < plotData->end(); ++dataPoint) {
            newValue = dataPoint->value * factor;

            maxValue = newValue > maxValue ? newValue : maxValue;
            minValue = newValue < minValue ? newValue : minValue;

            dataPoint->value = newValue;
        }
    }
}

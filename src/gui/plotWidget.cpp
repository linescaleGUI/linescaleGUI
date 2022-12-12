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
    customPlot = new QCustomPlot(parent);
    
    qDebug() << "devicePixelRatio: " << parent->devicePixelRatioF();

    customPlot->setOpenGl(true);
    customPlot->setNoAntialiasingOnDrag(true);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->xAxis->setLabel("");
    customPlot->yAxis->setLabel("");
    customPlot->legend->setVisible(false);
    // QFont legendFont = font();
    // legendFont.setPointSize(10);
    // customPlot->legend->setFont(legendFont);
    // customPlot->legend->setSelectedFont(legendFont);
    // customPlot->legend->setSelectableParts(
    //     QCPLegend::spItems);  // legend box shall not be selectable, only legend items

    customPlot->addGraph();
    customPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);

    // TODO: hide the dots before they overlap each other
    // customPlot->graph()->setScatterStyle(QCPScatterStyle::ssDisc);

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

   // connect some interaction slots:
    connect(customPlot, &QCustomPlot::axisDoubleClick, this, &Plot::axisLabelDoubleClick);
    // connect(customPlot, &QCustomPlot::legendDoubleClick, this, &Plot::legendDoubleClick);

    // connect slot that shows a message in the status bar when a graph is clicked:
    // connect(customPlot, &QCustomPlot::plottableClick, this, &Plot::graphClicked);

    // setup policy and connect slot for context menu popup:
    // customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(customPlot, &QCustomPlot::customContextMenuRequested, this,
    // &Plot::contextMenuRequest);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);
    this->setLayout(layout);
}

void Plot::axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part) {
    // Set an axis label by double clicking on it
    if (part == QCPAxis::spAxisLabel)  // only react when the actual axis label is clicked, not tick
                                       // label or axis backbone
    {
        bool ok;
        QString newLabel = QInputDialog::getText(
            this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
        if (ok) {
            axis->setLabel(newLabel);
            customPlot->replot();
        }
    }
}

void Plot::legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item) {
    // Rename a graph by double clicking on its legend item
    Q_UNUSED(legend)
    if (item)  // only react if item was clicked (user could have clicked on border padding of
               // legend where there is no item, then item is 0)
    {
        QCPPlottableLegendItem* plItem = qobject_cast<QCPPlottableLegendItem*>(item);
        bool ok;
        QString newName =
            QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal,
                                  plItem->plottable()->name(), &ok);
        if (ok) {
            plItem->plottable()->setName(newName);
            customPlot->replot();
        }
    }
}

void Plot::selectionChanged() {
    /*
     normally, axis base line, axis tick labels and axis labels are selectable separately, but we
     want the user only to be able to select the axis as a whole, so we tie the selected states of
     the tick labels and the axis base line together. However, the axis label shall be selectable
     individually.

     The selection state of the left and right axes shall be synchronized as well as the state of
     the bottom and top axes.

     Further, we want to synchronize the selection of the graphs with the selection state of the
     respective legend item belonging to that graph. So the user can select a graph by either
     clicking on the graph itself or on its legend item.
    */

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

    // synchronize selection of graphs with selection of corresponding legend items:
    for (int i = 0; i < customPlot->graphCount(); ++i) {
        QCPGraph* graph = customPlot->graph(i);
        QCPPlottableLegendItem* item = customPlot->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected()) {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
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

    customPlot->graph()->addData(x, y);
    auto lowerBound = x - customPlot->xAxis->range().size();
    customPlot->xAxis->setRange(lowerBound, x);
    customPlot->yAxis->setRange(minValue, maxValue);
}

void Plot::replot() {
    customPlot->replot();
}

void Plot::removeSelectedGraph() {
    if (customPlot->selectedGraphs().size() > 0) {
        customPlot->removeGraph(customPlot->selectedGraphs().first());
        customPlot->replot();
    }
}

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

void Plot::moveLegend() {
    if (QAction* contextAction =
            qobject_cast<QAction*>(sender()))  // make sure this slot is really called by a context
                                               // menu action, so it carries the data we need
    {
        bool ok;
        int dataInt = contextAction->data().toInt(&ok);
        if (ok) {
            customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
            customPlot->replot();
        }
    }
}

void Plot::graphClicked(QCPAbstractPlottable* plottable, int dataIndex) {
    // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
    // usually it's better to first check whether interface1D() returns non-zero, and only then use
    // it.
    //   double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    //   QString message = QString("Clicked on graph '%1' at data point #%2 with value
    //   %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue); statusBar->showMessage(message,
    //   2500);
}

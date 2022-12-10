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
 * @file dialogconfigure.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief Widget to display the informations about one connection
 *
 */

#pragma once
#ifndef DIALOGCONFIGURE_H_
#define DIALOGCONFIGURE_H_

#include <QDialog>
#include "../deviceCommunication/commMaster.h"
#include "connectionWidget.h"

namespace Ui {
class DialogConfigure;
}

class DialogConfigure : public QDialog {
    Q_OBJECT

   public:
    DialogConfigure(comm::CommMaster* comm, QWidget* parent = nullptr);
    ~DialogConfigure();

   private:
    /**
     * @brief Reload comboBox with all available connections
     *
     */
    void reloadConnections();
    void requestConnection();
    void initWidget();
    void updateFreq(int index);
    Ui::DialogConfigure* ui;
    ConnectionWidget* wConn;
    comm::CommMaster* comm;
    QList<comm::DeviceInfo> devices;
};

#endif  // DIALOGCONFIGURE_H_

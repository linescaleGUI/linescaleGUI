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
 * @file command.h
 * @authors Gschwind, Weber, Schoch, Niederberger
 *
 * @brief This file contains all default commands for the LineScale3
 * 
 * For a detailed description see design/serialProtocol.pdf
 *
 */

#pragma once
#ifndef COMMAND_H_
#define COMMAND_H_

#include <QByteArray>

namespace command {
const QByteArray POWEROFF             = QByteArrayLiteral("\x4F\x0D\x0A\x66"); ///< Power off
const QByteArray SETZERO              = QByteArrayLiteral("\x5A\x0D\x0A\x71"); ///< Zero command
const QByteArray SWITCHTOKN           = QByteArrayLiteral("\x4E\x0D\x0A\x65"); ///< Unit switch to kN command
const QByteArray SWITCHTOKGF          = QByteArrayLiteral("\x47\x0D\x0A\x5E"); ///< Unit switch to kgf command
const QByteArray SWITCHTOLBF          = QByteArrayLiteral("\x42\x0D\x0A\x59"); ///< Unit switch to lbf command
const QByteArray SETSPEED10           = QByteArrayLiteral("\x53\x0D\x0A\x6A"); ///< Speed switch to SLOW(10Hz) command
const QByteArray SETSPEED40           = QByteArrayLiteral("\x46\x0D\x0A\x5D"); ///< Speed switch to FAST(40Hz) command
const QByteArray SETSPEED640          = QByteArrayLiteral("\x4D\x0D\x0A\x64"); ///< Speed switch to 640Hz command
const QByteArray SETSPEED1280         = QByteArrayLiteral("\x51\x0D\x0A\x68"); ///< Speed switch to 1280Hz command
const QByteArray SWITCHMODE           = QByteArrayLiteral("\x4C\x0D\x0A\x63"); ///< Relative zero (zero) or absolute zero (net) mode switching command
const QByteArray SETRELATIVEMODE      = QByteArrayLiteral("\x58\x0D\x0A\x6F"); ///< Switch to relative zero mode command
const QByteArray SETABSOLUTEMODE      = QByteArrayLiteral("\x59\x0D\x0A\x70"); ///< Switch to absolute zero mode command
const QByteArray SETCURRENTTOABSOLUTE = QByteArrayLiteral("\x54\x0D\x0A\x6B"); ///< Set the current value as the absolute zero command
const QByteArray RESETPEAK            = QByteArrayLiteral("\x43\x0D\x0A\x5A"); ///< Peak clearing operation command
const QByteArray REQUESTONLINE        = QByteArrayLiteral("\x41\x0D\x0A\x58"); ///< Request PC or Bluetooth online command
const QByteArray DISCONNECTONLINE     = QByteArrayLiteral("\x45\x0D\x0A\x5C"); ///< Disconnect PC or Bluetooth online command
const QByteArray READFIRSTLOG         = QByteArrayLiteral("\x52\x30\x30\x0D\x0A\xC9"); ///<  Read the first log command
const QByteArray READLASTLOG          = QByteArrayLiteral("\x52\x39\x39\x0D\x0A\xDB"); ///<  Read the 100th log command
}  // namespace command

#endif  // COMMAND_H_



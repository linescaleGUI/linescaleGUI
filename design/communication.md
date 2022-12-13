# Communication

## Communication master
Provide a class as an interface to all connected devices. From this interface
upwards the type of connection shouldn't matter.

### USB communication
[Qt5 SerialPort](https://doc.qt.io/qt-5/qserialport.html)

SerialPort handling is done with one of the following libraries:

- `#include <QSerialPort>`
- `#include <QSerialPortInfo>`

***QSerialPort*** provides all methods to interact with a serial device.
`QSerialPort::QSerialPort(const QString &name, QObject *parent = nullptr)` to
create a new connection on port `<name>`
The baudRate will be set afterwards.

***QSerialPortInfo*** provides all connected devices; could be used on startup
to scan all connected devices. <br>
Get a array of QSerialPortInfo with
`QList<QSerialPortInfo> listOfCOMPorts = QSerialPortInfo::availablePorts();`

### BLE communication
[Qt5 Bluetooth](https://doc.qt.io/qt-5/qtbluetooth-index.html)
[Qt5 BluetoothLowEnergy](https://doc.qt.io/qt-5/qtbluetooth-le-overview.html)

Following classes are used to implement the BLE feature:

- `#include <QtBluetooth/QBluetoothLocalDevice>`
- `#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>`
- `#include <QLowEnergyController>`

***QtBluetooth/QBluetoothLocalDevice*** provides a standard interface to the
local bluetooth adapter. THerefore, it can be turned on and off trough this
interface.

***QtBluetooth/QBluetoothDeviceDiscoveryAgent*** provides services to search for
all sorts of bluetooth devices. For this implementation only BLE devices are
of interest.

***QLowEnergyController*** provides access to BLE devices for sending and
writing data back and forth.

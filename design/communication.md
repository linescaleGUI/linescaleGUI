# Communication

## Communication master
Provide a class as an interface to all connected devices. From this interface upwards the type of connection shouldn't matter.

### USB communication
[Qt5 SerialPort](https://doc.qt.io/qt-5/qserialport.html)

SerialPort handling is done with one of the following libraries:

- ```#include <QSerialPort>```
- ```#include <QSerialPortInfo>```

***QSerialPort*** provides all methods to interact with a serial device.  
```QSerialPort::QSerialPort(const QString &name, QObject *parent = nullptr)``` to create a new connection on port ```<name>```  
The baudRate will be set afterwards. 

***QSerialPortInfo*** provides all connected devices; could be used on startup to scan all connected devices. <br>
Get a array of QSerialPortInfo with
```QList<QSerialPortInfo> listOfCOMPorts = QSerialPortInfo::availablePorts();```

### BLE communication
```Todo by @Lemi0002```

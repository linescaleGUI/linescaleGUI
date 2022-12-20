#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <vector>
#include <QObject>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include "BluetoothDevice.h"

class Bluetooth : public QObject {
    Q_OBJECT

public:
    Bluetooth();
    ~Bluetooth(void);
    void PowerOn(void);
    void PowerOff(void);
    void PowerToggle(void);
    bool IsPowerOn(void);

    void ScanStart(void);
    void ScanStop(void);
    void ScanToggle(void);
    bool IsScanning(void);
    std::vector<BluetoothDevice*>& DevicesGet(void);

signals:
    //void PowerChanged(void);
    void PowerTurnedOn(void);
    void PowerTurnedOff(void);

    void ScanStarted(void);
    void ScanStopped(std::vector<BluetoothDevice*>& devices);
    void ScanDeviceDiscovered(BluetoothDevice* device);

    //void Connected(BluetoothDevice* device);
    //void Disconnected(BluetoothDevice* device);

private slots:
    //void LocalDeviceConnected(const QBluetoothAddress &address);
    //void LocalDeviceDisconnected(const QBluetoothAddress &address);
    void LocalDeviceErrorOccurred(QBluetoothLocalDevice::Error error);
    void LocalDeviceHostModeStateChanged(QBluetoothLocalDevice::HostMode state);
    //void LocalDevicePairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing);
    void DeviceDiscoveryAgentCanceled(void);
    void DeviceDiscoveryAgentDeviceDiscovered(const QBluetoothDeviceInfo& deviceInfo);
    void DeviceDiscoveryAgentDeviceUpdated(const QBluetoothDeviceInfo& deviceInfo, QBluetoothDeviceInfo::Fields fields);
    void DeviceDiscoveryAgentErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);
    void DeviceDiscoveryAgentFinished(void);

private:
    QBluetoothLocalDevice::HostMode stateBefore;
    QBluetoothLocalDevice localDevice;
    QBluetoothDeviceDiscoveryAgent deviceDiscoveryAgent;
    std::vector<BluetoothDevice*> devices;
};

#endif // BLUETOOTH_H

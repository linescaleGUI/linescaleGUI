#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <vector>
#include <QObject>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include "BluetoothService.h"

class BluetoothDevice : public QObject {
    Q_OBJECT

public:
    BluetoothDevice(const QBluetoothDeviceInfo& deviceInfo);
    ~BluetoothDevice();
    void Connect(void);
    void Disconnect(void);
    void Read(void);
    void Write(QByteArray& value);
    QBluetoothDeviceInfo& DeviceInfoGet(void);

signals:
    void Connected(BluetoothDevice* device);
    void Disconnected(BluetoothDevice* device);
    void ServiceDiscoveryFinished(BluetoothDevice* device, std::vector<BluetoothService*>& services);
    void ServiceDiscoveryFailed(BluetoothDevice* device);

    void CharacteristicChanged(BluetoothDevice* device, const QByteArray& value);
    void CharacteristicRead(BluetoothDevice* device, const QByteArray& value);
    void CharacteristicWritten(BluetoothDevice* device, const QByteArray& value);

private slots:
    void LowEnergyControllerConnected(void);
    void LowEnergyControllerConnectionUpdated(const QLowEnergyConnectionParameters& connectionParameters);
    void LowEnergyControllerDisconnected(void);
    void LowEnergyControllerDiscoveryFinished(void);
    void LowEnergyControllerErrorOccurred(QLowEnergyController::Error error);
    //void LowEnergyControllerMtuChanged(int mtu);
    void LowEnergyControllerServiceDiscovered(const QBluetoothUuid& uuid);
    void LowEnergyControllerStateChanged(QLowEnergyController::ControllerState state);

    void ServiceDetailsDiscovered(void);
    void ServiceCharacteristicChanged(const QLowEnergyCharacteristic& characteristic, const QByteArray& value);
    void ServiceCharacteristicRead(const QLowEnergyCharacteristic& characteristic, const QByteArray& value);
    void ServiceCharacteristicWritten(const QLowEnergyCharacteristic& characteristic, const QByteArray& value);

private:
    static const QBluetoothUuid UUID_SERVICE;
    static const QBluetoothUuid UUID_CHARACTERISTIC_READ;
    static const QBluetoothUuid UUID_CHARACTERISTIC_WRITE;

    QBluetoothDeviceInfo deviceInfo;

    size_t serviceIndex;
    QLowEnergyController* lowEnergyController;
    std::vector<BluetoothService*> services;

    BluetoothService* communicationService;
    QLowEnergyCharacteristic communicationCharacteristicRead;
    QLowEnergyCharacteristic communicationCharacteristicWrite;
};

#endif // BLUETOOTHDEVICE_H

#ifndef BLUETOOTHSERVICE_H
#define BLUETOOTHSERVICE_H

#include <vector>
#include <QObject>
#include <QLowEnergyController>

class BluetoothService : public QObject {
    Q_OBJECT

public:
    BluetoothService(QLowEnergyService* service);
    ~BluetoothService();
    //bool UuidSet(const QBluetoothUuid &uuid);
    bool DiscoverDetails(void);
    QLowEnergyService* ServiceGet(void);
    void Read(QLowEnergyCharacteristic &characteristic);
    void Write(QLowEnergyCharacteristic &characteristic, QByteArray& value);
    void ReadDescriptor(QLowEnergyDescriptor &descriptor);
    void WriteDescriptor(QLowEnergyDescriptor &descriptor, const QByteArray &value);

signals:
    void CharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void CharacteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void CharacteristicWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void DescriptorRead(const QLowEnergyDescriptor &descriptor, const QByteArray &value);
    void DescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &value);
    void ErrorOccurred(void);
    void DetailsDiscovered(void);

private slots:
    void LowEnergyServiceCharacteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void LowEnergyServiceCharacteristicRead(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void LowEnergyServiceCharacteristicWritten(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void LowEnergyServiceDescriptorRead(const QLowEnergyDescriptor &descriptor, const QByteArray &value);
    void LowEnergyServiceDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &value);
    void LowEnergyServiceErrorOccurred(QLowEnergyService::ServiceError error);
    void LowEnergyServiceStateChanged(QLowEnergyService::ServiceState state);

private:
    QLowEnergyService* service;
};

#endif // BLUETOOTHSERVICE_H

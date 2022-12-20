#include "BluetoothService.h"

BluetoothService::BluetoothService(QLowEnergyService* service)
    : service(service) {

    if(service == nullptr) {
        return;
    }

    connect(service, &QLowEnergyService::characteristicChanged, this, &BluetoothService::LowEnergyServiceCharacteristicChanged);
    connect(service, &QLowEnergyService::characteristicRead, this, &BluetoothService::LowEnergyServiceCharacteristicRead);
    connect(service, &QLowEnergyService::characteristicWritten, this, &BluetoothService::LowEnergyServiceCharacteristicWritten);
    connect(service, &QLowEnergyService::descriptorRead, this, &BluetoothService::LowEnergyServiceDescriptorRead);
    connect(service, &QLowEnergyService::descriptorWritten, this, &BluetoothService::LowEnergyServiceDescriptorWritten);
    connect(service, &QLowEnergyService::errorOccurred, this, &BluetoothService::LowEnergyServiceErrorOccurred);
    connect(service, &QLowEnergyService::stateChanged, this, &BluetoothService::LowEnergyServiceStateChanged);
}

BluetoothService::~BluetoothService() {
    if(service == nullptr) {
        return;
    }

    disconnect(service, &QLowEnergyService::characteristicChanged, this, &BluetoothService::LowEnergyServiceCharacteristicChanged);
    disconnect(service, &QLowEnergyService::characteristicRead, this, &BluetoothService::LowEnergyServiceCharacteristicRead);
    disconnect(service, &QLowEnergyService::characteristicWritten, this, &BluetoothService::LowEnergyServiceCharacteristicWritten);
    disconnect(service, &QLowEnergyService::descriptorRead, this, &BluetoothService::LowEnergyServiceDescriptorRead);
    disconnect(service, &QLowEnergyService::descriptorWritten, this, &BluetoothService::LowEnergyServiceDescriptorWritten);
    disconnect(service, &QLowEnergyService::errorOccurred, this, &BluetoothService::LowEnergyServiceErrorOccurred);
    disconnect(service, &QLowEnergyService::stateChanged, this, &BluetoothService::LowEnergyServiceStateChanged);

    // TODO: Is this redundant?
    delete service;
}

bool BluetoothService::DiscoverDetails(void) {
    switch(service->state()) {
        case QLowEnergyService::RemoteService:
            service->discoverDetails();
            return true;
        case QLowEnergyService::InvalidService:
        case QLowEnergyService::RemoteServiceDiscovering:
        case QLowEnergyService::RemoteServiceDiscovered:
        case QLowEnergyService::LocalService:
        default:
            return false;
    }
}

QLowEnergyService* BluetoothService::ServiceGet(void) {
    return service;
}

void BluetoothService::Read(QLowEnergyCharacteristic& characteristic) {
    if(service == nullptr) {
        return;
    }

    service->readCharacteristic(characteristic);
}

void BluetoothService::Write(QLowEnergyCharacteristic& characteristic, QByteArray& value) {
    if(service == nullptr) {
        return;
    }

    service->writeCharacteristic(characteristic, value);
}

void BluetoothService::ReadDescriptor(QLowEnergyDescriptor &descriptor) {
    if(service == nullptr) {
        return;
    }

    service->readDescriptor(descriptor);
}

void BluetoothService::WriteDescriptor(QLowEnergyDescriptor &descriptor, const QByteArray &value) {
    if(service == nullptr) {
        return;
    }

    service->writeDescriptor(descriptor, value);
}

/*
bool BluetoothService::UuidSet(const QBluetoothUuid &uuid) {

}
*/

void BluetoothService::LowEnergyServiceCharacteristicChanged(const QLowEnergyCharacteristic& characteristic, const QByteArray& value) {
    emit CharacteristicChanged(characteristic, value);
}

void BluetoothService::LowEnergyServiceCharacteristicRead(const QLowEnergyCharacteristic& characteristic, const QByteArray& value) {
    emit CharacteristicRead(characteristic, value);
}

void BluetoothService::LowEnergyServiceCharacteristicWritten(const QLowEnergyCharacteristic& characteristic, const QByteArray& value) {
    emit CharacteristicWritten(characteristic, value);
}

void BluetoothService::LowEnergyServiceDescriptorRead(const QLowEnergyDescriptor &descriptor, const QByteArray &value) {
    emit DescriptorRead(descriptor, value);
}

void BluetoothService::LowEnergyServiceDescriptorWritten(const QLowEnergyDescriptor &descriptor, const QByteArray &value) {
    emit DescriptorWritten(descriptor, value);
}

void BluetoothService::LowEnergyServiceErrorOccurred(QLowEnergyService::ServiceError error) {
    // TODO: Implement function
}

void BluetoothService::LowEnergyServiceStateChanged(QLowEnergyService::ServiceState state) {
    if(state == QLowEnergyService::RemoteServiceDiscovered) {
        emit DetailsDiscovered();
    }
}

#include "Bluetooth.h"

Bluetooth::Bluetooth() {
    stateBefore = localDevice.hostMode();
    deviceDiscoveryAgent.setLowEnergyDiscoveryTimeout(5000);
    ScanStop();

    connect(&localDevice, &QBluetoothLocalDevice::errorOccurred, this, &Bluetooth::LocalDeviceErrorOccurred);
    connect(&localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this, &Bluetooth::LocalDeviceHostModeStateChanged);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &Bluetooth::DeviceDiscoveryAgentCanceled);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Bluetooth::DeviceDiscoveryAgentDeviceDiscovered);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceUpdated, this, &Bluetooth::DeviceDiscoveryAgentDeviceUpdated);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this, &Bluetooth::DeviceDiscoveryAgentErrorOccurred);
    connect(&deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Bluetooth::DeviceDiscoveryAgentFinished);
}

Bluetooth::~Bluetooth(void) {
    devices.clear();
}

void Bluetooth::PowerOn(void) {
    /* May not work on iOS*/
    localDevice.setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void Bluetooth::PowerOff(void) {
    ScanStop();
    devices.clear();

    /* May not work on iOS*/
    localDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void Bluetooth::PowerToggle(void) {
    if(IsPowerOn()) {
        PowerOff();
    } else {
        PowerOn();
    }
}

bool Bluetooth::IsPowerOn(void) {
    return (localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) ? false : true;
}

void Bluetooth::ScanStart(void) {
    if(!IsPowerOn()) {
        return;
    }

    if(!IsScanning()) {
        devices.clear();
        emit ScanStarted();
        deviceDiscoveryAgent.start();
    }
}

void Bluetooth::ScanStop(void) {
    if(IsScanning()) {
        deviceDiscoveryAgent.stop();
    }
}

void Bluetooth::ScanToggle(void) {
    if(IsScanning()) {
        ScanStop();
    } else {
        ScanStart();
    }
}

bool Bluetooth::IsScanning(void) {
    return deviceDiscoveryAgent.isActive();
}

std::vector<BluetoothDevice*>& Bluetooth::DevicesGet(void) {
    return devices;
}

/*
void Bluetooth::LocalDeviceConnected(const QBluetoothAddress &address) {

}

void Bluetooth::LocalDeviceDisconnected(const QBluetoothAddress &address) {

}
*/

void Bluetooth::LocalDeviceErrorOccurred(QBluetoothLocalDevice::Error error) {
    // TODO: Do error checking
}

void Bluetooth::LocalDeviceHostModeStateChanged(QBluetoothLocalDevice::HostMode state) {
    if((stateBefore == QBluetoothLocalDevice::HostMode::HostPoweredOff) && (state != QBluetoothLocalDevice::HostMode::HostPoweredOff)) {
        emit PowerTurnedOn();
    } else if(state == QBluetoothLocalDevice::HostMode::HostPoweredOff) {
        ScanStop();
        devices.clear();
        emit PowerTurnedOff();
    }

    stateBefore = state;
}

/*
void Bluetooth::LocalDevicePairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing) {

}
*/

void Bluetooth::DeviceDiscoveryAgentCanceled() {
    emit ScanStopped(devices);
}

void Bluetooth::DeviceDiscoveryAgentDeviceDiscovered(const QBluetoothDeviceInfo& deviceInfo) {
    if(deviceInfo.coreConfigurations() == QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        BluetoothDevice* device = new BluetoothDevice(deviceInfo);
        devices.push_back(device);
        emit ScanDeviceDiscovered(device);
    }
}

void Bluetooth::DeviceDiscoveryAgentDeviceUpdated(const QBluetoothDeviceInfo& deviceInfo, QBluetoothDeviceInfo::Fields fields) {
    // TODO: Implement function
}

void Bluetooth::DeviceDiscoveryAgentErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error) {
    // TODO: Do error checking
}

void Bluetooth::DeviceDiscoveryAgentFinished() {
    emit ScanStopped(devices);
}

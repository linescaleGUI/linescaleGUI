```mermaid
flowchart RL
    subgraph GUI
        direction RL
        MainWindow<-->DeviceView
        MainWindow<-->Plotting
        MainWindow<-->AboutDialog
    end

    subgraph Communication
        direction LR
        CommunicationMaster<-->Parser;
        CommunicationMaster<-->USB;
        CommunicationMaster<-->BLE;
    end

    Communication<-->GUI;
```

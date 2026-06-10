<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                         LANGUAGE                                          | -->
<!-- |-------------------------------------------------------------------------------------------| -->
<div align="right">
  <a href="README.md">
    <img src="https://img.shields.io/badge/🇫🇷 Français-555555?style=for-the-badge" alt="Français"/>
  </a>
  <a href="README.en.md">
    <img src="https://img.shields.io/badge/🇬🇧 English-1e3a5f?style=for-the-badge" alt="English"/>
  </a>
</div>

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                          HEADER                                           | -->
<!-- |-------------------------------------------------------------------------------------------| -->
<h1 align="left">🌐 STP Network Simulator</h1>

<p align="left">
  A local network simulator written in C implementing the STP (Spanning Tree Protocol) and Ethernet switching, with frame propagation simulation.
</p>

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                       ABOUT                                               | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## About the project

This simulator allows you to model and simulate the behavior of a local area network (LAN) composed of stations and switches. It was developed as part of a project at IUT Robert Schuman.

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://www.cprogramming.com/)

The simulator implements :
- The **STP** protocol to prevent network loops through BPDU exchange
- **Ethernet switching** with automatic MAC address learning
- **Frame propagation** between stations through switches
- Display of **switching tables** and **STP port states**
- The ability to **enable or disable STP** in real time

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                        PREVIEW                                            | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## 📸 Preview

**Main menu** : Simulator startup interface with the 6 available options.

<div align="center">
<img src="assets/menu.png" alt="Main menu" width="600"/>
</div>
<br>

**Available stations** : List of network stations with their MAC and IP addresses.

<div align="center">
<img src="assets/stations.png" alt="Available stations" width="600"/>
</div>
<br>

**Frame simulation** : Tick-by-tick propagation of an Ethernet frame through the network.

<div align="center">
<img src="assets/simulation.png" alt="Frame simulation" width="600"/>
</div>
<br>

**STP port states** : Display of each port state after STP protocol convergence.

<div align="center">
<img src="assets/stp_ports.png" alt="STP port states" width="400"/>
</div>
<br>

**Adjacency matrix** : Visualization of the network topology and link costs.

<div align="center">
<img src="assets/matrice.png" alt="Adjacency matrix" width="400"/>
</div>
<br>

**Switching tables** : MAC tables dynamically learned by each switch.

<div align="center">
<img src="assets/tables_mac.png" alt="Switching tables" width="400"/>
</div>
<br>

**STP disabled** : All ports switch to designated state, loops are no longer handled.

<div align="center">
<img src="assets/stp_off.png" alt="STP disabled" width="600"/>
</div>
<br>

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                      INSTALLATION                                         | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## ⚙️ Installation

### Requirements

- **GCC** (GNU Compiler Collection)
- **Make**
- A terminal compatible with ANSI color codes

### Compilation

&nbsp; &nbsp; **1. Clone the repository**
```sh
git clone https://github.com/rmax3iu/STP-network-simulator.git
cd STP-network-simulator
```

&nbsp; &nbsp; **2. Compile the project**
```sh
make
```

&nbsp; &nbsp; **3. Run the simulator**
```sh
./bin/sae23
```

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                         USAGE                                             | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## 🚀 Usage

At startup, the simulator asks for a configuration file. Examples are available in the `configs/` folder.

```sh
Please enter the file name : configs/config2.txt
```

| Option | Description |
|--------|-------------|
| 1 | Run an Ethernet frame simulation |
| 2 | Display STP port states |
| 3 | Display the adjacency matrix |
| 4 | Display MAC tables |
| 5 | Quit |
| 6 | Enable / Disable STP |

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                      STP PROTOCOL                                         | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## 🌳 STP Protocol

The STP (Spanning Tree Protocol) prevents network loops by assigning a state to each port of each switch :

| State | Description |
|-------|-------------|
| ![Root](https://img.shields.io/badge/ROOT-2e7d32?style=flat-square&logoColor=white) | Port with the best path to the root bridge |
| ![Designated](https://img.shields.io/badge/DESIGNATED-f57f17?style=flat-square&logoColor=white) | Open port, forwards frames normally |
| ![Blocked](https://img.shields.io/badge/BLOCKED-c62828?style=flat-square&logoColor=white) | Closed port to break loops |

The **root bridge** is elected automatically : it is the switch with the smallest MAC address.

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                   CONFIGURATION FORMAT                                    | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## 📄 Configuration file format

```
<nb_devices> <nb_links>
2;<MAC>;<nb_ports>;<priority>     (switch)
1;<MAC>;<IP>                      (station)
<idx_a>;<idx_b>;<cost>            (link)
```

### 💡 Example

```
4 3
2;01:45:23:a6:f7:ab;8;1024
1;54:d6:a6:82:c5:23;130.79.80.21
1;c8:69:72:5e:43:af;130.79.80.27
1;77:ac:d6:82:12:23;130.79.80.42
0;1;4
0;2;19
0;3;4
```

| Bandwidth | STP Cost |
|-----------|----------|
| 10 Mb/s | 100 |
| 100 Mb/s | 19 |
| 1 Gb/s | 4 |

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                    PROJECT STRUCTURE                                      | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## 📁 Project structure

```
STP-network-simulator/
├── src/
│   ├── main.c          # Entry point and menu
│   ├── stp.c / stp.h   # STP protocol and scheduler
│   ├── reseau.c / .h   # Network structure and management
│   ├── switch.c / .h   # Switch and MAC table
│   ├── station.c / .h  # Station (MAC + IP)
│   ├── ethernet.c / .h # Ethernet frame
│   ├── config.c / .h   # Configuration file reader
│   ├── matrice.c / .h  # Adjacency matrix
│   ├── adresse.c / .h  # MAC and IPv4 types
│   └── utils.c / .h    # Utility functions
├── configs/            # Network configuration files
├── assets/             # Screenshots
├── docs/               # Documentation and subject
└── Makefile
```

<!-- |-------------------------------------------------------------------------------------------| -->
<!-- |                                       CONTRIBUTORS                                        | -->
<!-- |-------------------------------------------------------------------------------------------| -->
## 👥 Contributors

Project developed as a team of three at IUT Robert Schuman.

<div align="center">

[![rmax3iu](https://img.shields.io/badge/rmax3iu-1e3a5f?style=for-the-badge&logo=github&logoColor=white)](https://github.com/rmax3iu)
[![lucastreiber](https://img.shields.io/badge/lucastreiber-1e3a5f?style=for-the-badge&logo=github&logoColor=white)](https://github.com/lucastreiber)
[![marwaaan212](https://img.shields.io/badge/marwaaan212-1e3a5f?style=for-the-badge&logo=github&logoColor=white)](https://github.com/marwaaan212)

</div>

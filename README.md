# VNE-Sim 🕸

![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=flat&logo=cmake&logoColor=white)
![C++](https://img.shields.io/badge/C++11-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white)

**VNE-Sim** is a discrete event simulator designed for evaluating Virtual Network Embedding (VNE) algorithms. Written in C++11, it provides a robust platform for researchers and developers to test and compare different VNE strategies in a realistic simulation environment.

This implementation has been used in the article: **S. Haeri** and **Lj. Trajkovic**,
[_"Virtual network embedding via Monte-Carlo tree search,"_](http://www.sfu.ca/~ljilja/papers/t-cybernetics-2016_web.pdf)
IEEE Transactions on Cybernetics, vol. 48, no. 2, pp. 510-521, Feb. 2018.

---

## 💡 About VNE-Sim

Virtual Network Embedding (VNE) is a critical challenge in network virtualization. It involves efficiently mapping virtual network requests (VNRs) onto a shared substrate network infrastructure. VNE-Sim is built to facilitate the development and performance evaluation of VNE algorithms. It allows users to:

- **Simulate VNE Algorithms**: Implement and test various node and link embedding algorithms.
- **Evaluate Performance**: Measure key performance metrics such as request acceptance ratio, resource utilization, revenue, and cost.
- **Experiment with Topologies**: Generate and use different substrate and virtual network topologies using BRITE and FNSS network generators.
- **Customize Simulations**: Configure various simulation parameters, including VNR arrival rates, durations, resource demands, and algorithm-specific settings.

## 🏛️ Architecture

VNE-Sim is designed with a modular architecture, comprising the following key components:

- **Core Simulation Engine**: A discrete event simulation engine built using ADEVS, handling event scheduling and execution.
- **Network Model**: Represents substrate and virtual networks with nodes and links, supporting resource allocation and management.
- **VNE Algorithms**: Implements various VNE algorithms, including:
    - **Vineyard**: A two-stage algorithm with Linear Programming based node and link mapping.
    - **MC-VNE**: Monte Carlo Tree Search based Node Embedding with options for MCF or BFS-SP link embedding.
    - **GRC**: Global Resource Capacity based Node Embedding with options for MCF or BFS-SP link embedding.
- **Network Generators**: Integrates BRITE and FNSS for generating realistic network topologies:
    - **BRITE**: Boston university Representative Topology gEnerator for creating AS-level and Router-level topologies.
    - **FNSS**:  Framework for Network Simulation and Service for generating Datacenter Network Topologies.
- **Statistics and Logging**: Collects and logs detailed simulation statistics using Hiberlite for database storage and Boost.Log for logging.
- **Command-Line Interface (CLI)**: Provides a user-friendly command-line interface for configuring and running simulations, and generating network files.

## 🛠️ Building VNE-Sim

### Prerequisites

| Component              | Purpose                                    | Debian/Ubuntu Package    |
| ---------------------- | ------------------------------------------ | ------------------------ |
| Python 3.x             | Build scripts & FNSS installation          | `python3`                |
| CMake (≥3.14)           | Build system                               | `cmake`                  |
| GNU Scientific Library | Random number generation                   | `libgsl-dev`             |
| GLPK                   | Mixed-integer linear programming           | `libglpk-dev`            |
| Boost                  | Filesystem, logging, threading, unit tests | `libboost-all-dev`       |
| MPI (OpenMPI/MPICH)    | Parallelization for MCTS                   | `libopenmpi-dev`         |
| SQLite3                | Simulation results storage                 | `sqlite3 libsqlite3-dev` |

#### Development only Dependencies

| Component    | Purpose                                                        | Debian/Ubuntu Package    |
| ------------ | -------------------------------------------------------------- | ------------------------ |
| pre-commit   | Installs the pre commit hooks to ensure proper code formatting | pip package (pre-commit) |
| clang-format | code formatting                                                | `clang-format`           |

### 🏗️ Build Steps

1. **Clone and Install Dependencies with CMake**

   ```bash
   git clone https://bitbucket.org/shaeri/vne-sim.git
   # if you do not need FNSS for creating datacenter networks leave FNSS_SUPPORT=OFF
   mkdir build && cd build && cmake .. -DWITH_FNSS_SUPPORT=ON
   ```

2. **Install FNSS Python Package**

   ```bash
   # assuming you are still in the build folder
   cd deps/src/fnss
   # install fnss for user or any virtual env you want to use
   python3 setup.py install --user
   # alternatively you can install it system wide
   sudo python3 setup.py install
   ```

3. **Compile VNE-Sim**

   ```bash
   make -j$(nproc)
   ```

4. **Further steps for development**

   ```bash
   # install the pre-commit hooks
   pre-commit install
   ```

---

## 🚀 Running VNE-Sim

### CLI Command Structure

```bash
./build/bin/vnesim --config [CONFIG_FILE] [SUBCOMMAND] [OPTIONS]
```

#### Subcommands

VNE-Sim CLI is structured around subcommands to organize different functionalities:

- `experiment`: Subcommands for running VNE simulation experiments.
- `netgen`: Subcommands for generating network files (substrate and virtual requests).

#### Experiment Subcommands

The `experiment` subcommand is used to run VNE algorithms.

```bash
./build/bin/vnesim experiment [ALGORITHM] [OPTIONS]
```

##### Algorithms

The following algorithms are supported via subcommands under `experiment`:

- `vineyard`: Runs the **Vineyard** algorithm.
  ```bash
  ./build/bin/vnesim experiment vineyard --config config.toml
  ```

- `grc`: Runs the **GRC** (Global Resource Capacity) algorithm. Requires specifying a link embedding algorithm.
  ```bash
  ./build/bin/vnesim experiment grc --config config.toml -l [LINK_EMBEDDING_ALGO]
  ```
  - `-l, --link-embedding-algo [LINK_EMBEDDING_ALGO]`: Specifies the link embedding algorithm for GRC. Options are:
    - `sp`: Shortest Path
    - `mcf`: Multi-Commodity Flow

- `mcvne`: Runs the **MCVNE** (Monte Carlo Virtual Network Embedding) algorithm. Requires specifying a link embedding algorithm.
  ```bash
  ./build/bin/vnesim experiment mcvne --config config.toml -l [LINK_EMBEDDING_ALGO]
  ```
    - `-l, --link-embedding-algo [LINK_EMBEDDING_ALGO]`: Specifies the link embedding algorithm for MCVNE. Options are:
        - `sp`: Shortest Path
        - `mcf`: Multi-Commodity Flow

##### Experiment Options

- `-c, --config [CONFIG_FILE]`: Specifies the path to the TOML configuration file. Defaults to `config.toml`.
- `-l, --link-embedding-algo [LINK_EMBEDDING_ALGO]`: (For `grc` and `mcvne` subcommands) Specifies the link embedding algorithm.

#### Network Generation Subcommands

The `netgen` subcommand is used to generate network files.

```bash
./build/bin/vnesim netgen [NETWORK_TYPE] [OPTIONS]
```

##### Network Types

- `substrate`: Generates substrate network files.
  ```bash
  ./build/bin/vnesim netgen substrate --config config.toml -H [HANDLER] -t [TOPOLOGY_TYPE] -n [NUM_NODES]
  ```
    - `-H, --handler [HANDLER]`: Network generator handler. Options are `BRITE` or `FNSS`.
    - `-t, --topology [TOPOLOGY_TYPE]`: Substrate network topology type (handler dependent).
    - `-n, --num-nodes [NUM_NODES]`: Number of nodes in the substrate network.

- `vr`: Generates virtual network request files.
  ```bash
  ./build/bin/vnesim netgen vr --config config.toml -H [HANDLER] -t [TOPOLOGY_TYPE] -T [TOTAL_TIME]
  ```
    - `-H, --handler [HANDLER]`: Network generator handler. Options are `BRITE` or `FNSS`.
    - `-t, --topology [TOPOLOGY_TYPE]`: Virtual network topology type (handler dependent).
    - `-T, --total-time [TOTAL_TIME]`: Total simulation time for VNR generation.

##### Network Generation Options

- `-H, --handler [HANDLER]`: Network generator handler (`BRITE` or `FNSS`).
- `-t, --topology [TOPOLOGY_TYPE]`: Topology type (handler dependent).
- `-d, --dest [PATH]`: Destination directory to save generated network files.
- `-b, --brite-seed-file [FILE]`: BRITE seed file path.
- `-p, --brite-node-placement [PLACEMENT]`: BRITE node placement strategy (`1` for Random, `2` for Heavy Tailed).
- `-n, --brite-num-neighbors [NUM]`: BRITE neighbors per node.
- `-i, --brite-inner-grid [SIZE]`: BRITE inner grid size.
- `-o, --brite-outer-grid [SIZE]`: BRITE outer grid size.
- `-g, --waxman-growth-type [TYPE]`: Waxman growth type (`1` for Incremental, `2` for All).
- `-A, --waxman-alpha [ALPHA]`: Waxman alpha parameter (0-1).
- `-B, --waxman-beta [BETA]`: Waxman beta parameter (0-1).
- `--sn-cpu-dist [DIST]`: Substrate node CPU distribution type.
- `--sn-cpu-p1 [PARAM]`: Substrate node CPU distribution parameter 1.
- `--sl-bw-dist [DIST]`: Substrate link bandwidth distribution type.
- `--sl-bw-p1 [PARAM]`: Substrate link bandwidth distribution parameter 1.
- `--vnr-num-nodes-dist [DIST]`: VNR number of nodes distribution type.
- `--vnr-num-nodes-p1 [PARAM]`: VNR number of nodes distribution parameter 1.
- `--vnr-duration-dist [DIST]`: VNR duration distribution type.
- `--vnr-duration-p1 [PARAM]`: VNR duration distribution parameter 1.
- `--vnr-arrival-dist [DIST]`: VNR arrival time distribution type.
- `--vnr-arrival-p1 [PARAM]`: VNR arrival time distribution parameter 1.

Refer to `config.toml` for a comprehensive list of configurable parameters and their descriptions.

---

## 📖 Citation and Papers

If you use this implementation in your research, please cite:

**S. Haeri** and **Lj. Trajkovic**,
[_"Virtual network embedding via Monte-Carlo tree search,"_](http://www.sfu.ca/~ljilja/papers/t-cybernetics-2016_web.pdf)
IEEE Transactions on Cybernetics, vol. 48, no. 2, pp. 510-521, Feb. 2018.

📜 **BibTeX**:

```bibtex
@article{haeri2018vne,
title={Virtual network embedding via Monte-Carlo tree search},
author={Haeri, S. and Trajkovic, Lj.},
journal={IEEE Transactions on Cybernetics},
volume={48},
number={2},
pages={510--521},
year={2018},
month={Feb},
doi={10.1109/TCYB.2017.2653244}

}
```

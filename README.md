# 🗓️ Smart Scheduler System — Pro Edition

> A multi-functional C++ desktop application built with **Qt6 (Qt5 compatible)** that unifies three fundamental computer science concepts — **Timetable Management**, **CPU Job Scheduling**, and **Expression Conversion** — into a single, polished native GUI application with a modern dark theme.

---

## ✨ Features

### 📅 Matrix Timetable
- Visual **Day × Time grid** (Monday–Friday, 9:00–16:00) rendered using `QTableWidget`
- Add schedule slots of three types: **Lecture**, **Lab**, and **Break**
- Each type is **colour-coded** for instant visual identification:
  - 🔵 Lecture → Blue (`#89b4fa`)
  - 🟣 Lab → Purple (`#cba6f7`)
  - 🟡 Break → Yellow (`#f9e2af`)
- **Graph-based Conflict Detection** — builds an adjacency matrix over all slots and flags any two entries sharing the same Day, Time, and Room simultaneously

### ⚙️ Job Scheduler
- Add CPU processes with **Priority Level** (High / Low / IO), **Process Name**, **Burst Time**, and **Arrival Time**
- **FCFS Algorithm** — processes jobs in arrival order, computes per-job Waiting Time and Turnaround Time
- **Priority Scheduling** — sorts jobs by priority level before execution and displays the ordered results
- Results displayed in a styled HTML dialog for clear readability

### 🔧 Logic Utilities — Expression Converter
- Converts mathematical **Infix expressions** to:
  - **Postfix** (Reverse Polish Notation) via the Shunting-Yard algorithm using a `std::stack`
  - **Prefix** (Polish Notation) via the reverse-string + modified postfix method
- Supports operators `+`, `-`, `*`, `/` with correct precedence and parenthesis handling
- Accepts alphanumeric operands (e.g., `A`, `B`, `C`, `X1`)
- Large monospace result display for instant readability

### 📖 Help / User Guide
- Built-in HTML-rendered guide explaining all three modules and the underlying algorithms

---

## 🛠️ Tech Stack

| Component | Technology |
|-----------|-----------|
| Language | C++17 |
| GUI Framework | Qt6 / Qt5 (auto-detected) |
| Build System | CMake 3.16+ |
| UI Components | QMainWindow, QTabWidget, QTableWidget, QFormLayout |
| Data Structures | `std::vector`, `std::stack` (STL) |
| Internationalization | Qt Linguist (`.ts` / `.qm`) |
| Theme | Catppuccin Mocha dark palette |

---

## 🏗️ Project Structure

```
SmartScheduler/
├── CMakeLists.txt              # CMake build configuration (Qt6/Qt5 dual support)
├── main.cpp                    # Application entry point — initializes QApplication
├── mainwindow.h                # MainWindow class declaration & UI element members
├── mainwindow.cpp              # All logic: UI setup, algorithms, Qt slots
└── SmartScheduler_en_IN.ts     # Qt Linguist translation file (en_IN locale)
```

### Key Classes & Structures

```cpp
struct ScheduleItem          // Base struct for polymorphic schedule data
struct TimeSlot              // Holds: type, day, start time, room name
struct Job                   // Holds: type, burst time, arrival time, priority
class MainWindow             // QMainWindow subclass — owns all UI + logic
```

---

## ⚙️ Algorithms Implemented

### 1. Graph Conflict Detection (Adjacency Matrix)
Builds an `n × n` adjacency matrix over all schedule slots. For every pair `(i, j)`, if both slots share the **same Day + Time + Room**, the edge `adj[i][j]` is marked and a conflict is reported.

```
Time Complexity:  O(n²)
Space Complexity: O(n²)
```

### 2. FCFS CPU Scheduling
Processes jobs in insertion (arrival) order. Computes:
- `wait = current_time - arrival_time`
- `turnaround = wait + burst_time`

### 3. Priority CPU Scheduling
Sorts the job list by priority value (1 = High, 2 = IO, 3 = Low) using `std::sort` with a lambda comparator, then processes in sorted order.

### 4. Infix → Postfix (Shunting-Yard)
Classic stack-based algorithm scanning left-to-right:
- Operands → append to output
- `(` → push to stack
- `)` → pop to output until `(`
- Operator → pop operators of `>=` precedence, then push

### 5. Infix → Prefix (Reverse-Postfix Method)
Three-step algorithm:
1. Reverse the infix string and swap `(` ↔ `)`
2. Apply the Shunting-Yard algorithm with **strict `>`** precedence (not `>=`)
3. Reverse the result to obtain the prefix expression

---

## 🚀 Getting Started

### Prerequisites

- **Qt 6.x** (recommended) or **Qt 5.15+**
- **CMake 3.16+**
- **C++17** compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- **Qt Creator** (recommended IDE) or any CMake-aware IDE

### Build Instructions

#### Using Qt Creator (Recommended)
1. Open **Qt Creator**
2. Click **File → Open File or Project**
3. Select `CMakeLists.txt`
4. Choose your Qt kit (Qt 6.x preferred)
5. Click **Build** (Ctrl+B) then **Run** (Ctrl+R)

#### Using Command Line

```bash
# Clone the repository
git clone https://github.com/your-username/SmartScheduler.git
cd SmartScheduler

# Configure the build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --parallel

# Run
./build/SmartScheduler          # Linux/macOS
build\SmartScheduler.exe        # Windows
```

> **Note:** Ensure `Qt6_DIR` or `Qt5_DIR` is set in your environment if CMake cannot locate Qt automatically.  
> Example: `cmake -B build -DQt6_DIR=/path/to/Qt/6.x.x/gcc_64/lib/cmake/Qt6`

---

## 📋 Pre-loaded Sample Data

The application launches with the following data already populated for demonstration:

**Timetable Slots:**
| Name | Type | Day | Time | Room |
|------|------|-----|------|------|
| Data Structures | Lecture | Monday | 9:00 | A101 |
| C++ Lab | Lab | Monday | 10:00 | A101 |
| Tech Training | Lecture | Tuesday | 11:00 | B202 |
| Lunch Break | Break | Wednesday | 12:00 | Cafeteria |

**CPU Jobs:**
| Name | Priority | Burst | Arrival |
|------|----------|-------|---------|
| ExamResult | High (1) | 3 ms | 0 ms |
| BackupData | Low (3) | 5 ms | 1 ms |
| PrintDocs | IO (2) | 2 ms | 2 ms |

---

## 🎨 Theme

The UI uses the **Catppuccin Mocha** dark colour palette applied globally via `QMainWindow::setStyleSheet()`:

| Element | Colour |
|---------|--------|
| Background | `#1e1e2e` |
| Surface | `#313244` |
| Active Tab | `#cba6f7` (Mauve) |
| Button | `#89b4fa` (Blue) |
| Table Header | `#f38ba8` (Red) |
| Text | `#cdd6f4` (Text) |

---

## 🌐 Internationalization

The project includes a Qt Linguist translation source file `SmartScheduler_en_IN.ts` for **English (India)** locale. To add a new language:

```bash
# Generate/update .ts file
lupdate SmartScheduler.pro -ts SmartScheduler_hi_IN.ts

# Translate using Qt Linguist GUI
linguist SmartScheduler_hi_IN.ts

# Release (compile to .qm binary)
lrelease SmartScheduler_hi_IN.ts
```

---

## 🔭 Possible Future Enhancements

- [ ] Persistent storage — save/load timetable and jobs from a JSON or SQLite file
- [ ] Undo/Redo support using `QUndoStack`
- [ ] Round Robin and SRTF scheduling algorithms
- [ ] Export timetable matrix to PDF via `QPrinter`
- [ ] Drag-and-drop slot reordering in the timetable grid
- [ ] Dark/Light theme toggle at runtime
- [ ] Weighted graph support for advanced conflict path detection

---

## 👨‍💻 Author
**Vishal Mehta**<br>
**Karan Singh**<br>
MCA — Data Science  
University Institute of Computing, Chandigarh University  

---

## 📄 License

This project is developed as part of the **Technical Training** coursework at Chandigarh University.  
For academic use only.

---

<div align="center">
  <sub>Built with ❤️ using C++17 and Qt6 | Catppuccin Mocha Theme</sub>
</div>

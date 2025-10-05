# 🌙 MoonBerry-Painter

**Pen Plotter Driver Library — Written in C (Open for You)**
> 🌱🖌️ “Draw the world in code and ink.” ♡⁠(⁠Ӧ⁠ｖ⁠Ӧ⁠｡⁠)

# Logo
![MoonBerry Painter](archanaberry/moonberrypainter.png)

---

## 🧩 Overview

**MoonBerry-Painter** is a lightweight and high-performance **C-based driver library** for controlling **pen plotter machines**.
It is designed with **real-time precision**, **low-latency GPIO control**, and **modular adaptability** — making it suitable for both embedded systems and desktop environments.

Whether you’re building your own drawing robot, testing motion control algorithms, or integrating plotting into your automation workflow — MoonBerry-Painter gives you the tools to **paint with hardware precision**.

---

## ✨ Features
✅ Successfully implement
🌱 Growing implement 
🌸 Customizable implement

| Feature                              | Description                                              | Status |
| :----------------------------------- | :------------------------------------------------------- | :----: |
| 🪶 **Pure C Implementation**         | No runtime overhead, just efficient C code.              |    ✅   |
| ⚙️ **Low-Latency GPIO Driver**       | Optimized for real-time pen plotting control.            |    ✅   |
| 🧠 **Configurable Axis System**      | Supports multiple stepper motor axes (X, Y, Z, etc.).    |    ✅   |
| 🎨 **Path & Curve Interpreter**      | Converts paths or vector data into motion commands.      |   🧪   |
| 🔌 **Teensy / Raspberry Pi Support** | Works with both microcontroller and SBC environments.    |    ✅   |
| 🧰 **Modular I/O Layer**             | Easily adapt to your own hardware interface.             |    ✅   |
| 🧾 **Logging & Debug Mode**          | Built-in serial and runtime logger for debugging.        |    ✅   |
| 🪴 **Open & Extendable**             | Fully open for customization, integration, and learning. |   🌸   |
|🖋️ **Svg Preview and Svg Render converting** | Fully compatibility convert vector format. | 🌱 |

---

## 🚀 Getting Started

### 🧱 Prerequisites

You’ll need:

* **C Compiler** (GCC / Clang / ABCC / etc.)
* **Make** or **CMake** (optional)
* Access to GPIO (for Raspberry Pi / Teensy/Arduino)
* Basic knowledge of C and embedded I/O
* Hobbyst custom or DIY pen plotter hardware printing math with super highly precision curve and shapes

---

### ⚙️ Building from Source

```bash
# Clone the repository
git clone https://github.com/archanaberry/MoonBerry-Painter.git
cd MoonBerry-Painter

# Build using GCC
gcc -o moonberry main.c src/*.c -Iinclude -lm
```

If you use CMake:

```bash
mkdir build && cd build
cmake ..
make
```

---

### 🧩 Example Usage

```c
#include "moonberry.h"

int main(void) {
    mb_init();
    mb_set_pen(UP);

    mb_move_to(0, 0);
    mb_set_pen(DOWN);
    mb_draw_line(0, 0, 100, 50);

    mb_set_pen(UP);
    mb_shutdown();
    return 0;
}
```

```mpn
; MoonBerry Painter Script
; Converted from C example

INIT                ; set up system (equivalent to mb_init)
PEN UP              ; mb_set_pen(UP)

MOVE X0 Y0          ; mb_move_to(0, 0)
PEN DOWN            ; mb_set_pen(DOWN)
DRAW X100 Y50       ; mb_draw_line(0, 0, 100, 50)

PEN UP              ; mb_set_pen(UP)
SHUTDOWN            ; mb_shutdown
END
```

---

## 🧬 Architecture

```
MoonBerry-Painter
│
├── core/              → Motion control & GPIO abstraction
├── backend/           → Hardware drivers (Teensy, Raspberry Pi, etc.)
├── driver/           → OS Driver Hardware for Linux Windows or Microcontroller OS.
├── parser/            → Command & vector interpreter
├── include/           → Public headers
├── scripts/           → For etc, or python plugin
└── examples/          → Usage demos & test code
```

Each module is isolated and can be replaced with your own driver backend or logic layer.

---

## 🧠 Design Philosophy

MoonBerry-Painter follows the **“Open Core, Modular Edge”** principle:

* The **core** provides stable, clean abstractions for motion control and I/O.
* The **edges** (drivers, parsers, extensions) are replaceable — you can make it yours.
* Code simplicity and readability are prioritized over over-engineering.

---

## 🧪 Hardware Compatibility

| Platform       | GPIO Type                  | Supported | Notes                     |
|----------------|----------------------------|------------|----------------------------|
| Raspberry Pi   | BCM / sysfs / memory-mapped | ✅         | Recommended for prototype  |
| Teensy         | Native I/O                 | ✅         | Best latency               |
| Arduino        | via serial link            | ⚙️         | Experimental               |
| Custom Board   | Custom driver interface    | 🧩         | Extend with your own driver |

---

## 🛠️ Configuration

You can configure machine parameters via a simple header file or runtime config:

```c
#define MB_AXIS_X_STEPS_PER_MM 80
#define MB_AXIS_Y_STEPS_PER_MM 80
#define MB_FEEDRATE_DEFAULT   1200
#define MB_PEN_DOWN_DELAY_MS  50
```

```mpn
; MoonBerry Painter runtime setup
CONFIG AXIS X.STEP_PER_MM 80
CONFIG AXIS Y.STEP_PER_MM 80
CONFIG FEEDRATE 1200
CONFIG PEN.DELAY 50

; Ready to draw
INIT
```

Or load via configuration file (optional support planned).

---

## 🧑‍💻 Development

Contributions are welcome!
Feel free to fork, modify, or extend modules such as:

* **`backend/teensy_gpio.c`** for microcontroller support
* **`path.c`** for vector data decoding

---

## 📜 License

**ABPL License**
This project is open-source and free to use in any personal with berry personality or commercial project (With berry permission) — just give credit if possible. 🌱🌙

---

## 💬 Contact & Community

* **Author:** Archana Berry
* **Email:** [archanaberry101originally@gmail.com](mailto:archanaberry101originally@gmail.com)
* **Project Page:** [https://github.com/archanaberry/MoonBerry-Painter](https://github.com/archanaberry/MoonBerry-Painter)

> 🌸 *MoonBerry-Painter — where hardware meets art.*

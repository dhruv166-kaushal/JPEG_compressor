# JPEG Compressor

A lightweight image processing tool that demonstrates how to read, process, and compress JPEG images. The core logic is written in C++ using the `stb_image` libraries for efficient, dependency-free image manipulation, alongside a Python script for additional encoding operations.

---

## 📁 Repository Structure

* **`main.cpp`**: The core C++ source code responsible for loading the image, performing the compression/processing, and saving the output.
* **`encoder.py`**: Python script handling encoding tasks and utility functions for the compression pipeline.
* **`stb_image.h` & `stb_image_write.h`**: Single-header libraries by Sean Barrett used to read and write various image formats in C/C++ without external dependencies.
* **Executables**: Pre-compiled Windows binaries (`a.exe`, `compressor.exe`).
* **Sample Images**:
  * `input.jpg`: The original, uncompressed source image.
  * `output_compressed.jpg`: Standard compressed output.
  * `output_50pct.jpg`: Image demonstrating compression at 50% quality.
  * `output_color.jpg` & `output_pro.jpg`: Additional processed output samples.

## ⚙️ Prerequisites

To compile and run this project from source, you will need:
* A C++ compiler (e.g., GCC/G++, MinGW, or MSVC)
* Python 3.x (to run the encoder script)

## 🚀 Build Instructions

Because the project uses the header-only `stb` libraries, compiling is straightforward. You do not need to link any bulky external libraries.

**Using G++ (Linux/macOS/MinGW):**
```bash
g++ main.cpp -o compressor.exe

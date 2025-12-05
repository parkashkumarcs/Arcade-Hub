# 🎮 Gaming Hub - Professional C++ Arcade

A professional graphical arcade game built with **C++** and **raylib** graphics library. Features 5 interactive mini-games with modern UI, animations, and smooth gameplay.

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![raylib](https://img.shields.io/badge/raylib-black?style=for-the-badge&logo=raylib&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

---

## ✨ Features

- 🎨 **Professional GUI** - Beautiful graphics with animated particle backgrounds
- 🎮 **5 Complete Games** - Various genres from puzzle to action
- 🖱️ **Interactive Controls** - Mouse and keyboard support
- 💾 **Score Tracking** - Scores saved to files
- ⚡ **60 FPS Gameplay** - Smooth animations and responsive controls
- 📦 **Single File** - All games in one C++ source file

---

## 🎯 Games Included

| # | Game | Type | Description |
|---|------|------|-------------|
| 1 | **Tic-Tac-Toe** | Strategy | Classic 2-player game on a 3x3 grid |
| 2 | **Space Survivor** | Action | Dodge falling asteroids in space |
| 3 | **Break The Code** | Puzzle | Mastermind-style code guessing game |
| 4 | **Escape Zombie Land** | Survival | Navigate grid to escape the zombie |
| 5 | **Swap Cards** | Memory | Find matching pairs of cards |

---

## 🚀 Quick Start

### Prerequisites
- **MinGW g++** compiler ([Download](https://winlibs.com/))
- **raylib** library ([Download](https://www.raylib.com/))

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/Gaming-Hub.git
   cd Gaming-Hub
   ```

2. **Download raylib**
   ```bash
   # Download and extract raylib-5.0_win64_mingw-w64.zip to project folder
   ```

3. **Compile**
   ```bash
   g++ -o GamingHub.exe GamingHub.cpp -I "raylib-5.0_win64_mingw-w64/include" -L "raylib-5.0_win64_mingw-w64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
   ```

4. **Run**
   ```bash
   ./GamingHub.exe
   ```

> 💡 **Quick Method:** Double-click `compile.bat` to auto-compile and run!

---

## 🎮 Controls

| Action | Key/Button |
|--------|------------|
| Select Game | Mouse Click |
| Return to Menu | `M` or `ESC` |
| Restart Game | `SPACE` |
| Move (Games) | `WASD` or Arrow Keys |
| Exit | `ESC` on main menu |

---

## 📁 Project Structure

```
Gaming-Hub/
├── GamingHub.cpp              # Main source code (~1000 lines)
├── GamingHub.exe              # Compiled executable
├── compile.bat                # Compilation script
├── README.md                  # This file
├── GamingHub_Documentation.pdf # Full documentation
├── SETUP_GUIDE.txt            # Installation guide
└── raylib-5.0_win64_mingw-w64/ # raylib library
    ├── include/
    └── lib/
```

---

## 💻 C++ Concepts Demonstrated

- ✅ Object-Oriented Programming (Classes)
- ✅ Namespaces
- ✅ Vectors (STL)
- ✅ File I/O (fstream)
- ✅ Enums
- ✅ Control Structures
- ✅ Functions & Modularity
- ✅ Game Loop Pattern

---

## 📚 Documentation

See [GamingHub_Documentation.pdf](GamingHub_Documentation.pdf) for complete documentation including:
- Detailed game descriptions
- Code structure explanation
- Setup instructions
- Code examples

---

## 🛠️ Built With

- **C++** - Programming Language
- **[raylib](https://www.raylib.com/)** - Simple game programming library
- **OpenGL** - Graphics rendering
- **MinGW** - Compiler

---

## 📄 License

This project is open source and available for educational purposes.

---

## 🙏 Acknowledgments

- [raylib](https://www.raylib.com/) - For the amazing graphics library
- First Semester C++ Programming Course

---

<p align="center">
  <b>⭐ Star this repo if you found it helpful! ⭐</b>
</p>


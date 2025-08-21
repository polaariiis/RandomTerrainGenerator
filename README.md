
# Random Terrain Generator 🌄

This project is a simple **C++ terrain generator** built with **SFML**.  
It creates randomized 2D landscapes, perfect as a starting point for game development, simulations, or procedural generation experiments.

---

## 🚀 Features
- **Randomized Terrain Generation** → every run produces unique terrain, making it useful for prototyping procedural worlds.  
- **SFML Graphics Rendering** → terrain is displayed in a simple graphical window using [SFML](https://www.sfml-dev.org/).  
- **Font Rendering Support** → load and display text using fonts stored in the `fonts/` folder.  
- **Lightweight & Extendable** → the codebase is small and easy to modify, so you can tweak height maps, randomness, or even add colors and player movement.

---

## 📂 Project Structure
```

.
├── RandomTerrainGenerator.cpp   # Main source file
├── fonts/                       # Fonts used for rendering
├── sideproject1.sln             # Visual Studio solution
├── sideproject1.vcxproj         # Visual Studio project
└── .gitignore                   # Git ignore rules

````

---

## 🛠️ Requirements
- **C++17 or later**
- [SFML 2.5+](https://www.sfml-dev.org/download.php)
- Visual Studio 2019/2022 (recommended) or another C++ compiler

---

## 📥 Installing SFML

### Windows (Visual Studio)
1. Download SFML from: [SFML Downloads](https://www.sfml-dev.org/download.php).  
   Choose the version matching your Visual Studio compiler (e.g., `Visual C++ 15 (2017)` or `Visual C++ 16 (2019)`).
2. Extract the SFML folder somewhere on your PC.  
3. In Visual Studio:  
   - Open **Project Properties** → **VC++ Directories**.  
   - Add `SFML/include` to **Include Directories**.  
   - Add `SFML/lib` to **Library Directories**.  
   - Link the required `.lib` files (e.g., `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib`) under **Linker > Input**.  
4. Copy the required `.dll` files from `SFML/bin` into your build output folder (next to your `.exe`).

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install libsfml-dev
````

### macOS (Homebrew)

```bash
brew install sfml
```

---

## ⚙️ Build & Run

### Using Visual Studio

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/sideproject1.git
   cd sideproject1
   ```
2. Open `sideproject1.sln` in Visual Studio.
3. Make sure SFML is set up (include & lib paths configured).
4. Build and run the project.

### Using g++ (Linux / MinGW)

If you have SFML installed globally, run:

```bash
g++ RandomTerrainGenerator.cpp -o terrain -lsfml-graphics -lsfml-window -lsfml-system
./terrain
```

---

## 🎮 Usage

* Run the program → a window will open displaying randomly generated terrain.
* Fonts are loaded from the `fonts/` directory.
* To experiment, open `RandomTerrainGenerator.cpp` and adjust generation parameters (e.g., terrain height, noise randomness, or drawing styles).

---

## 🖼️ Screenshots

Here’s a preview of the generated terrain:
<img width="798" height="650" alt="{AAF9A84D-6FE7-491C-BEF9-D58D4D86269A}" src="https://github.com/user-attachments/assets/ce5f68f1-4eb0-4e4f-b19f-49c1c7ef18fc" />
<img width="798" height="641" alt="{F6CA697B-E670-4E2F-A64E-1ECEABD4FDB8}" src="https://github.com/user-attachments/assets/87a05e5a-732a-482b-ae22-88c9d00efa78" />
<img width="797" height="634" alt="{26A9ECD3-28BF-4BDB-BA23-C62106EE709F}" src="https://github.com/user-attachments/assets/f43b13b8-4fe7-49b2-a137-2899406ea18f" />


---

## 📌 Notes

* The `dependencies/` folder is ignored in Git (you’ll need to install SFML separately).
* If using Visual Studio, place the required **SFML .dll files** next to the compiled `.exe` before running.

---

## 🤝 Contributing

Feel free to fork this repository and experiment with your own terrain generation ideas. PRs are welcome!

---

## 📜 License

This project is licensed under the **Apache License 2.0**.
You may obtain a copy of the License at:

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.



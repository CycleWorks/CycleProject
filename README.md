# CycleProject

\> This project is licensed under the **[GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.en.html)**

## Building the Project

System dependencies:
- C++23 compiler
- Python3
- CMake 3.16+

To resolve python dependencies, run the following commands in your terminal:
```bash
pip install pathlib
pip install keyboard
```

To build Cycle, run the following command in your terminal:

```bash
python3 build.py
```

\> This should run the CMake configuration and build the project
<br>
\> All settings entered on first build will be saved for future builds
<br>
\> The final compiled executable should be located in ```./bin/```

To reset build files and settings, run the following command in your terminal:

```bash
python3 reset.py
```

For IDE linting, CMake exports compile commands in ```./build/``` after first build (at configure time)

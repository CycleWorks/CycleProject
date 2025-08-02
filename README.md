# CycleProject

The **Cycle programming language** is extremely hypothetical and building a working compiler for it might not even be possible.

\> This project is licensed under the **[GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.en.html)**
<br>
\> This project uses LLVM, which uses **[Apache 2.0 w/ LLVM Exception](https://llvm.org/LICENSE.txt)**
<br>
\> This project uses Catch2, which uses **[Boost Software License 1.0](https://www.boost.org/LICENSE_1_0.txt)**

---

## Building the Project

Manual dependencies:
- Compiler supporting C++23
- Python3
- CMake 3.16+
- LLVM 18.1.8 (Static linking with dynamic C libraries)
- Clangd for linting (optional)

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

---

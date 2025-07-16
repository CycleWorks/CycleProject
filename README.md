# CycleProject

The **Cycle programming language** is extremely hypothetical and building a working compiler for it might not even be possible.

\> This project is licensed under the **[GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.en.html)**, see **[LICENSE](./LICENSE)** for full details.
<br>
\> This project links against LLVM, which uses **[Apache 2.0 w/ LLVM Exception](https://llvm.org/LICENSE.txt)**, See **[LICENSE.LLVM](./LICENSE.LLVM)** for details.

---

## Building the Project

Dependencies:
- Python3
- Compiler supporting C++23
- CMake 3.16+
- Any CMake generator
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

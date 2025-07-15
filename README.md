# CycleProject

The **Cycle programming language** is extremely hypothetical and building a working compiler for it might not even be possible.

\> This project is licensed under the **[GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.en.html)**, see **[LICENSE](./LICENSE)** for full details.
<br>
\> This project links against LLVM, which uses **[Apache 2.0 w/ LLVM Exception](https://llvm.org/LICENSE.txt)**, See **[LICENSE.LLVM](./LICENSE.LLVM)** for details.

---

## Building the Project

Dependencies:
- Clang or GCC
- CMake + generator
- Clangd for linting (optional)
- LLVM 18.1.8 (MSVCRT)

About LLVM:
- LLVM needs to be installed on your system. Add your LLVM installation's ```LLVMConfig.cmake``` to your environment search path so CMake can find it
- Cycle relies on LLVM's dynamically linked libraries. Make sure the libraries are in the executable's folder before running Cycle

To build Cycle, run the following commands in your terminal:

```bash
cmake -G <GENERATOR> -DCMAKE_BUILD_TYPE=<BUILD_TYPE> -DCMAKE_CXX_COMPILER=<COMPILER> -B build
cmake --build build --parallel
```

- Where ```<GENERATOR>``` specifies the backend build system (Ex: ```Ninja```, ```Unix Makefiles```, ```Visual Studio```, etc...)
- Where ```<BUILD_TYPE>``` can be either ```Release```, ```Debug```, ```RelWithDebInfo``` or ```MinSizeRel```
- Where ```<COMPILER>``` can be either ```clang++``` or ```g++```, other compilers are NOT guaranteed to work

\> Compilers other than the ones mentionned are NOT guarenteed to work with this project
<br>
\> The final compiled executable should be located in ```./bin/```

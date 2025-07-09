# CycleProject

The **Cycle programming language** is extremely hypothetical and building a working compiler for it might not even be possible.

This project is licensed under the **[GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.en.html)**, see **[LICENSE](./LICENSE)** for full details.

---

## Building the Project

To build CycleProject, run the following commands in your terminal:

```bash
cmake -G <GENERATOR> -DCMAKE_BUILD_TYPE=<BUILD_TYPE> -B build
cmake --build build --parallel
```

- Where <GENERATOR> specifies the backend build system (Ex: ```Ninja```, ```Unix Makefiles```, ```Visual Studio```, etc...)
- Where <BUILD_TYPE> can be either ```Release``` or ```Debug```

The final compiled executable should be located in ```./bin/```

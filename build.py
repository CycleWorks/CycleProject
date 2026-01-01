from pathlib import Path
import sys
import json
import subprocess
import re
from typing import TypedDict, List

# ---- Constants ----

settings_file = Path("build_settings.json")
required_cmake_version: tuple[int, int, int] = (3, 16, 0)

# ---- Types ----

class BuildSettings(TypedDict):
    generator: str
    compiler: str
    build_type: str

# ---- Environment helpers ----

def check_cmake_installed() -> None:
    try:
        subprocess.run(
            ["cmake", "--version"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            check=True,
        )
    except FileNotFoundError:
        print("[ERROR]: CMake is not installed or not found in PATH.")
        sys.exit(-1)
    except subprocess.CalledProcessError:
        print("[ERROR]: CMake is installed but returned an error.")
        sys.exit(-1)

def check_cmake_version() -> None:
    try:
        output: str = subprocess.check_output(["cmake", "--version"], text=True)
        match = re.search(r"cmake version (\d+)\.(\d+)\.(\d+)", output)
        if match:
            cmake_version = tuple(map(int, match.groups()))
            if cmake_version < required_cmake_version:
                print("[ERROR]: CMake version must be 3.16+")
                sys.exit(-1)
        else:
            print("[ERROR]: Unable to determine CMake version.")
            sys.exit(-1)
    except Exception as error:
        print(f"[ERROR]: Unable to determine CMake version: {error}")
        sys.exit(-1)

def get_valid_generators() -> List[str]:
    try:
        output: str = subprocess.check_output(
            ["cmake", "-E", "capabilities"], text=True
        )
        data: dict = json.loads(output)
        return [gen["name"] for gen in data.get("generators", [])]
    except Exception as error:
        print(f"[ERROR]: Unable to fetch CMake generator options: {error}")
        sys.exit(-1)

# ---- Validate Environment ----

check_cmake_installed()
check_cmake_version()

valid_generators: List[str] = get_valid_generators()
valid_build_types: List[str] = ["Release", "Debug", "RelWithDebInfo", "MinSizeRel"]
valid_regular_compilers: List[str] = ["clang", "gcc"]
valid_MSVC_compilers: List[str] = ["cl", "clang-cl"]
valid_compilers: List[str] = valid_regular_compilers + valid_MSVC_compilers

# ---- Build functions ----

def prompt_user_for_settings() -> BuildSettings:
    print("Please enter your build settings:")

    print("\nGenerator options:")
    print("\n".join(f"- {gen}" for gen in valid_generators))
    while True:
        generator = input("\nEnter generator: ").strip()
        if generator in valid_generators:
            break
        print("Invalid generator, please try again")

    print("\nC++ compiler options:")
    print("\n".join(f"- {comp}" for comp in valid_compilers))
    while True:
        compiler = input("\nEnter C++ Compiler: ").strip()
        if compiler in valid_compilers:
            break
        print("Invalid compiler, please try again")

    print("\nBuild type options:")
    print("\n".join(f"- {bt}" for bt in valid_build_types))
    while True:
        build_type = input("\nEnter build Type: ").strip()
        if build_type in valid_build_types:
            break
        print("Invalid build type, please try again")

    if "Visual Studio" in generator and compiler not in valid_MSVC_compilers:
        print("[ERROR]: The Visual Studio generator requires an MSVC-like compiler")
        sys.exit(-1)

    settings: BuildSettings = {
        "generator": generator,
        "compiler": compiler,
        "build_type": build_type,
    }

    with settings_file.open("w") as f:
        json.dump(settings, f, indent=4)

    return settings

def read_settings() -> BuildSettings:
    with settings_file.open("r") as f:
        data: BuildSettings = json.load(f)
        return data

def run_cmake_build(settings: BuildSettings) -> None:
    build_dir = "build"

    configure_cmd = [
        "cmake",
        "-G", settings["generator"],
        f"-DCMAKE_BUILD_TYPE={settings['build_type']}",
        f"-DCMAKE_CXX_COMPILER={settings['compiler']}",
        "-B", build_dir,
    ]

    build_cmd = [
        "cmake",
        "--build", build_dir,
        "--parallel",
    ]

    print("\nRunning CMake configuration:")
    print("> " + " ".join(configure_cmd))
    try:
        subprocess.run(configure_cmd, check=True)
    except Exception as error:
        print(f"[ERROR]: Failed when running CMake configuration: {error}")
        sys.exit(-1)

    print("\nBuilding the project:")
    print("> " + " ".join(build_cmd))
    try:
        subprocess.run(build_cmd, check=True)
    except Exception as error:
        print(f"[ERROR]: Failed when building the project: {error}")
        sys.exit(-1)

# ---- Main function ----

def main() -> None:
    if not settings_file.exists():
        settings = prompt_user_for_settings()
    else:
        settings = read_settings()

    run_cmake_build(settings)

if __name__ == "__main__":
    main()

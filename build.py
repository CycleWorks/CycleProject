from pathlib import Path
import sys
import json
import subprocess
import re

# ---- Constants ----

settings_file = Path("build_settings.json")
required_cmake_version = (3, 16, 0)

# ---- Environment helpers ----

def check_cmake_installed():
    try:
        subprocess.run(["cmake", "--version"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    except FileNotFoundError:
        print("[ERROR]: CMake is not installed or not found in PATH.")
        sys.exit()
    except subprocess.CalledProcessError:
        print("[ERROR]: CMake is installed but returned an error.")
        sys.exit()

def check_cmake_version():
    try:
        output = subprocess.check_output(["cmake", "--version"], text=True)
        match = re.search(r"cmake version (\d+)\.(\d+)\.(\d+)", output)
        if match:
            cmake_version = tuple(map(int, match.groups()))
            if cmake_version < required_cmake_version:
                print("[ERROR]: CMake version must be 3.16+")
                sys.exit()
        else:
            print("[ERROR]: Unable to determine CMake version.")
            sys.exit()
    except Exception as error:
        print(f"[ERROR]: Unable to determine CMake version: {error}")
        sys.exit()

def get_valid_generators():
    try:
        output = subprocess.check_output(["cmake", "-E", "capabilities"], text=True)
        data = json.loads(output)
        return [gen["name"] for gen in data.get("generators", [])]
    except Exception as error:
        print(f"[ERROR]: Unable to fetch CMake generator options: {error}")
        sys.exit()

# ---- Validate Environment ----

check_cmake_installed()
check_cmake_version()

valid_generators = get_valid_generators()
valid_build_types = ["Release", "Debug", "RelWithDebInfo", "MinSizeRel"]
valid_regular_compilers = ["clang", "gcc"]
valid_MSVC_compilers = ["cl", "clang-cl"]
valid_compilers = valid_regular_compilers + valid_MSVC_compilers

# ---- Build functions ----

def prompt_user_for_settings():
    print("Please enter your build settings:")

    print("\nGenerator options:")
    print("\n".join(f"- {gen}" for gen in valid_generators))
    while True:
        generator = input(f"\nEnter generator: ").strip()
        if generator in valid_generators:
            break
        print("Invalid generator, please try again")

    print("\nBuild type options:")
    print("\n".join(f"- {bt}" for bt in valid_build_types))
    while True:
        build_type = input(f"\nEnter build Type: ").strip()
        if build_type in valid_build_types:
            break
        print("Invalid build type, please try again")

    print("\nC++ compiler options:")
    print("\n".join(f"- {comp}" for comp in valid_compilers))
    while True:
        compiler = input(f"\nEnter C++ Compiler: ").strip()
        if compiler in valid_compilers:
            break
        print(f"Invalid compiler, please try again")
    
    if "Visual Studio" in generator and compiler not in valid_MSVC_compilers:
        print(f"[ERROR]: The Visual Studio generator requires an MSVC-like compiler")
        sys.exit()

    settings = {
        "generator": generator,
        "build_type": build_type,
        "compiler": compiler
    }
    with open(settings_file, "w") as f:
        json.dump(settings, f, indent=4)
    return settings

def read_settings():
    with open(settings_file, "r") as f:
        return json.load(f)

def run_cmake_build(settings):
    build_dir = "build"

    configure_cmd = [
        "cmake",
        "-G", settings["generator"],
        f"-DCMAKE_BUILD_TYPE={settings['build_type']}",
        f"-DCMAKE_CXX_COMPILER={settings['compiler']}",
        "-B", build_dir
    ]

    build_cmd = [
        "cmake",
        "--build", build_dir,
        "--parallel"
    ]

    print("\nRunning CMake configuration:")
    print("> " + " ".join(configure_cmd))
    try:
        subprocess.run(configure_cmd, check=True)
    except Exception as error:
        print(f"[ERROR]: Failed when running CMake configuration: {error}")
        sys.exit()

    print("\nBuilding the project:")
    print("> " + " ".join(build_cmd))
    try:
        subprocess.run(build_cmd, check=True)
    except Exception as error:
        print(f"[ERROR]: Failed when building the project: {error}")
        sys.exit()

# ---- Main function ----

def main():
    if not settings_file.exists():
        settings = prompt_user_for_settings()
    else:
        settings = read_settings()

    run_cmake_build(settings)

if __name__ == "__main__":
    main()

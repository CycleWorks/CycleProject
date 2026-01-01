from pathlib import Path
import sys
import shutil

# ---- Helpers ----

def delete_file(file_path: Path) -> None:
    if file_path.is_file():
        try:
            file_path.unlink()
        except:
            print(f"Couldn't delete file \"{file_path}\"")

def empty_folder_contents(folder_path: Path) -> None:
    if folder_path.is_dir():
        for child in folder_path.iterdir():
            try:
                if child.is_file() or child.is_symlink():
                    child.unlink()
                elif child.is_dir():
                    shutil.rmtree(child)
            except:
                print(f"Couldn't delete entry at path \"{child}\"")

# ---- Main function ----

def main() -> None:
    this_script_path = Path(__file__).resolve().parent
    build_settings_file_path = this_script_path / "build_settings.json"
    build_folder_path = this_script_path / "build"
    bin_folder_path = this_script_path / "bin"

    print(f"\nDeleting file: \"{build_settings_file_path}\"")
    print(f"Deleting all files in: \"{build_folder_path}\"")
    print(f"Deleting all files in: \"{bin_folder_path}\"\n")

    while True:
        answer = input("Would you like to proceed? [Y/N]: ")
        if answer.upper() == "Y":
            break
        if answer.upper() == "N":
            print("Deletion aborted")
            sys.exit(-1)
        print("Invalid answer")

    delete_file(build_settings_file_path)
    empty_folder_contents(build_folder_path)
    empty_folder_contents(bin_folder_path)
    print("Build files deleted\n")

if __name__ == "__main__":
    main()

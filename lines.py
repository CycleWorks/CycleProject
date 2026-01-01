from pathlib import Path
import sys

# ---- helpers ----

def count_lines_in_file(file_path: Path) -> int:
    try:
        with file_path.open('r', encoding='utf-8', errors='ignore') as file:
            return sum(1 for _ in file)
    except Exception as error:
        print(f"Error reading \"{file_path}\": {error}")
        return 0

def count_lines_in_directory(dir_path: Path) -> tuple[int, dict[str, int]]:
    if not dir_path.exists() or not dir_path.is_dir():
        print(f"Error reading \"{dir_path}\": Folder doesn't exists")
        sys.exit(-1)

    valid_extensions = {'.cpp', '.cc', '.c', '.hpp', '.hh', '.h', '.tpp'}
    total_lines = 0
    file_counts = {}

    for file_path in dir_path.rglob('*'):
        if file_path.suffix.lower() not in valid_extensions or not file_path.is_file():
            continue

        line_count = count_lines_in_file(file_path)
        file_counts[str(file_path)] = line_count
        total_lines += line_count
    return total_lines, file_counts

# ---- Main function ----

def main() -> None:
    dirs = ["project"]
    total_lines = 0

    for dir_name in dirs:
        dir_path = Path(dir_name)
        total_lines_dir, file_counts_dir = count_lines_in_directory(dir_path)
        total_lines += total_lines_dir
        for file_path, line_count in file_counts_dir.items():
            print(f"{file_path}: {line_count} lines")
    print(f"Total lines: {total_lines}")

if __name__ == "__main__":
    main()

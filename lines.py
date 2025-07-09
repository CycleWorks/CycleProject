import os

def count_lines_in_file(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
            return sum(1 for _ in file)
    except Exception as error:
        print(f"Error reading {file_path}: {error}")
        return 0

def count_lines_in_directory(directory):
    total_lines = 0
    file_counts = {}

    for root, _, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            line_count = count_lines_in_file(file_path)
            file_counts[file_path] = line_count
            total_lines += line_count

    return total_lines, file_counts

if __name__ == "__main__":
    dirs = ["include", "src"]
    total_lines = 0

    for dir in dirs:
        total_lines_dir, file_counts_dir = count_lines_in_directory(dir)
        total_lines += total_lines_dir
        for file_path, line_count in file_counts_dir.items():
            print(f"{file_path}: {line_count} lines")

    print(f"Total lines: {total_lines}")

import os

def print_tree(start_path='.', prefix=''):
    """Recursively prints a tree view of folders and files."""
    # Get all entries in the directory
    entries = sorted(os.listdir(start_path))
    entries_count = len(entries)

    for index, entry in enumerate(entries):
        path = os.path.join(start_path, entry)
        connector = '└── ' if index == entries_count - 1 else '├── '

        print(prefix + connector + entry)

        if os.path.isdir(path):
            extension = '    ' if index == entries_count - 1 else '│   '
            print_tree(path, prefix + extension)

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Print a directory tree.")
    parser.add_argument("path", nargs="?", default=".", help="Directory to display (default: current directory)")
    args = parser.parse_args()

    print(f"\nDirectory tree for: {os.path.abspath(args.path)}\n")
    print_tree(args.path)

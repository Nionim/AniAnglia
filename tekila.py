from __future__ import annotations
from pathlib import Path

SOURCE_DIR = Path(__file__).resolve().parent / "."
OUTPUT_FILE = Path(__file__).resolve().parent / "tekila.txt"

USER_PROMPT = "Заголовки опущены"


def collect_files(source_dir: Path, output_file: Path | None = None) -> list[Path]:
    if not source_dir.exists():
        raise FileNotFoundError(f"Cannot found: {source_dir}")

    all_files = []
    for item in source_dir.rglob("*"):
        if item.is_file():
            if output_file and item.samefile(output_file):
                continue
            all_files.append(item)

    return sorted(set(all_files))


def write_combined_file(file_paths: list[Path], output_path: Path, prompt: str) -> None:
    with open(output_path, "w", encoding="utf-8") as out_f:
        for file_path in file_paths:
            rel_path = file_path.relative_to(SOURCE_DIR)
            out_f.write(f"{rel_path}\n")
        out_f.write(prompt + "\n")


def main() -> None:
    files = collect_files(SOURCE_DIR, OUTPUT_FILE)

    if not files:
        print("No files found")
        return

    print(f"Files found: {len(files)}")
    write_combined_file(files, OUTPUT_FILE, USER_PROMPT)
    print(f"Paths written to: {OUTPUT_FILE}")


if __name__ == "__main__":
    main()
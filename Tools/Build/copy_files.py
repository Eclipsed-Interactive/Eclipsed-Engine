import os
from pathlib import Path
import shutil

def copy_files(source, destination, suffixes):
    folder = Path(source)
    destination = Path(destination)

    destination.mkdir(parents=True, exist_ok=True)

    for file in folder.rglob("*"):
        if file.suffix in suffixes:
            destFile = destination / file.name

            if destFile.exists():
                src_stat = file.stat()
                dst_stat = destFile.stat()

                if (src_stat.st_size == dst_stat.st_size and
                        src_stat.st_mtime <= dst_stat.st_mtime):
                    continue

            shutil.copy2(file, destFile)
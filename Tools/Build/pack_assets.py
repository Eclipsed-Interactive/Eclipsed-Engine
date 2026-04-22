import struct

from enum import Enum

from dataclasses import dataclass
from pathlib import Path

from PIL import Image
import numpy as np

from typing import BinaryIO

def pack_asset_datas(assets, out: BinaryIO):
    AssetIndexSize = 24
    for asset in assets:
         with open(asset, "rb") as f:
            f.seek(AssetIndexSize)
            bytes_data = f.read()
            out.write(bytes_data)


def pack_asset_headers(assets: list, out: BinaryIO):
    AssetIndexSize = 24
    out.write(struct.pack(">I", len(assets)))
    for asset in assets:
         with open(asset, "rb") as f:
            bytes_data = f.read(AssetIndexSize)
            out.write(bytes_data)
            
    
def pack_assets(source, destination, split: bool = False):
    folder = Path(source)
    assets = []

    for file in folder.rglob("*"):
        if file.is_file():
            assets.append(str(file))
            
    output_file = Path(destination) / "packed_assets.bin"
    output_file.parent.mkdir(parents=True, exist_ok=True)
    
    with open(output_file, "wb") as out:
        pack_asset_headers(assets, out)
        pack_asset_datas(assets, out)
            
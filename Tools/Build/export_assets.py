import struct
from enum import Enum
from dataclasses import dataclass
from pathlib import Path
from PIL import Image
import numpy as np
from common.hash import fnv1a_64

@dataclass
class AssetIndex:
    asset_id: int
    file_id: int
    size: int
    offset: int
    pixels: np.ndarray
    img: Image.Image
    
class AssetType(Enum):
    Texture = "texture"
    Material = "material"

textureSuffixes = {".png"}
materialSuffixes = {".mat"}

def export_textures(assetList, destinationFolder):
    assetIndexList = []
    offsetCounter: int = len(assetList) * 24 + 4
    for texture in assetList:
        img = Image.open(texture).convert("RGBA")
        pixels = np.array(img)
        
        id = fnv1a_64(str(texture).encode("utf-8"))
        dirId = str(id)[:2]
        dirPath = destinationFolder + "/" + dirId
        Path(dirPath).mkdir(parents=True, exist_ok=True)
        
        with open(dirPath + "/" + str(id) + ".bin", "wb") as out:
            out.write(struct.pack(">III", *pixels.shape))
            out.write(pixels.tobytes())

def get_asset_key(assetPath: Path):
    if assetPath.suffix in textureSuffixes:
        return AssetType.Texture
    elif assetPath.suffix in materialSuffixes:
        return AssetType.Material

def export_assets_list(key, assetList, destinationFolder):
    if key == AssetType.Texture:
        export_textures(assetList, destinationFolder)

def gather_assets(file: Path, assets: dict):
    assets.setdefault(get_asset_key(file), []).append(file)
    
def export_assets(source, destinationFolder):
    assets: dict = {
        AssetType.Texture: [],
        AssetType.Material: []
    }
    
    folder = Path(source)
    for file in folder.rglob("*"):
        if file.suffix != ".meta":
            gather_assets(file, assets)
            
    for key, assetList in assets.items():
        export_assets_list(key, assetList, destinationFolder)

def export_asset(assetPath, destinationFolder):

    if isinstance(assetPath, str):
        assetPath = Path(assetPath)

    export_assets_list(get_asset_key(assetPath), [assetPath], destinationFolder)
  
if __name__ == "__main__":
    # export_asset("C:/MyFiles/Projects/Eclipsed-Engine/Assets/Default/Textures/Default_Texture.png", "C:/MyFiles/Projects/Eclipsed-Engine/Bin/Assets/Artifacts")
    export_assets("C:/MyFiles/Projects/Eclipsed-Engine/Assets", "C:/MyFiles/Projects/Eclipsed-Engine/Bin/Assets/Artifacts")
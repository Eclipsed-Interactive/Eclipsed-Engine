import sys

from export_assets import export_assets
from pack_assets import pack_assets

from copy_files import copy_files

if len(sys.argv) > 1:
    engineRoot = sys.argv[1]
    engineSourceFilesPath = sys.argv[2]
    engineOutputPath = sys.argv[3]
    engineAssetsPath = sys.argv[4]
    
    copy_files(engineSourceFilesPath, engineOutputPath + "/Headers", { ".h", ".hpp" })
    copy_files(engineRoot + "/Thirdparty/DLLs", engineOutputPath, { ".dll" })
    
    export_assets(engineAssetsPath, engineOutputPath + "/Local/Assets/")
    pack_assets(engineOutputPath + "/Local/Assets/", engineOutputPath)
else:
    print("Failed")
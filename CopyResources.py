
import os
import shutil
import sys

targetDir = sys.argv[1] + "/resources"
projectDir = sys.argv[2]
sourceDir = sys.argv[3]
gameDir = sys.argv[4]

if not os.path.exists(targetDir):
    os.mkdir(targetDir)

modelDir = targetDir + "/models"
shaderDir = targetDir + "/shaders"
textureDir = targetDir + "/textures"

excludedDirs = [".git", ".idea", ".vs", ".vscode", "cmake-build-debug", "cmake-build-release", "cmake-build-relwithprofiler", "cmake-build-relwithdebinfo", "build", "include"]

def copy_from_source(_root_dir):
    for path, subdirs, files in os.walk(_root_dir, True):
        shouldSkip = False
        for excludedDir in excludedDirs:
            if path == projectDir + "\\" + excludedDir:
                shouldSkip = True
                subdirs[:] = []

        if shouldSkip:
            continue

        print(path)

        for file in files:
            name, ext = os.path.splitext(file)

            match ext:
                case ".obj" | ".mtl":
                    output_dir = modelDir
                case ".glsl":
                    output_dir = shaderDir
                case ".png" | ".jpg" | ".jpeg":
                    output_dir = textureDir
                case _:
                    continue

            shutil.copy(path + "\\" + file, output_dir + "/" + file)

def clean_directory(_path):
    if os.path.exists(_path):
        shutil.rmtree(_path)

    os.mkdir(_path)

print("Clearing build resource directories")

clean_directory(modelDir)
clean_directory(shaderDir)
clean_directory(textureDir)

print("Copying resources")

copy_from_source(sourceDir)
copy_from_source(gameDir)
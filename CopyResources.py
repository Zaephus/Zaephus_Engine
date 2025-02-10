
import os
import shutil
import sys

targetDir = sys.argv[1] + "/resources"
sourceDir = sys.argv[2]
gameDir = sys.argv[3]

if not os.path.exists(targetDir):
    os.mkdir(targetDir)

modelDir = targetDir + "/models"
shaderDir = targetDir + "/shaders"
textureDir = targetDir + "/textures"

def copy_from_source(_root_dir):
    for path, subdirs, files in os.walk(_root_dir):

        for file in files:
            name, ext = os.path.splitext(file)

            match ext:
                case ".obj":
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
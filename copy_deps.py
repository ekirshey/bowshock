import sys
import glob, os, shutil



destination = "F:/github/bowshock_editor/deps/bowshock/lib/bowshock.lib"
source = sys.argv[1] + "/lib/Debug/bowshock.lib"

print(f"Copying {source} to {destination}")
shutil.copy2(source, destination)


destination = "F:/github/bowshock_editor/deps/bowshock/bowshock"
source = sys.argv[1] + "/src/"
print(f"Copying headers from {source} to {destination}")
files = glob.iglob(os.path.join(source, "*.h"))
for file in files:
    if os.path.isfile(file):
        shutil.copy2(file, destination)
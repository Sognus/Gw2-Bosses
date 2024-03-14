import os
import sys

# Get the project directory from command-line argument
if len(sys.argv) < 2:
    print("Error: Project directory not provided.")
    sys.exit(1)

project_dir = sys.argv[1]
file_path = os.path.join(project_dir, "build")

# Check if the file exists
if not os.path.exists(file_path):
    print(f"Error: File not found at {file_path}")
    build_version = 0
    with open(file_path, "w") as f:
        f.write(str(build_version))
else:
    with open(file_path, "r") as f:
        build_version = int(f.read())
    build_version += 1
    with open(file_path, "w") as f:
        f.write(str(build_version))

# Write to BuildVersion.h
with open("BuildVersion.h", "w") as f:
    f.write("""#ifndef GW2_BOSSES_VERSION_BUILD
#define GW2_BOSSES_VERSION_BUILD
#define VERSION_REVISION {}
#endif // GW2_BOSSES_VERSION_BUILD""".format(build_version))

print(f"Build number updated: {build_version - 1} -> {build_version}")
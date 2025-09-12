import sys
import subprocess
import re

from pathlib import Path

try:
    import yaml
except ImportError:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pyyaml"])
    import yaml  # import lại sau khi cài

input_file = ".clangd"
output_file = ".clangd"

# cấu hình cần thêm
extra_compile_flags = ["-Wno-enum-conversion"]

extra_diagnostics_suppress = ["enum-conversion"]

with open(input_file, "r", encoding="utf-8") as f:
    config = yaml.safe_load(f)

# nếu chưa có CompileFlags -> tạo
if "CompileFlags" not in config:
    config["CompileFlags"] = {}
if "Add" not in config["CompileFlags"]:
    config["CompileFlags"]["Add"] = []

# thêm flag nếu chưa có
for flag in extra_compile_flags:
    if flag not in config["CompileFlags"]["Add"]:
        config["CompileFlags"]["Add"].append(flag)

# nếu chưa có Diagnostics -> tạo
if "Diagnostics" not in config:
    config["Diagnostics"] = {}
if "Suppress" not in config["Diagnostics"]:
    config["Diagnostics"]["Suppress"] = []

# thêm suppress nếu chưa có
for diag in extra_diagnostics_suppress:
    if diag not in config["Diagnostics"]["Suppress"]:
        config["Diagnostics"]["Suppress"].append(diag)

# ghi ra file mới
with open(output_file, "w", encoding="utf-8") as f:
    yaml.dump(config, f, sort_keys=False)

cmake_file = Path("CMakeLists.txt")

text = cmake_file.read_text(encoding="utf-8")

src_dir = Path("Core/Src")
system_file = None
for f in src_dir.glob("system_stm32*.c"):
    system_file = f
    break

if system_file:
    system_file_path = f"${{CMAKE_SOURCE_DIR}}/{system_file.as_posix()}"
else:
    system_file_path = "# (no system_stm32*.c found)"

pattern = re.compile(
    r"# Add sources to executable.*?# Add include paths.*?\)", re.DOTALL
)

replacement = f"""# Add sources to executable
file(GLOB USER_SOURCES
    "${{CMAKE_SOURCE_DIR}}/Core/Src/*.c"
)

# Exclude system_stm32xx.c
list(REMOVE_ITEM USER_SOURCES "{system_file_path}")

target_sources(${{CMAKE_PROJECT_NAME}} PRIVATE
    ${{USER_SOURCES}}
)

# Add include paths
target_include_directories(${{CMAKE_PROJECT_NAME}} PRIVATE
    # Add user defined include paths
    ${{CMAKE_SOURCE_DIR}}/Core/Inc
)"""

new_text = pattern.sub(replacement, text)

hex_bin_block = """\n
# Create .hex
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMENT "Creating HEX file: ${CMAKE_PROJECT_NAME}.hex"
)

# Create .bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Creating BIN file: ${CMAKE_PROJECT_NAME}.bin"
)
"""

if "Creating HEX file" not in new_text:
    new_text = new_text.rstrip() + hex_bin_block

cmake_file.write_text(new_text, encoding="utf-8")

print("✅ Setup Completed !")

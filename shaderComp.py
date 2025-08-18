import os, subprocess

CWD = os.getcwd()
SRC_PATH = os.path.join(CWD, "res", "shader")
DST_PATH = os.path.join(CWD, "include", "render", "shaderSrc")

MAX_COLUMN_LENGTH = 100
TAB_SIZE = 4

srcDir = os.fsencode(SRC_PATH)
dstDir = os.fsencode(DST_PATH)

if not os.path.exists(dstDir):
	os.mkdir(dstDir)

def createHeader(filename: bytes, bytecode: bytes) -> None:
	split = os.fsdecode(filename).split(".")
	varName: str = split[0] + split[1][0].capitalize() + "ShaderSrc"
	out: str = f"""#pragma once

#include <array>
#include <cstdint>

namespace Bloodlust {{

constexpr const std::array<const uint8_t, {len(bytecode) + 1}> {varName} = {{
	"""
	column: int = TAB_SIZE
	for v in bytecode:
		codepoint: str = "0x%0.02X, " % v
		if column + 6 > MAX_COLUMN_LENGTH:
			out = out[:-1] + "\n\t"
			column = TAB_SIZE
		out += codepoint
		column += 6
	out = out[:-2]
	out += "\n};\n\n}\n"
	with open(os.path.join(dstDir, filename + ".hpp".encode()), "w") as file:
		file.write(out)

for file in os.listdir(srcDir):
	filename: str = os.fsdecode(file)
	if filename.endswith(".vert") or filename.endswith(".frag"):
		compiled: bytes = subprocess.check_output(["glslc", os.path.join(srcDir, file), "-o", "-"])
		createHeader(file, compiled)

import os, subprocess, threading

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
	decodedFilename = os.fsdecode(filename)
	split = decodedFilename.split(".")
	varName: str = split[0] + split[1][0].capitalize() + "ShaderSrc"
	out: str = f"""#pragma once

#include <array>
#include <cstdint>

namespace Bloodlust {{

// Precompiled shader for file "{decodedFilename}"
constexpr const std::array<const std::uint8_t, {len(bytecode) + 1}> {varName} = {{
	"""
	for v in bytecode:
		codepoint: str = "0x%0.02X," % v
		out += codepoint
	out += "0x00};};"
	outPath = os.path.join(dstDir, filename + ".hpp".encode())
	with open(outPath, "w") as file:
		file.write(out)
	subprocess.check_output(["clang-format", "-i", outPath.decode()])

def compileShader(file: bytes):
	filename: str = os.fsdecode(file)
	if filename.endswith(".vert") or filename.endswith(".frag"):
		try:
			compiled: bytes = subprocess.check_output(["glslc", os.path.join(srcDir, file), "-o", "-"])
		except subprocess.CalledProcessError:
			print("-ERROR- Compilation failed for file " + filename)
			return
		createHeader(file, compiled)

threads: list[threading.Thread] = []
for file in os.listdir(srcDir):
	threads.append(threading.Thread(target=compileShader, args=(file,)))

for t in threads:
	t.start()
for t in threads:
	t.join()

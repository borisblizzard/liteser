# Cateia Games Upsell Screen Graphics Assembler

import os
import shutil
import sys

from Model import *
from Util import *

VERSION = "3.1"

def process():
	print("")
	print("-------------------------------------------------------------------------------")
	print("| Liteser Format Converter " + VERSION)
	print("-------------------------------------------------------------------------------")
	
	if len(sys.argv) != 5 and len(sys.argv) != 6:
		help()
		return
	inputPath = sys.argv[1]
	outputPath = sys.argv[2]
	inputFormat = sys.argv[3]
	outputFormat = sys.argv[4]
	deleteOriginals = False
	if len(sys.argv) > 5:
		deleteOriginals = (sys.argv[5] == "1")
	if not inputFormat in Model.Formats:
		print("ERROR: Format not supported: " + inputFormat)
		return
	if not outputFormat in Model.Formats:
		print("ERROR: Format not supported: " + outputFormat)
		return
	
	if os.path.isdir(inputPath):
		print("")
		print("-- Finding files...")
		print("")
		files = Util.getFileList(inputPath)
		inputFiles = []
		outputFiles = []
		inputExtension = "." + inputFormat
		outputExtension = "." + outputFormat
		for file in files:
			if file.endswith(inputExtension):
				filename = file[len(inputPath) + 1: len(file) - len(inputExtension)]
				inputFiles.append(inputPath + "/" + filename + inputExtension)
				outputFiles.append(outputPath + "/" + filename + outputExtension)
	else:
		inputFiles = [inputPath]
		outputFiles = [outputPath]
	if len(inputFiles) != len(outputFiles):
		print("ERROR: inputFiles size and outputFiles size don't match: %d and %d" % (len(inputFiles), len(outputFiles)))
		return
		
	print("")
	print("-- Reading files...")
	print("")
	data = []
	for file in inputFiles:
		print("  reading: " + file)
		data.append(Model.readFile(inputFormat, file))
	if len(data) != len(outputFiles):
		print("ERROR: Data size and outputFiles size don't match: %d and %d" % (len(data), len(outputFiles)))
		return
		
	print("")
	print("-- Writing files...")
	print("")
	for i in range(len(data)):
		print("  writing: " + outputFiles[i])
		try:
			os.makedirs(os.path.dirname(outputFiles[i]))
		except:
			pass
		Model.writeFile(outputFormat, outputFiles[i], data[i])
	
	if deleteOriginals:
		print("")
		print("-- Deleting originals...")
		print("")
		for file in inputFiles:
			print("  deleting: " + file)
			os.remove(file)
	
	print("")
	print("Done!")
	
def help():
	print("")
	print("This script converts files between the various Liteser formats.")
	print("")
	print("usage:   convert_format.py INPUT_PATH OUTPUT_PATH INPUT_FORMAT OUTPUT_FORMAT [DELETE_ORIGINALS]")
	print("")
	print("  INPUT_PATH       - Input directory or filename. If directory, it will be")
	print("                     parsed recursively.")
	print("  OUTPUT_PATH      - Output directory or filename. Should match INPUT_PATH's")
	print("                     path type to avoid problems.")
	print("  INPUT_FORMAT     - The input format. See below for supported formats.")
	print("  OUTPUT_FORMAT    - The output format. See below for supported formats.")
	print("  DELETE_ORIGINALS - 0 or 1, deletes originals after successful conversion.")
	print("")
	print("Supported formats:")
	print("  ls3 - Liteser 3 Binary")
	print("  lsx - Liteser XML")
	print("")
	print("example: convert_format.py ../data data/ls ls3 lsx")
	print("example: convert_format.py characters.lsx \"data files/characters.ls3\" lsx ls3")
	print("")
	os.system("pause")

process()

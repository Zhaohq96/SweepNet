import sys, time, os
import getopt
import argparse
import shutil

from Logic import Architecture
from contextlib import redirect_stdout


def help():
	
	print("\nThis script can be executed for training a CNN model and using an existing CNN model to classify the unknown data.\n")
	print("Required flag:")
	print("\t-n: mode (str), \"train\" or \"predict\"\n")
	print("If -n is \"train\",")
	print("the: -d, -h, -w, -e, -o, -t flags are required")
	print("\t-d: path to an input folder (str), the folder should be organized as:")
	print("\t\tFolder_main")
	print("\t\t--Folder_classA")
	print("\t\t----classA_image_1")
	print("\t\t----classA_image_2")
	print("\t\t----...")
	print("\t\t--Folder_classB")
	print("\t\t----classB_image_1")
	print("\t\t----classB_image_2")
	print("\t\t----...")
	print("\t-h: the height of the images (int)")
	print("\t-w: the width of the images (int)")
	print("\t-e: the number of training epochs (int) (def: 10)")
	print("\t-o: path to an output folder (str)")
	print("\t-t: the number of threads (int) (def: 8)\n")
	print("If -n is \"predict\",")
	print("the: -d, -h, -w, -m, -o, -t flags are required")
	print("\t-d: path to an input folder containing the unknown data to predict (str)")
	print("\t-h: the height of the images (int)")
	print("\t-w: the width of the images (int)")
	print("\t-m: path to the folder containing the training model (str), NOTE that it is the path of the folder and the training model should be a .hdf5 file named as \"weights.best.hdf5\"")
	print("\t-o: path to an output folder (str)")
	print("\t-t: the number of threads (int) (def: 8)\n")
	
	
	

def main(argv):

	opts, ars = getopt.getopt(argv, "d:n:h:w:e:m:o:t:s:c:", ["directory=", "mode=", "height=", "width=", "epoch=", "model=", "outpath=", "threads=", "thread=", "hardware=", "help"])
    	
	epoch = 10
	hardware = "CPU"
	model = "SweepNet"
    	
	for opt, arg in opts:
		if opt in ("-d", "--directory"):
			direct = arg
		elif opt in ("-n", "--mode"):
    			mod = arg
		elif opt in ("-h", "--height"):
			height = arg
		elif opt in ("-w", "--width"):
			width = arg
		elif opt in ("-e", "--epoch"):
			epoch = arg
		elif opt in ("-m", "--model"):
			model = arg
		elif opt in ("-o", "--outpath"):
			out = arg
		elif opt in ("-t", "--threads"):
			threads = arg
		elif opt in ("-s", "--thread"):
			thread = arg
		elif opt in ("-c", "--hardware"):
			hardware = arg
		elif opt in ("--help"):
			help()
			return 0
	
	
	
	
	if (mod == "train"):
		
		trainModel = Architecture.Training(direct, int(float(height)), int(float(width)), int(epoch), model, out, int(threads))
		trainModel.traingModel()
		
		
		
	if (mod == "predict"):
		if not os.path.exists(out):
			os.makedirs(out)
		else:
			shutil.rmtree(out)
			os.makedirs(out)
		
		loadModel = Architecture.Load(model, direct, height, width, out, int(threads))
		numberOfImages = loadModel.imageFolder()
		loadModel.generateReport()
		
	
	
	
if __name__ == "__main__":
    main(sys.argv[1:])	

import numpy as np
import re, math, sys, os, string
import getopt
import shutil
import time
from PIL import Image
	
	
def help():
	
	print("\nThis script can be executed for converting the .txt files of the binary matrices into images.\n")
	print("The: -i, -h, -w, -o, -f, -m flags are required")
	print("\t-d: path to an input folder containing the .txt files of the binary matrices (str)")
	print("\t-h: the height of the images (int)")
	print("\t-w: the width of the images (int)")
	print("\t-o: path to an output folder (str)")
	print("\t-f: the stored format (str), \"pdf\" or \"png\" (def: png)")
	print("\t-m: color mode (str), \"black\", \"grey\" or \"RGB\" (def: black)\n")
	

def main(argv):
	
	mode = 'black'
	
	opts, ars = getopt.getopt(argv, "i:o:w:h:f:m:", ["help", "inputpath=", "outputpath=", "windowwidth=", "windowheight=", "format=", "mode="])
	
	for opt, arg in opts:
		if opt == '--help':
			help()
			sys.exit()
		elif opt in ("-i", "--inputpath"):
			indir = arg
		elif opt in ("-o", "--outputpath"):
			outdir = arg
		elif opt in ("-w", "--windowwidth"):
			width = arg
		elif opt in ("-h", "--windowheight"):
			height = arg
		elif opt in ("-f", "--format"):
			form = arg
		elif opt in ("-m", "--mode"):
			mode = arg
		
	
	start=time.time()
	
	if not os.path.exists(outdir):
		os.makedirs(outdir)
		
	else:
		shutil.rmtree(outdir)
		os.makedirs(outdir)
	
	filepath = indir
	filelist = os.listdir(filepath)
	
	for file in filelist:     # iterate through all files in the directory
		matrix = np.empty([int(height), int(width)], dtype=np.uint8)
		
		f=open(os.path.join(filepath, file))

		
		for lineindex, line in enumerate(f):
			line = line.replace('\n', '')

			
			matrix[lineindex] = np.array(list(line), dtype=np.uint8)     # store the data into matrix
			

		matrix_RGB = np.empty([int(height), int(width), 3], dtype=np.uint8)
		

		
		if mode == 'grey':
			matrix = matrix * 127
			img = Image.fromarray(matrix)
			
		elif mode == 'black':
			matrix = 255 - matrix * 255

			img = Image.fromarray(matrix)
			
		elif mode == 'RGB':
			for i in range(int(height)):
				for j in range(int(width)):
					if matrix[i][j] == 0:
						matrix_RGB[i][j] = [0, 0, 255]
					elif matrix[i][j] == 1:
						matrix_RGB[i][j] = [255, 0, 0]
					else:
						matrix_RGB[i][j] = [0, 255, 0]

			img = Image.fromarray(np.uint8(matrix_RGB))
			
		else:
			print('EEROR: No available image format indicated')
			sys.exit()
			

		
		if form == 'png':
			img.save(outdir + '/' + str(file.split('.')[0]) + '.png')
			
		elif form == 'pdf':
			img.save(outdir + '/' + str(file.split('.')[0]) + '.jpeg', format='jpeg', bbox_inches='tight')
			
		else:
			print('EEROR: No image format indicated')
			sys.exit()
		
		
		f.close()
	
	end=time.time()
	print(end-start)
	
	
	
	
	
if __name__ == "__main__":
	main(sys.argv[1:])


	

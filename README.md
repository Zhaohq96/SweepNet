# SweepNet: a lightweight CNN architecture for the classification of adaptive genomic regions

First release: 1/3/2023

Last update: 4/7/2023

Version: 1.0

## About
SweepNet has 5 layers and an extra SE block. Before feeding images to SweepNet, a data pre-processing method is employed to further improve classification ability of the CNN.

The article describing SweepNet is published in _PASC2023 (Platform for Advanced Scientific Computing)_:

1. SweepNet: A Lightweight CNN Architecture for the Classification of Adaptive Genomic Regions

   URL: https://dl.acm.org/doi/abs/10.1145/3592979.3593411

## Environmental Setup
SweepNet uses Keras, a high-level API which using TensorFlow as its backend, to build the network. To avoid package dependencies, we recommend to use Anaconda to build virtual environment. The installation of Anaconda can be found via https://www.anaconda.com/

To build virtual environment by command:

``conda create -n myenv python=3.8 protobuf tensorflow=2.8 keras=2.8 numpy h5py matplotlib=3.4.1 tensorboard=2.8 pillow=7.0.0``

"myenv" as the name of virtual environment, can be changed.

To activate virtual environment:

``conda activate myenv``

To deactivate virtual environment:

``conda deactivate``

## Source Code and Already-generated Datasets Download
The zip file of the source code can be downloaded via github. Then the following command can be used to unzip the file and to be in the path of main folder.

``unzip SweepNet-main.zip``

``cd SweepNet-main``

The already-generated datasets used in the paper can be downloaded via https://figshare.com/articles/dataset/Datasets_used_in_SweepNet/22194118 and then the following command can be used to unzip the file.

``unzip DATASETS.zip``



## A quick example
In order to run a quick example successfully, we recommend to put the already-generated datasets under the folder _SweepNet-main_. First, correct the _current_path_ in the .sh file _TrainTest.sh_.

Then, the following command can be used to run a quick example.

``sh TrainTest.sh -m SweepNet -c bp -i DATASETS/D1 -o Example-results -h 128 -n 128 -s 5000 -l 100000 -r 5 -e 10 -t black``

The tranining model and testing will be stored in the folder _Example-results_.

The description of the supported flags of _TrainTest.sh_ can be provided by the following command.

``sh TrainTest.sh -H``

The quick-reference help message is the following.
```
This is a fast script for identifying selective sweep using SweepNet (a developed CNN).

	-i: path to an input folder (str), the folder should be organized as:
		Path_input
		--train
		----neutral.ms
		----selection.ms
		--test
		----neutral.ms
		----selection.ms
	The sub-folders "train" and "test" both contain two files in ms format. "neutral.ms" and "selection.ms" are the ms files containing neutrality and selective sweep data respectively.
	NOTE that the sub-folders and the ms files should be named and organized exactly same as the above structure.
	-o: path to an output folder (str)
	-h: height of images (int)
	-n: width of images (int)
``` 


## In-tool Help
SweepNet consists of two separate scripts. Each script output a quick-reference help message that provides a short description for each command-line flags.

For _NN.py_, the following command can be used.

``python NN.py --help``

The generated message is the following.

```
This script can be executed for training a CNN model and using an existing CNN model to classify the unknown data.

Required flag:
	-n: mode (str), "train" or "predict"

If -n is "train",
the: -d, -h, -w, -e, -o, -t flags are required
	-d: path to an input folder (str), the folder should be organized as:
		Folder_main
		--Folder_classA
		----classA_image_1
		----classA_image_2
		----...
		--Folder_classB
		----classB_image_1
		----classB_image_2
		----...
	-h: the height of the images (int)
	-w: the width of the images (int)
	-e: the number of training epochs (int) (def: 10)
	-o: path to an output folder (str)
	-t: the number of threads (int) (def: 8)

If -n is "predict",
the: -d, -h, -w, -m, -o, -t flags are required
	-d: path to an input folder containing the unknown data to predict (str)
	-h: the height of the images (int)
	-w: the width of the images (int)
	-m: path to the folder containing the training model (str), NOTE that it is the path of the folder and the training model should be a .hdf5 file named as "weights.best.hdf5"
	-o: path to an output folder (str)
	-t: the number of threads (int) (def: 8)
```

For _NN.py_, the following command can be used.

``python win2img.py --help``

The generated message is the following.

```
This script can be executed for converting the .txt files of the binary matrices into images.

The: -i, -h, -w, -o, -f, -m flags are required
	-d: path to an input folder containing the .txt files of the binary matrices (str)
	-h: the height of the images (int)
	-w: the width of the images (int)
	-o: path to an output folder (str)
	-f: the stored format (str), "pdf" or "png" (def: png)
	-m: color mode (str), "black", "grey" or "RGB" (def: black)
```


## Input File Formats
The current SweepNet can process SNP data in Hudson's ms file format. As SweepNet consists of separate scripts, it can also process data in image file format separately by feeding images to _NN.py_.




## Output Files


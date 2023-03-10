# SweepNet: a lightweight CNN architecture for the classification of adaptive genomic regions

First release: 1/3/2023

Last update: 2/3/2023

Version: 1.0

## About
SweepNet has 5 layers and an extra SE block. Before feeding images to SweepNet, a data pre-processing method is employed to further improve classification ability of the CNN.

## Environmental setup
SweepNet uses Keras, a high-level API which using TensorFlow as its backend, to build the network. To avoid package dependencies, we recommend to use Anaconda to build virtual environment. The installation of Anaconda can be found via https://www.anaconda.com/

To build virtual environment by command:

``conda create -n myenv python=3.8 protobuf tensorflow=2.8 keras=2.8 numpy h5py matplotlib=3.4.1 tensorboard=2.8 pillow=7.0.0``

"myenv" as the name of virtual environment, can be changed.

To activate virtual environment:

``conda activate myenv``

To deactivate virtual environment:

``conda deactivate``

## Source code and already-generated datasets download
The zip file of the source code can be downloaded via github. Then the following command can be used to unzip the file and to be in the path of main folder.

``unzip SweepNet-main.zip``

``cd SweepNet-main``

The already-generated datasets used in the paper can be downloaded via https://figshare.com/articles/dataset/Datasets_used_in_SweepNet/22194118 and then the following command can be used to unzip the file.

``unzip DATASETS.zip``



## A quick example
In order to run a quick example successfully, we recommend to put the already-generated datasets under the folder _SweepNet-main_. The following command can be used to run a quick example.

``sh TrainTest.sh -m SweepNet -c bp -i DATASETS/D1 -o Example-results -h 128 -n 128 -s 5000 -l 100000 -r 5 -e 10 -t black``

The tranining model and testing will be stored in the folder _Example-results_.

## To be completed...

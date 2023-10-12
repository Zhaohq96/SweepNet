#!/bin/bash

helpprint()
{
	echo ''
	echo 'This is a fast script for identifying selective sweep using SweepNet (a developed CNN).'
	echo ''
	
	echo '\t-i: path to an input folder (str), the folder should be organized as:'
	echo '\t\tPath_input'
	echo '\t\t--train'
	echo '\t\t----neutral.ms'
	echo '\t\t----selection.ms'
	echo '\t\t--test'
	echo '\t\t----neutral.ms'
	echo '\t\t----selection.ms'
	echo '\tThe sub-folders "train" and "test" both contain two files in ms format. "neutral.ms" and "selection.ms" are the ms files containing neutrality and selective sweep data respectively.'
	echo '\tNOTE that the sub-folders and the ms files should be named and organized exactly same as the above structure.'
	
	echo '\t-o: path to an output folder (str)'
	echo '\t-h: height of images (int)'
	echo '\t-n: width of images (int)'
	echo ''
	
	exit 1
}

current_path=/home/david/project/12ReorderStrategies

modeA=0
modeB=0
center='bp'
nn='SweepNet'
win_site=5000
length=100000
min_snp=1
color='grey'
grid=1

while getopts "d:a:b:m:c:e:i:o:h:n:s:l:r:t:g:H" opt
do
	case "${opt}" in
		d) data=${OPTARG};;
		a) modeA=${OPTARG};;
		b) modeB=${OPTARG};;
		m) nn=${OPTARG};;
		c) center=${OPTARG};;
  		e) epoch=${OPTARG};;
		i) input_path=${OPTARG};;
		o) output_path=${OPTARG};;
		h) height=${OPTARG};;
		n) win_snp=${OPTARG};;
		s) win_site=${OPTARG};;
		l) length=${OPTARG};;
		r) min_snp=${OPTARG};;
		t) color=${OPTARG};;
		g) grid=${OPTARG};;
		H) helpprint ;;
	esac
done

$current_path/ms2txt -i $input_path/train/neutral.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/train/split_txt/neutral;

$current_path/ms2txt -i $input_path/train/selection.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/train/split_txt/selection;

python $current_path/win2img.py -i $output_path/train/split_txt/neutral -o $output_path/train/images/neutral -w $win_snp -h $height -f png -m "$color";

python $current_path/win2img.py -i $output_path/train/split_txt/selection -o $output_path/train/images/selection -w $win_snp -h $height -f png -m "$color";

########
$current_path/ms2txt -i $input_path/test/neutral.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/test/split_txt/neutral;

$current_path/ms2txt -i $input_path/test/selection.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/test/split_txt/selection;

python $current_path/win2img.py -i $output_path/test/split_txt/neutral -o $output_path/test/images/neutral -w $win_snp -h $height -f png -m "$color";

python $current_path/win2img.py -i $output_path/test/split_txt/selection -o $output_path/test/images/selection -w $win_snp -h $height -f png -m "$color";

###########
python $current_path/NN.py -n train -m SweepNet -o $output_path/train/Model_A$((modeA))_B$((modeB))_"$center"_w$((win_snp))h$((height)) -h $height -w $win_snp -d window/$((win_snp))/D$((data))/A$((modeA))_B$((modeB))/train/images -e $epoch -t 8;

python $current_path/NN.py -n predict -m $output_path/train/Model_A$((modeA))_B$((modeB))_"$center"_w$((win_snp))h$((height)) -h $height -w $win_snp -d window/$((win_snp))/D$((data))/A$((modeA))_B$((modeB))/test/images/neutral -o $output_path/test/result/neutral;

python $current_path/NN.py -n predict -m $output_path/train/Model_A$((modeA))_B$((modeB))_"$center"_w$((win_snp))h$((height)) -h $height -w $win_snp -d window/$((win_snp))/D$((data))/A$((modeA))_B$((modeB))/test/images/selection -o $output_path/test/result/selection;


#!/bin/bash


current_path=/home/david/project/12ReorderStrategies

while getopts "d:a:b:m:c:i:o:h:n:s:l:r:t:g:" opt
do
	case "${opt}" in
		d) data=${OPTARG};;
		a) modeA=${OPTARG};;
		b) modeB=${OPTARG};;
		m) nn=${OPTARG};;
		c) center=${OPTARG};;
		i) input_path=${OPTARG};;
		o) output_path=${OPTARG};;
		h) height=${OPTARG};;
		n) win_snp=${OPTARG};;
		s) win_site=${OPTARG};;
		l) length=${OPTARG};;
		r) min_snp=${OPTARG};;
		t) color=${OPTARG};;
		g) grid=${OPTARG};;	
	esac
done

$current_path/ms2txt -i $input_path/train/neutral.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/train/split_txt/neutral;

$current_path/ms2txt -i $input_path/train/selection.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/train/split_txt/selection;

python $current_path/win2img.py -i $output_path/train/split_txt/neutral -o $output_path/train/images/neutral -w $win_snp -h $height -f png -m "$color";

python $current_path/win2img.py -i $output_path/train/split_txt/selection -o $output_path/train/images/selection -w $win_snp -h $height -f png -m "$color";

###########
$current_path/ms2txt -i $input_path/test/neutral.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/test/split_txt/neutral;

$current_path/ms2txt -i $input_path/test/selection.ms -c "$center" -w $win_snp -s $win_site -l $length -r $min_snp -a $modeA -b $modeB -o $output_path/test/split_txt/selection;

python $current_path/win2img.py -i $output_path/test/split_txt/neutral -o $output_path/test/images/neutral -w $win_snp -h $height -f png -m "$color";

python $current_path/win2img.py -i $output_path/test/split_txt/selection -o $output_path/test/images/selection -w $win_snp -h $height -f png -m "$color";

###########
python $current_path/NN.py -n train -m SweepNet -o $output_path/train/Model_A$((modeA))_B$((modeB))_"$center"_w$((win_snp))h$((height)) -h $height -w $win_snp -d $output_path/train/images -e 1 -t 8;

python $current_path/NN.py -n predict -m $output_path/train/Model_A$((modeA))_B$((modeB))_"$center"_w$((win_snp))h$((height)) -h $height -w $win_snp -d $output_path/test/images/neutral -o $output_path/test/result/neutral;

python $current_path/NN.py -n predict -m $output_path/train/Model_A$((modeA))_B$((modeB))_"$center"_w$((win_snp))h$((height)) -h $height -w $win_snp -d $output_path/test/images/selection -o $output_path/test/result/selection;


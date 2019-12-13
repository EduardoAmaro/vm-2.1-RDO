import os
import sys

if len(sys.argv) == 4:
	lf = sys.argv[1]
	prediction = sys.argv[2]
	_lambda = sys.argv[3]

	cmd1 = 'make all -j 6 && '
	cmd2 = 'time .././mule-encoder-bin '
	cmd3 = '-lf /home/douglascorrea/light-field/dataset/Lenslet/' + lf + '/ '
	cmd4 = '-o /home/douglascorrea/light-field/resultados_prediction/' + sys.argv[1] + '/coding/' + 'output_' + prediction + '_' + _lambda + '.LF '
	cmd5 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
	cmd6 = '-lambda ' + _lambda + ' '
	cmd7 = '-prediction ' + prediction + ' && '
	cmd8 = 'mv partition_codes.txt /home/douglascorrea/light-field/resultados_prediction/' + lf + '/partition_codes_files/partition_codes_' + prediction + '_' + _lambda + '.txt'

	print(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6 + cmd7 + cmd8)
	os.system(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6 + cmd7 + cmd8)

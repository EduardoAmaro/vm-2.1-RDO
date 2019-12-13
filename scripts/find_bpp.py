import os
import  csv
import math

_lambda = 140
file_size = 0
step = 10
total_pixels = 13*13*625*434
flag_if = '>'
bpp = 0

name = './mule-encoder-bin_DR'
results_path = '/home/douglascorrea/light-field/resultados_prediction/Bikes/coding/'

with open('lambdas_DR.csv', 'w') as fp:
	fp_writer = csv.writer(fp)

	while bpp <= 0.75*0.99 or file_size >= 0.75*1.01:
		print('lambda: ' + str(_lambda))
	
		alg_name = name.split('_')[1]
		cmd1 = '../' + name + ' -lf /home/douglascorrea/light-field/dataset/Lenslet/Bikes/ '
		cmd2 = '-o ' + results_path + 'bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF '
		cmd3 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
		cmd4 = '-lambda ' + str(_lambda) + ' > ' + results_path + 'summary/summary_' + alg_name + '_' + str(_lambda) + '.txt'
		print(cmd1 + cmd2 + cmd3 + cmd4)
		cmd = cmd1 + cmd2 + cmd3 + cmd4
		os.system(cmd)

		file_size = os.path.getsize(results_path + 'bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF')
		bpp = (file_size*8)/(total_pixels)
		print('size: ' + str(file_size) + ' - bpp:' + str(bpp))
		print('')
		string = str(_lambda) + ',' + str(file_size) + ',' + str(bpp)
		fp_writer.writerows(string)

		if bpp > 0.75*1.01:
			_lambda += step
			if flag_if == '<':
				step -= 1
			flag_if = '>'
		elif bpp < 0.75*0.99:
			_lambda -= step
			if flag_if == '>':
				step -= 1
			flag_if = '<'
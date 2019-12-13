import os
import sys

prediction_list = ['rdo']
split_flags = ['1', '0']
#superior_bitplanes = ['0', '2', '4', '6', '8', '10', '12', '14', '16', '18', '20', '22', '24', '26', '28']
superior_bitplanes = ['30']
inferior_bitplanes = ['0', '2', '4', '6', '8', '10', '12', '14', '16', '18', '20', '22', '24', '26', '28']
for prediction in prediction_list:
	for split in split_flags:
		for superior_bitplane in superior_bitplanes:
			for inferior_bitplane in inferior_bitplanes:
				cmd1 = './mule-decoder-bin '
				cmd2 = '-i /home/eduardo/Documentos/resultados_prediction/Bikes/coding/bitstream_bitplanes/output_' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.LF '
				cmd3 = '-o /home/eduardo/Documentos/resultados_prediction/Bikes/decoding/' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '/ '
				cmd4 = '-mule /home/douglascorrea/GitHub/vm-2.1-prediction_simulacao/ '
				cmd5 = '-prediction ' + prediction + ' ' 
				cmd6 = '-predictionFileName /home/eduardo/Documentos/resultados_prediction/Bikes/coding/prediction_bitplanes/prediction_' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt'
				cmd7 = ' > summary.txt'
				
				print(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6 + cmd7)
				os.system(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6 + cmd7)
				
				cmd = 'mv summary.txt /home/eduardo/Documentos/resultados_prediction/Bikes/decoding/bitplanes/summary/' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt'
				print(cmd)
				os.system(cmd)

				cmd = 'mkdir /home/eduardo/Documentos/resultados_prediction/Bikes/decoding/bitplanes/' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '/'
				print(cmd)
				os.system(cmd)

				cmd = 'mv *.ppm /home/eduardo/Documentos/resultados_prediction/Bikes/decoding/bitplanes/' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '/'
				print(cmd)
				os.system(cmd)
				print('')

import os
import sys

prediction_list = ['rdo']
split_flags = ['0', '1']
#superior_bitplanes = ['0', '2', '4', '6', '8', '10', '12', '14', '16', '18', '20', '22', '24', '26', '28']
superior_bitplanes = ['30']
inferior_bitplanes = ['0', '2', '4', '6', '8', '10', '12', '14', '16', '18', '20', '22', '24', '26', '28']

for prediction in prediction_list:
	for split in split_flags:
		for superior_bitplane in superior_bitplanes:
			for inferior_bitplane in inferior_bitplanes:
				cmd1 = 'time ./mule-encoder-bin '
				cmd2 = '-lf /home/eduardo/Documentos/Datasets/Bikes/Bikes/ '
				cmd3 = '-o /home/eduardo/Documentos/resultados_prediction/Bikes/coding/bitstream_bitplanes/output_' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.LF '
				cmd4 = '-cf /home/eduardo/Documentos/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
				cmd5 = '-lambda 1 '
				#cmd6 = '-prediction ' + prediction + ' '
				cmd6 = '-split ' + split + ' -inferiorBitPlane ' + inferior_bitplane + ' -superiorBitPlane ' + superior_bitplane + ' -evaluateOptimumBitPlane 0 -performRDO 0 > summary.txt'
				
				print(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6)
				os.system(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6)

				cmd = 'mv partition_codes.txt /home/eduardo/Documentos/resultados_prediction/Bikes/coding/partition_codes_bitplanes/partition_codes_' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt'
				print(cmd)
				os.system(cmd)

				cmd = 'mv summary.txt /home/eduardo/Documentos/resultados_prediction/Bikes/coding/summary_bitplanes/summary_' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt'
				print(cmd)
				os.system(cmd)

				cmd = 'mv prediction.txt /home/eduardo/Documentos/resultados_prediction/Bikes/coding/prediction_bitplanes/prediction_' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt'
				print(cmd)
				os.system(cmd)

				cmd = 'mv DC_predictors.txt /home/eduardo/Documentos/resultados_prediction/Bikes/coding/DC_predictors/DC_predictor_' + prediction + '_1_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt'
				print(cmd)
				os.system(cmd)
				print('')

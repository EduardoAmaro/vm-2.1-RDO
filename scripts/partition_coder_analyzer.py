import sys
from collections import Counter
import numpy as np
import matplotlib.pyplot as plt
import os

prediction_list = ['mule', 'diffR', 'diffRDC', 'diffC', 'diffCDC']
lambdas = ['322']#['322', '8750', '127000', '1250000']
split_list = ['1']
superior_bitplanes = ['30']
inferior_bitplanes = inferior_bitplanes = ['0', '2', '4', '6', '8', '10', '12', '14', '16', '18', '20', '22', '24', '26', '28']
results_path = '/home/douglascorrea/light-field/resultados_prediction/Bikes/coding/partition_codes_bitplanes/'
figures_path = '/home/douglascorrea/light-field/resultados_prediction/Bikes/coding/partition_codes_figures/'
partitioning_analysis_path = '/home/douglascorrea/light-field/resultados_prediction/Bikes/coding/partition_code_analysis/'
# prediction = sys.argv[1]
# _lambda = sys.argv[2]
for prediction in prediction_list:
	for _lambda in lambdas:
		for split in split_list:
			for superior_bitplane in superior_bitplanes:
				for inferior_bitplane in inferior_bitplanes:
					spatial_partitioning = 0
					angular_partitioning = 0

					print(prediction + '_' + _lambda + '_' + split + '_' + superior_bitplane + '_' + inferior_bitplane)
					partition_code_list = []
					with open(results_path + 'partition_codes_' + prediction + '_' + _lambda + '_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt', 'r') as fp:
						file_content = fp.readlines()
						print(results_path + prediction + '_' + _lambda + '_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt')
						
						for partition_code in file_content:
							partition_code = partition_code.rstrip('\n')
							#print(partition_code)

							spatial_partitioning += partition_code.count('S')
							angular_partitioning += partition_code.count('V')

							partition_code_list.append(partition_code)

					fp_write = open(partitioning_analysis_path + prediction + '_' + _lambda + '_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.txt', 'w')
					fp_write.write("Spatial partitionings: " + str(spatial_partitioning) + '\n')
					fp_write.write("Angular partitionings: " + str(angular_partitioning) + '\n')
					fp_write.write("Total partitionings: " + str(spatial_partitioning + angular_partitioning) + '\n')
					
					#counts = dict(Counter(partition_code_list).most_common(10))
					counts = Counter(partition_code_list)
					partition_code, frequency = zip(*counts.items())
					index_sort = np.argsort(frequency)[::1]
					partition_code = np.array(partition_code)[index_sort]
					frequency = np.array(frequency)[index_sort]
					indexes = np.arange(len(partition_code))

					#print(partition_code)
					fp_write.write('\n' + str(partition_code) + '\n')
					fp_write.close()


					if prediction == 'diffR':
						prediction_type = 'Differential Raster'
					elif prediction == 'diffC':
						prediction_type = 'Differential Central'
					elif prediction == 'mule':
						prediction_type = 'MuLE'
					elif prediction == 'diffRDC':
						prediction_type = 'Differential Raster DC Ref Plane'
					elif prediction == 'diffCDC':
						prediction_type = 'Differencial Central DC Ref Plane'

					#plt.figure(figsize=(30, 10))
					plt.bar(indexes, frequency, tick_label=indexes)
					plt.xticks(indexes, partition_code)
					plt.title(prediction_type + ' - Bmin = ' +  inferior_bitplane + ' - Occurrences of each partition code', fontsize=14)
					plt.xlabel('Partition codes', fontsize=12)
					plt.ylabel('Occurrences', fontsize=12)
					plt.yscale('log')
					plt.xticks(rotation=90, fontsize=6)
					#plt.tight_layout()
					#ax = plt.gca()
					# ax.set_xticks(ax.get_xticks()[::2])
					#plt.show()
					plt.savefig('partition_code_figure.png', bbox_inches='tight', dpi=400)
					plt.close()

					cmd = 'mv partition_code_figure.png ' + figures_path +  prediction + '_' + _lambda + '_' + split + '_' + superior_bitplane + '_' + inferior_bitplane + '.png'
					print(cmd)
					os.system(cmd)
					print('')
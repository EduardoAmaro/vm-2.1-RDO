import os

lf_dataset = ['Bikes']#, 'Danger_de_Mort']
lambdas = [322, 5800, 127000, 1250000]
exec_names = ['./mule-decoder-bin_H']

for lf in lf_dataset:
	for name in exec_names:
		for _lambda in lambdas:
			alg_name = name.split('_')[1]
			cmd1 = '../' + name + ' -i /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF '
			cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + alg_name + '_' + str(_lambda) + '/ '
			cmd3 = '-mule /home/douglascorrea/GitHub/vm-2.1-prediction/ '
			print(cmd1 + cmd2 + cmd3)
			cmd = cmd1 + cmd2 + cmd3
			os.system(cmd)

			cmd = 'mv *.ppm /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + alg_name + '_' + str(_lambda) + '/ '
			print(cmd)
			os.system(cmd)

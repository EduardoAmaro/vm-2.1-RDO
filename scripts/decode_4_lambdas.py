import os

lf_dataset = ['Bikes', 'Danger_de_Mort', 'Fountain_Vincent2', 'Stone_Pillars_Outside']
prediction_list = ['mule', 'diffR', 'diffC']
lambdas_Bikes = [322, 5800, 127000, 1250000]
lambdas_Danger_de_Mort = [380, 8750, 140000, 1075800]
lambdas_Fountain_Vincent = [349, 7500, 158000, 1580000]
lambdas_Stone_Pillars_Outside = [317, 5300, 106000, 1055000]

exec_name = './mule-decoder-bin'

for lf in lf_dataset:
	for prediction in prediction_list:
		if lf == 'Bikes':
			for _lambda in lambdas_Bikes:
				cmd1 = '../' + exec_name + ' -i /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + prediction + '_' + str(_lambda) + '.LF '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				cmd3 = '-mule /home/douglascorrea/GitHub/vm-2.1-prediction/ -prediction ' + prediction
				print(cmd1 + cmd2 + cmd3)
				cmd = cmd1 + cmd2 + cmd3
				os.system(cmd)

				cmd = 'mkdir /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
				cmd = 'mv *.ppm /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
		elif lf == 'Danger_de_Mort':
			for _lambda in lambdas_Danger_de_Mort:
				cmd1 = '../' + exec_name + ' -i /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + prediction + '_' + str(_lambda) + '.LF '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				cmd3 = '-mule /home/douglascorrea/GitHub/vm-2.1-prediction/ -prediction ' + prediction
				print(cmd1 + cmd2 + cmd3)
				cmd = cmd1 + cmd2 + cmd3
				os.system(cmd)
				
				cmd = 'mkdir /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
				cmd = 'mv *.ppm /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
		elif lf == 'Fountain_Vincent2':
			for _lambda in lambdas_Fountain_Vincent:
				cmd1 = '../' + exec_name + ' -i /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + prediction + '_' + str(_lambda) + '.LF '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				cmd3 = '-mule /home/douglascorrea/GitHub/vm-2.1-prediction/ -prediction ' + prediction
				print(cmd1 + cmd2 + cmd3)
				cmd = cmd1 + cmd2 + cmd3
				os.system(cmd)

				cmd = 'mkdir /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
				cmd = 'mv *.ppm /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
		elif lf == 'Stone_Pillars_Outside':
			for _lambda in lambdas_Stone_Pillars_Outside:
				cmd1 = '../' + exec_name + ' -i /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + prediction + '_' + str(_lambda) + '.LF '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				cmd3 = '-mule /home/douglascorrea/GitHub/vm-2.1-prediction/ -prediction ' + prediction
				print(cmd1 + cmd2 + cmd3)
				cmd = cmd1 + cmd2 + cmd3
				os.system(cmd)

				cmd = 'mkdir /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
				cmd = 'mv *.ppm /home/douglascorrea/light-field/resultados_prediction/' + lf + '/decoding/' + prediction + '_' + str(_lambda) + '/ '
				print(cmd)
				os.system(cmd)
			

import os

lf_dataset = ['Bikes', 'Danger_de_Mort', 'Fountain_Vincent2', 'Stone_Pillars_Outside']
lambdas_Bikes = [322, 5800, 127000, 1250000]
lambdas_Danger_de_Mort = [380, 8750, 140000, 1075800]
lambdas_Fountain_Vincent = [349, 7500, 158000, 1580000]
lambdas_Stone_Pillars_Outside = [317, 5300, 106000, 1055000]

exec_names = ['./mule-encoder-bin_MuLE']

for lf in lf_dataset:
	for name in exec_names:
		if lf == 'Bikes':
			for _lambda in lambdas_Bikes:
				alg_name = name.split('_')[1]
				cmd1 = '../' + name + ' -lf /home/douglascorrea/light-field/dataset/Lenslet/'+ lf + '/ '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF '
				cmd3 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
				cmd4 = '-lambda ' + str(_lambda)
				print(cmd1 + cmd2 + cmd3 + cmd4)

				cmd = cmd1 + cmd2 + cmd3 + cmd4
				os.system(cmd)
		elif lf == 'Danger_de_Mort':
			for _lambda in lambdas_Danger_de_Mort:
				alg_name = name.split('_')[1]
				cmd1 = '../' + name + ' -lf /home/douglascorrea/light-field/dataset/Lenslet/'+ lf + '/ '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF '
				cmd3 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
				cmd4 = '-lambda ' + str(_lambda)
				print(cmd1 + cmd2 + cmd3 + cmd4)

				cmd = cmd1 + cmd2 + cmd3 + cmd4
				os.system(cmd)
		elif lf == 'Fountain_Vincent2':
			for _lambda in lambdas_Fountain_Vincent:
				alg_name = name.split('_')[1]
				cmd1 = '../' + name + ' -lf /home/douglascorrea/light-field/dataset/Lenslet/'+ lf + '/ '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF '
				cmd3 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
				cmd4 = '-lambda ' + str(_lambda)
				print(cmd1 + cmd2 + cmd3 + cmd4)

				cmd = cmd1 + cmd2 + cmd3 + cmd4
				os.system(cmd)
		elif lf == 'Stone_Pillars_Outside':
			for _lambda in lambdas_Stone_Pillars_Outside:
				alg_name = name.split('_')[1]
				cmd1 = '../' + name + ' -lf /home/douglascorrea/light-field/dataset/Lenslet/'+ lf + '/ '
				cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF '
				cmd3 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
				cmd4 = '-lambda ' + str(_lambda)
				print(cmd1 + cmd2 + cmd3 + cmd4)

				cmd = cmd1 + cmd2 + cmd3 + cmd4
				os.system(cmd)
			

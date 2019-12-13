import os

lf_dataset = ['Bikes']
lambdas_bikes = [1250000, 127000, 5800, 322]
#lambdas_Danger_de_Mort = [380, 5800, 127000, 1250000]
exec_names = ['./mule-encoder-bin_H']

for lf in lf_dataset:
	for name in exec_names:
		for _lambda in lambdas_bikes:
			alg_name = name.split('_')[1]
			cmd1 = '../' + name + ' -lf /home/douglascorrea/light-field/dataset/Lenslet/'+ lf + '/ '
			cmd2 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/bitstream/output_' + alg_name + '_' + str(_lambda) + '.LF '
			cmd3 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
			cmd4 = '-lambda ' + str(_lambda)
			print(cmd1 + cmd2 + cmd3 + cmd4)

			cmd = cmd1 + cmd2 + cmd3 + cmd4
			os.system(cmd)


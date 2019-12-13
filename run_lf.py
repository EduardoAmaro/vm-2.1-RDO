import os
import sys

lf = sys.argv[1]
_lambda = sys.argv[2]
prediction = sys.argv[3]
split = sys.argv[4]
evaluateOptimumBitPlane = sys.argv[5]
superiorBitPlane = sys.argv[6]
inferiorBitPlane = sys.argv[7]
performRDO = sys.argv[8]

cmd1 = 'make all -j 6 && '
cmd2 = 'time ./mule-encoder-bin '
cmd3 = '-lf /home/douglascorrea/light-field/dataset/Lenslet/' + lf + '/ '
cmd4 = '-o /home/douglascorrea/light-field/resultados_prediction/' + lf + '/coding/' + 'output_' + prediction + '_' + _lambda + '_' + split + '_' + evaluateOptimumBitPlane + '_' + superiorBitPlane + '_' + inferiorBitPlane + '.LF '
cmd5 = '-cf /home/douglascorrea/GitHub/vm-2.1-prediction/MULE_CFGs/Bikes/I01_Bikes_22016.json '
cmd6 = '-lambda ' + _lambda + ' '
cmd7 = '-prediction ' + prediction + ' '
cmd8 = '-split ' + split + ' '
cmd9 = '-evaluateOptimumBitPlane ' + evaluateOptimumBitPlane + ' '
cmd10 = '-superiorBitPlane ' + superiorBitPlane + ' '
cmd11 = '-inferiorBitPlane ' + inferiorBitPlane + ' '
cmd12 = '-performRDO ' + performRDO

print(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6 + cmd7 + cmd8 + cmd9 + cmd10 + cmd11 + cmd12)
os.system(cmd1 + cmd2 + cmd3 + cmd4 + cmd5 + cmd6 + cmd7 + cmd8 + cmd9 + cmd10 + cmd11 + cmd12)



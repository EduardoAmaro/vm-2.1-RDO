mule_mag_error = 0
mule_mag = 0
mule_distortion = 0
dr_mag_error = 0
dr_mag = 0
dr_distortion = 0

with open('../magnitude_file_MuLE.txt') as fp_MuLE:
	line = fp_MuLE.readline()
	print(line)
	mag_error = line.split(',')[0]
	mag = line.split(',')[1]
	distortion = line.split(',')[3]
	mule_mag_error += int(mag_error)
	mule_mag += int(mag)
	mule_distortion = float(distortion)

with open('../magnitude_file_DR.txt') as fp_DR:
	line = fp_DR.readline()
	mag_error = line.split(',')[0]
	mag = line.split(',')[1]
	distortion = line.split(',')[3]
	dr_mag_error += int(mag_error)
	dr_mag += int(mag)
	dr_distortion = float(distortion)

print('MuLE-[mag_error, mag, distortion]: ' + str(mule_mag_error) + ', ' + str(mule_mag) + ', ' + str(distortion))
print('DR-[mag_error, mag, distortion]: ' + str(dr_mag_error) + ', ' + str(dr_mag) + ', ' + str(distortion))
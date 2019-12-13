import math

matrix = [	[6,6,4,6,3,6,4,6,3,6,4,6,6],
			[6,5,6,5,6,5,6,5,6,5,6,5,6],
			[4,6,2,6,4,6,2,6,4,6,2,6,4],
			[6,5,6,5,6,5,6,5,6,5,6,5,6],
			[3,6,4,6,3,6,4,6,3,6,4,6,3],
			[6,5,6,5,6,5,6,5,6,5,6,5,6],
			[4,6,2,6,4,6,1,6,4,6,2,6,4],
			[6,5,6,5,6,5,6,5,6,5,6,5,6],
			[3,6,4,6,3,6,4,6,3,6,4,6,3],
			[6,5,6,5,6,5,6,5,6,5,6,5,6],
			[4,6,2,6,4,6,2,6,4,6,2,6,4],
			[6,5,6,5,6,5,6,5,6,5,6,5,6],
			[6,6,4,6,3,6,4,6,3,6,4,6,6]]

l1 = []
l2 = []
l3 = []
l4 = []
l5 = []
l6 = []

neighbors_dict =	{

}

def calculate_distance(index):
	l2_neighbors = []
	l3_neighbors = []
	l4_neighbors = []
	l5_neighbors = []
	l6_neighbors = []

	if matrix[index[0]][index[1]] == 2:
		select_minimun_neighbors(index, l2_neighbors, 2)
	if matrix[index[0]][index[1]] == 3:
		for i in range(0, 13):
			for j in range(0, 13):
				if matrix[i][j] < 3:
					l3_neighbors.append([i, j])
		select_minimun_neighbors(index, l3_neighbors, 3)
		#neighbors_dict[str(index)] = l3_neighbors

	if matrix[index[0]][index[1]] == 4:
		for i in range(0, 13):
			for j in range(0, 13):
				if matrix[i][j] < 4:
					l4_neighbors.append([i, j])
		select_minimun_neighbors(index, l4_neighbors, 4)
		#neighbors_dict[str(index)] = l4_neighbors
	
	if matrix[index[0]][index[1]] == 5:
		for i in range(0, 13):
			for j in range(0, 13):
				if matrix[i][j] < 5:
					l5_neighbors.append([i, j])
		select_minimun_neighbors(index, l5_neighbors, 5)
		#neighbors_dict[str(index)] = l5_neighbors

	if matrix[index[0]][index[1]] == 6:
		for i in range(0, 13):
			for j in range(0, 13):
				if matrix[i][j] < 6:
					l6_neighbors.append([i, j])
		select_minimun_neighbors(index, l6_neighbors, 6)
		#neighbors_dict[str(index)] = l6_neighbors
	
	#math.sqrt((i - index[0])**2 + (j - index[1])**2)

def select_minimun_neighbors(index, neighbors_list, level):
	best_neighbors = []
	l2_neighbors = []
	l3_neighbors = []
	l4_neighbors = []
	l5_neighbors = []

	if level == 2:
		best_neighbors.append([6,6])
	if level == 3:
		for neighbor_index in neighbors_list:
			if matrix[neighbor_index[0]][neighbor_index[1]] == 2:
				l2_neighbors.append(neighbor_index)
		
		minimum_distance_l2 = math.sqrt((l2_neighbors[0][0] - index[0])**2 + (l2_neighbors[0][1] - index[1])**2)

		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l2:
				minimum_distance_l2 = distance
		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l2:
				best_neighbors.append(neighbor_index)

	if level == 4:
		for neighbor_index in neighbors_list:
			if matrix[neighbor_index[0]][neighbor_index[1]] == 3:
				l3_neighbors.append(neighbor_index)
			if matrix[neighbor_index[0]][neighbor_index[1]] == 2:
				l2_neighbors.append(neighbor_index)
		
		minimum_distance_l3 = math.sqrt((l3_neighbors[0][0] - index[0])**2 + (l3_neighbors[0][1] - index[1])**2)
		minimum_distance_l2 = math.sqrt((l2_neighbors[0][0] - index[0])**2 + (l2_neighbors[0][1] - index[1])**2)
		for neighbor_index in l3_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l3:
				minimum_distance_l3 = distance
		for neighbor_index in l3_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l3:
				best_neighbors.append(neighbor_index)

		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l2:
				minimum_distance_l2 = distance

		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l2:
				best_neighbors.append(neighbor_index)

	if level == 5:
		for neighbor_index in neighbors_list:
			if matrix[neighbor_index[0]][neighbor_index[1]] == 4:
				l4_neighbors.append(neighbor_index)
			if matrix[neighbor_index[0]][neighbor_index[1]] == 3:
				l3_neighbors.append(neighbor_index)
			if matrix[neighbor_index[0]][neighbor_index[1]] == 2:
				l2_neighbors.append(neighbor_index)
		
		minimum_distance_l4 = math.sqrt((l4_neighbors[0][0] - index[0])**2 + (l4_neighbors[0][1] - index[1])**2)
		minimum_distance_l3 = math.sqrt((l3_neighbors[0][0] - index[0])**2 + (l3_neighbors[0][1] - index[1])**2)
		minimum_distance_l2 = math.sqrt((l2_neighbors[0][0] - index[0])**2 + (l2_neighbors[0][1] - index[1])**2)
		for neighbor_index in l4_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l4:
				minimum_distance_l4 = distance
		for neighbor_index in l4_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l4:
				best_neighbors.append(neighbor_index)

		for neighbor_index in l3_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l3:
				minimum_distance_l3 = distance
		for neighbor_index in l3_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l3:
				best_neighbors.append(neighbor_index)

		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l2:
				minimum_distance_l2 = distance
		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l2:
				best_neighbors.append(neighbor_index)

	if level == 6:
		for neighbor_index in neighbors_list:
			if matrix[neighbor_index[0]][neighbor_index[1]] == 5:
				l5_neighbors.append(neighbor_index)
			if matrix[neighbor_index[0]][neighbor_index[1]] == 4:
				l4_neighbors.append(neighbor_index)
			if matrix[neighbor_index[0]][neighbor_index[1]] == 3:
				l3_neighbors.append(neighbor_index)
			if matrix[neighbor_index[0]][neighbor_index[1]] == 2:
				l2_neighbors.append(neighbor_index)
		
		minimum_distance_l5 = math.sqrt((l5_neighbors[0][0] - index[0])**2 + (l5_neighbors[0][1] - index[1])**2)
		minimum_distance_l4 = math.sqrt((l4_neighbors[0][0] - index[0])**2 + (l4_neighbors[0][1] - index[1])**2)
		minimum_distance_l3 = math.sqrt((l3_neighbors[0][0] - index[0])**2 + (l3_neighbors[0][1] - index[1])**2)
		minimum_distance_l2 = math.sqrt((l2_neighbors[0][0] - index[0])**2 + (l2_neighbors[0][1] - index[1])**2)

		for neighbor_index in l5_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l5:
				minimum_distance_l5 = distance
		for neighbor_index in l5_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l5:
				best_neighbors.append(neighbor_index)

		for neighbor_index in l4_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l4:
				minimum_distance_l4 = distance
		for neighbor_index in l4_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l4:
				best_neighbors.append(neighbor_index)

		for neighbor_index in l3_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l3:
				minimum_distance_l3 = distance
		for neighbor_index in l3_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l3:
				best_neighbors.append(neighbor_index)

		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance < minimum_distance_l2:
				minimum_distance_l2 = distance
		for neighbor_index in l2_neighbors:
			distance = math.sqrt((neighbor_index[0] - index[0])**2 + (neighbor_index[1] - index[1])**2)
			if distance == minimum_distance_l2:
				best_neighbors.append(neighbor_index)
	
	#print(str(index) + ": " + str(best_neighbors) + " - len: " + str(len(best_neighbors)))
	#print()
	#generate_text_coding(index, best_neighbors, level)
	generate_text_decoding(index, best_neighbors, level)
	#neighbors_dict[key] = best_neighbors


def generate_text_coding(index, best_neighbors, level):
	weight = 1/(len(best_neighbors))
	#print(str(index) + ": ")
	str2 = ""
	str1 = "residueBlock->mPixel[" + str(index[0]) + "][" + str(index[1]) + "][viewLine][viewColumn] = origBlock->mPixel[" + str(index[0]) + "][" + str(index[1]) + "][viewLine][viewColumn] - (origBlock->mPixel[6][6][viewLine][viewColumn]"
	for neighbor in best_neighbors:
		#print(best_neighbors)
		str2 += " + residueBlock->mPixel[" + str(neighbor[0]) + "][" + str(neighbor[1]) + "][viewLine][viewColumn]*" + str(round(weight,2))
	
	print(str1 + str2 + ");")


def generate_text_decoding(index, best_neighbors, level):
	weight = 1/len(best_neighbors)
	#print(str(index) + ": ")
	str2 = ""
	str1 = "reconstructedBlock->mPixel[" + str(index[0]) + "][" + str(index[1]) + "][viewLine][viewColumn] = residueBlock->mPixel[" + str(index[0]) + "][" + str(index[1]) + "][viewLine][viewColumn] + residueBlock->mPixel[6][6][viewLine][viewColumn]"
	for neighbor in best_neighbors:
		str2 += " + residueBlock->mPixel[" + str(neighbor[0]) + "][" + str(neighbor[1]) + "][viewLine][viewColumn]*" + str(round(weight,2))
	
	print(str1 + str2 + ";")


# residueBlock->mPixel[verticalView][horizontalView][viewLine][viewColumn] =
# 								origBlock->mPixel[verticalView][horizontalView][viewLine][viewColumn] - 
# 								(origBlock->mPixel[verticalView][0][viewLine][viewColumn] + residuesSum);

for i in range(0, 13):
	for j in range(0, 13):
		if matrix[i][j] == 1:
			l1.append([i,j])
		elif matrix[i][j] == 2:
			l2.append([i,j])
		elif matrix[i][j] == 3:
			l3.append([i,j])
		elif matrix[i][j] == 4:
			l4.append([i,j])
		elif matrix[i][j] == 5:
			l5.append([i,j])
		elif matrix[i][j] == 6:
			l6.append([i,j])

for view in l1:
	calculate_distance(view)
for view in l2:
	calculate_distance(view)
for view in l3:
	calculate_distance(view)
for view in l4:
	calculate_distance(view)
for view in l5:
	calculate_distance(view)
for view in l6:
	calculate_distance(view)

# for key in neighbors_dict:
# 	select_minimun_neighbors(key)
# 	#print(key + ": " + str(neighbors_dict[key]) + '\n')
import matplotlib
import matplotlib.pyplot as plt
import numpy as np


labels = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30]
mule_coefficients = [220773, 163967, 310402, 664345, 1262911, 2576214, 5068997, 9693720, 17144613, 25235680, 27712632, 22079587, 14005831, 7293256, 3079632, 1241889, 586298, 320071, 171106, 72031, 26757, 9381, 2915, 335, 7, 0, 0, 0, 0, 0, 0]
diffRDC_coefficients = [276635, 156838, 268522, 609764, 1144445, 2342994, 4598634, 8785692, 15717448, 24340171, 29920220, 26307360, 15275209, 5961591, 1899970, 682533, 325176, 185423, 100880, 37401, 6228, 216, 0, 0, 0, 0, 0, 0, 0, 0, 0]

x = np.arange(len(labels))  # the label locations
width = 0.35  # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, mule_coefficients, width, label='MuLE')
rects2 = ax.bar(x + width/2, diffRDC_coefficients, width, label='diffRDC')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Number of coefficiente per BitPlane')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()
plt.yscale('log')

# def autolabel(rects):
#     """Attach a text label above each bar in *rects*, displaying its height."""
#     for rect in rects:
#         height = rect.get_height()
#         ax.annotate('{}'.format(height),
#                     xy=(rect.get_x() + rect.get_width() / 2, height),
#                     xytext=(0, 3),  # 3 points vertical offset
#                     textcoords="offset points",
#                     ha='center', va='bottom')


# autolabel(rects1)
# autolabel(rects2)

print(sum(mule_coefficients))
print(sum(diffRDC_coefficients))
fig.tight_layout()

plt.show()
import networkx
import matplotlib.pyplot as plt
import csv

def remove_junk(li):
    new_list = list()
    for ele in li:
        if ele not in {' ', ''}:
            new_list.append(ele)
    return new_list

path_name = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/unique_authors.csv'
new_list = list()

# Put the unique names into a list used for comparisons
with open(path_name, encoding='utf-8-sig') as f:
    readCSV = csv.reader(f, delimiter=',')
    for row in readCSV:
        for element in row:
            new_list.append(element)

# Remove potential spaces
list_wo_spaces = remove_junk(new_list)



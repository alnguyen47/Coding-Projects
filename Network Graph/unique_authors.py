import csv

# Create set of unique authors
unique_authors = {'Bradford G Orr'}

# Convert list into tuple
def convert_to_tuple(li):
    return tuple(li)

loc = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/data.csv'
path_name = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/unique_authors.csv'
new_list = list()

with open(loc, encoding='utf-8-sig') as f:
    readCSV = csv.reader(f, delimiter=',')
    for row in readCSV:
        new_tuple = convert_to_tuple(row)
        for element in new_tuple:
            unique_authors.add(element)

# Write into a new datafile
with open(path_name, 'a', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(unique_authors)

print('Finished')

# Read the unique authors and compare them to each of the other authors in the list.
# If they have worked together, then it will be denoted as a 1
# Otherwise it will be denoted as a 0.

import csv

# Function to remove spaces from the list
def remove_spaces(li):
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

# Remove potential spaces and the list will contain the unique authors
list_wo_spaces = remove_spaces(new_list)

# Iterate through the list and compare each author with the authors in the list
filtered_data_path = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/filtered_data.csv'
matrix_path = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/matrix.csv'



with open(filtered_data_path, encoding='utf-8-sig') as f:
    for author in list_wo_spaces:
        list_of_ones_zeroes = list()
        for compared_authors in list_wo_spaces:
            # Create bool value
            is_one = False
            readCSV = csv.reader(f, delimiter=',')
            # Need to reset the stream to read from top of file again
            for row in readCSV:
                new_list = list()
                new_list = remove_spaces(row)
                # If the two names are in the same line then it means they have worked together
                if (author in new_list) and (compared_authors in new_list):
                    list_of_ones_zeroes.append(1)
                    is_one = True
                    break
            f.seek(0)

            if not is_one:
                list_of_ones_zeroes.append(0)

        # Once that author has been compared to every other author, then append the list to the
        # matrix.csv file
        with open(matrix_path, 'a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(list_of_ones_zeroes)


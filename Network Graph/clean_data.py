# Clean the data in data.csv
import csv

# Function to remove spaces from the list
def remove_spaces(li):
    new_list = list()
    for ele in li:
        if ele not in {' ', ''}:
            new_list.append(ele)
    return new_list

def find_name_using_split(name, list_of_names):
    new_name = name.split()
    i = 0
    index = 0

    for elements in list_of_names:
        fname_and_lname = elements.split()
        if (new_name[-1].lower() == fname_and_lname[-1].lower()) and \
                (name[0][0].lower() == fname_and_lname[0][0].lower()):
            index = i
        i += 1
    return index


path_name = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/unique_authors.csv'
new_list = list()

# Put the unique names into a list used for comparisons
with open(path_name, encoding='utf-8-sig') as f:
    readCSV = csv.reader(f, delimiter=',')
    for row in readCSV:
        for element in row:
            new_list.append(element)

# Remove potential spaces
list_wo_spaces = remove_spaces(new_list)
dummy_list_wo_spaces = list()

# Dummy list to have lower case authors
for element in list_wo_spaces:
     dummy_list_wo_spaces.append(element.lower())
print(dummy_list_wo_spaces)

# Open the data file to read the data
path_name1 = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/data.csv'
path_name2 = 'C:/Users/alber/Desktop/PyCharm Projects/Network Graph/filtered_data.csv'

with open(path_name1, encoding='utf-8-sig') as f:
    readerCSV = csv.reader(f, delimiter=',')
    for row in readerCSV:
        # Compare names
        row_list = remove_spaces(row)
        print(row_list)
        name = list()
        for elements in row_list:
            # First check if the name is in the unique authors list
            if elements.lower() in dummy_list_wo_spaces:  # keeps repeating too many times, FIX
                index = dummy_list_wo_spaces.index(elements.lower())
                name.append(list_wo_spaces[index])
            else:
                # If the string does not match perfectly, look at the last name, first name
                index = find_name_using_split(elements, list_wo_spaces)
                name.append(list_wo_spaces[index])

        with open(path_name2, 'a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(name)

print('Finished')






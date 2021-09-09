import scholarly
import csv

# Get Prof Orr's publications
search_query = scholarly.search_author('Bradford Orr')
author = next(search_query).fill()

# Iterate through each article and pull out the authors
all_authors = list()

# Path name to where data.csv is stored, change to specific path
path_name = 'C:/Users/alber\Desktop/PyCharm Projects/Network Graph/data.csv'

# Write the headings
headings = 'Article_Number Authors'
headings = headings.split()
with open(path_name, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(headings)

for i in range(0, 50):
    pub = author.publications[i].fill()
    authors = pub.bib['author'].split(' and ')
    # Insert number to keep track of which publication it is
    authors.insert(0, i)
    # Put the list into the data.csv file
    with open(path_name, 'a', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(authors)


# Finished
print("Finished.")






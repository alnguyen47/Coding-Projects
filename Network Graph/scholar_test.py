import scholarly
import csv

# Get the desired author
search_query = scholarly.search_author('Bradford Orr')
author = next(search_query).fill()

# Get one author
authors = list()

path_name = 'C:/Users/alber\Desktop/PyCharm Projects/Network Graph/unique_authors.csv'

# Write the headings
headings = 'Article_Number Authors'
headings = headings.split()
with open(path_name, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(headings)


pub = author.publications[123].fill()
print(pub)

authors = pub.bib['author'].split(' and ')

# Put the list into the data.csv file
authors.insert(0, 123)

with open(path_name, 'a', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(authors)


# Finished
print("Finished.")





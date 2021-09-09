import scholarly
import csv

# Exact same thing as scholars.py, but used it to put into second csv file to then append to data.csv

# Get Prof Orr's name
search_query = scholarly.search_author('Bradford Orr')
author = next(search_query).fill()

# Iterate through each article and pull out the authors
all_authors = list()

# 226 publications, divide into multiple files to extract into Excel files

path_name = 'C:/Users/alber\Desktop/PyCharm Projects/Network Graph/unique_authors.csv'

# Write the headings
headings = 'Article_Number Authors'
headings = headings.split()
with open(path_name, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(headings)

# Gets authors from article 50 to 99
for i in range(190, 227):
    pub = author.publications[i].fill()
    authors = pub.bib['author'].split(' and ')
    # Put the list into the data.csv file
    authors.insert(0, i)

    with open(path_name, 'a', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(authors)


# Finished
print("Finished.")






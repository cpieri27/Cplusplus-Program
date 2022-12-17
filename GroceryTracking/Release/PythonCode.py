import re
import string

# Create dictionary from the input file where key is unique item name and value
# is the frequency in which that key occurs in the file
def ItemDictFromFile():
    itemDict = {}
    itemFile = 'CS210_Project_Three_Input_File.txt'

    # Open the file, add each unique key to the dictionary and assign value to 1
    # If key already in dictionary, increase value by 1 instead
    with open(itemFile) as file:
        for line in file:
            item = line.strip().title()

            if item in itemDict:
                itemDict[item] += 1
            else:
                itemDict[item] = 1

    return itemDict

# Read file, identify item and the frequencies that it occurs, then print
# a table of that information to the screen
def OverallFrequency():
    itemDict = ItemDictFromFile()

    print("Item\t\t     Item Frequency\n")

    # Print each line with unique item name and its frequency
    for item in itemDict:
        print(f"{item:20} {itemDict[item]}")

    print()

# Read file, find if userItem is in the file, then return the frequency 
# in which that item occurs
def IndividualFrequency(userItem):
    itemDict = ItemDictFromFile()
    titleCaseItem = userItem.title()

    # Check if the item is in the dictionary.  If so, return frequency
    # If not, return 0
    if titleCaseItem in itemDict:
        return itemDict[titleCaseItem]
    else:
        return 0

# Read file, create dictionary with keys as unique items and values as
# the frequency in which they occur.  Create "frequency.dat" file with
# that information.
def Histogram():
    itemDict = ItemDictFromFile()
    
    with open('frequency.dat', 'w') as file:
        for item in itemDict:
            file.write(f"{item} {itemDict[item]}\n")

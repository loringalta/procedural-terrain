import math
def main():
    total = 0.0
    length = 0.0
    average = 0.0
    avg = 0.0
    variance = 0.0
    sd = 0.0

    try:
        #Get the name of a file
        filename = raw_input('Enter a file name: ')
        numbers = []
        #Open the file
        infile = open(filename, 'r')
        #Read values from file and compute average
        for line in infile:
            amount = float(line.rstrip("\n"))
            total += amount
            length = length + 1
        average = total / length
        infile.close()
        
        infile = open(filename, 'r')

        for line in infile: 
            amount = float(line.rstrip("\n"))
            variance += (amount-average)**2

        sd = (variance/length)**(1.0/2.0)

        #Close the file
        infile.close()

        #Print the amount of numbers in file and average
        print 'There were', length, 'numbers in the file.'
        print 'average', format(average, ',.12f')
        print 'standard deviation', format(sd, ',.12f')

    except IOError:
        print 'An error occurred trying to read the file.' 

    except ValueError:
        print 'Non-numeric data found in the file'

    except:
        print('An error has occurred')
main()


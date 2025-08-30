import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) == 3:
        filename_CSV = sys.argv[1]
        filename_text = sys.argv[2]
    else:
        print("Error")

    # TODO: Read database file into a variable
    rows = []
    with open(filename_CSV) as csvfile:
        reader = csv.DictReader(csvfile)
        STRs = reader.fieldnames[1:]
        for row in reader:
            rows.append(row)

    # TODO: Read DNA sequence file into a variable
    with open(filename_text) as f:
        dna_sequence = f.read()

    # TODO: Find longest match of each STR in DNA sequence
    L = {}
    for STR in STRs:
        L[STR] = longest_match(dna_sequence, STR)

    # TODO: Check database for matching profiles
    found = False
    for row in rows:
        name = row.pop('name')
        for k in row.keys():
            row[k] = int(row[k])
        if row == L:
            print(name)
            found = True
            break
    if not found:
        print('No match')


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

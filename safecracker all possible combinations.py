import csv

with open('combinations.csv', mode='w') as csv_file:
    writer = csv.writer(csv_file)
    for i in range(100):
        for j in range(100):
            for k in range(100):
                # Ensure at least one number is X
                if i == 40 or j == 40 or k == 40:
                    writer.writerow([i, j, k])

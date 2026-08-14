# TODO

from cs50 import get_float

while True:
    cents = get_float("Change: ")
    if cents > 0:
        break

cents = round(cents * 100)

counter = 0

# Quarters
while cents >= 25:
    cents = cents - 25
    counter += 1

# Dimes
while cents >= 10:
    cents = cents - 10
    counter += 1

# Nickels
while cents >= 5:
    cents = cents - 5
    counter += 1

# Pennies
while cents >= 1:
    cents = cents - 1
    counter += 1

print("Total: ", counter)

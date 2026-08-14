# TODO

from cs50 import get_string

# this is all derived from the c version earlier in the course

text = get_string("Text: ")

letters = 0
words = 1
sentences = 0

for i in text:
    if i.isalpha():
        letters += 1
    elif i == " ":
        words += 1
    elif i == "." or i == "!" or i == "?":
        sentences += 1

    # index = 0.0588 * L - 0.296 * S - 15.8
    # where L is the average number of letters per 100 words in the text,
    # and S is the average number of sentences per 100 words in the text.
index = 0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade ", round(index))

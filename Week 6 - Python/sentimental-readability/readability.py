from cs50 import get_string


def count_letters(text):
    return sum(1 for char in text if char.isalpha())


def count_words(text):
    return sum(1 for char in text if char == " ") + 1


def count_sentences(text):
    return sum(1 for char in text if (char == '.' or char == '?' or char == '!'))


text = get_string("Text: ")

L = count_letters(text) * 100.00 / count_words(text)
S = count_sentences(text) * 100.00 / count_words(text)
index = round(0.0588 * L - 0.296 * S - 15.8)

if index <= 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")

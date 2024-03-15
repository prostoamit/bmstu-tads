import sys
from english_words import get_english_words_set

lowerset = list(get_english_words_set(["web2"], lower=True))


def main():

    for size in range(10000, 100000, 5000):
        with open(f"data/random_{size}.txt", "w") as out_file:
            out_file.write(' '.join(lowerset[:size]))
            

if __name__ == "__main__":
    main()

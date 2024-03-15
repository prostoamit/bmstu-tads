import sys
from english_words import get_english_words_set

lowerset = list(get_english_words_set(["web2"], lower=True))

def main():


    for size in range(0, 100001, 5000):
        file_name = f"data/rand_{size}.txt"
        result = lowerset[:size - 1]
        result.insert(size // 2, "owaro")
        with open(file_name , "w") as out_file:
            out_file.write(' '.join(result))
            print("\t\"" + file_name + "\",")
    for size in range(0, 100001, 5000):
            print(f"\t{size},")


if __name__ == "__main__":
    main()

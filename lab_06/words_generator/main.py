import sys
from english_words import get_english_words_set

lowerset = list(get_english_words_set(["web2"], lower=True))

def main():

    if len(sys.argv) != 3:
        return 1

    file_arg = sys.argv[1]
    size_arg = sys.argv[2]
    size = 0


    try:
        size = int(size_arg)
    except ValueError:
        return 2
    except Exception:
        return 255
    else:
        if size <= 0:
            return 3


    with open(file_arg, "w") as out_file:
        out_file.write(' '.join(lowerset[:size]))


if __name__ == "__main__":
    main()

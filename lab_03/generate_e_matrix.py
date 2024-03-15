file_name = input("Введите имя файла: ")
size = int(input("Введите размер единичной матрицы: "))

with open(file_name, "w") as file:
    file.write(f'{size} {size}\n')

    for i in range(size):
        for j in range(size):
            file.write("1 " if i == j else "0 ")
        file.write("\n")

file_name = input("Введите имя файла: ")
size = int(input("Введите размер полностью заполненной последовательно квадратной матрицы: "))

sum = 1
with open(file_name, "w") as file:
    file.write(f'{size} {size}\n')

    for i in range(size):
        for j in range(size):
            file.write(f"{sum} ")
            sum += 1
        file.write("\n")

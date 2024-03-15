# Компилятор
CC := gcc
# Команда для удаления файлов.
RM := rm -rf

# Директория исходных файлов.
DSRC := ./src/
# Директория зависимостей.
DINC := ./inc/
# Директория выходных файлов.
DOUT := ./out/
# Директория с модульным тестированием.
DUNIT := ./unit_tests/
# Директория с функциональным тестированием.
DFUNC := ./func_tests/

# Имена исходных файлов (для собздания имён объектных файлов).
SRCS := $(wildcard $(DSRC)*.c)
UNITSRCS := $(wildcard $(DUNIT)*.c)
# Именя объектных файлов.
OUTS := $(SRCS:$(DSRC)%.c=$(DOUT)%.o)
UNITOUTS := $(UNITSRCS:$(DUNIT)%.c=$(DOUT)%.o)
# Объектные файлы (для сбора информации о покрытии)
CLEANOUTS := $(OUTS:$(DOUT)%.o=%.o)
# Имена d-файлов.
DFILES := $(OUTS:.o=.d)
DUNITFILES := $(UNITOUTS:.o=.d)

# Общие ключи компиляции.
CFLAGS := -std=gnu99 -Wall -Wextra -Wpedantic -Werror -Wfloat-equal -Wfloat-conversion -Wfloat-conversion -Wvla
# Дополнительные ключи компиляции.
CADDFLAGS = 
# Общие ключи компоновки.
LFLAGS = -lm
# Дополнительные ключи компоновки.
LADDFLAGS = 

.PHONY : debug
debug :: debug.lastbuildstate
debug :: CADDFLAGS = -g -O0
debug :: app.exe

.PHONY : release
release :: release.lastbuildstate 
release :: CADDFLAGS = -g0 -O3
release :: app.exe

.PHONY : coverage
coverage :: coverage.lastbuildstate
coverage :: CADDFLAGS = -O0 --coverage 
coverage :: LADDFLAGS = -lgcov 
coverage :: app.exe

.PHONY : valgrind
valgrind :: valgrind.lastbuildstate 
valgrind :: CADDFLAGS = -g -O0
valgrind :: app.exe

.PHONY : mult 
mult :: CADDFLAGS = -g3 -O0
mult :: csc_csr_multiplication.exe

.PHONY : rand
rand :: CADDFLAGS = -g3 -O0
rand :: random_filling.exe

.PHONY : research
research :: CADDFLAGS = -g3 -O0
research :: research.exe

.PHONY : func
func : 
	$(DFUNC)scripts/func_tests.sh -v

.PHONY : clean
clean :
	$(RM) ./*.exe $(DOUT) out.txt ./*.lastbuildstate

.PHONY : collect_coverage
collect_coverage : 
	gcov -r $(OUTS)
	mv *.gcov $(DOUT)
	lcov -q -c -d $(DOUT) -o $(DOUT)lcov_result.info
	lcov -l $(DOUT)lcov_result.info

app.exe : $(addprefix $(DOUT),mem_manager.o matrix.o csc_matrix.o csr_matrix.o arrays.o matrices_operations.o research.o input.o main.o)
	$(CC) $^ -o $@ $(LFLAGS) $(LADDFLAGS)

matrix_to_csc.exe : $(addprefix $(DOUT),mem_manager.o matrix.o csc_matrix.o arrays.o matrix_to_csc.o)
	$(CC) $^ -o $@ $(LFLAGS) $(LADDFLAGS)

csc_csr_multiplication.exe : $(addprefix $(DOUT),mem_manager.o matrix.o csc_matrix.o csr_matrix.o arrays.o matrices_operations.o csc_csr_multiplication.o)
	$(CC) $^ -o $@ $(LFLAGS) $(LADDFLAGS)

random_filling.exe : $(addprefix $(DOUT),mem_manager.o arrays.o matrix.o test_random_filling.o)
	$(CC) $^ -o $@ $(LFLAGS) $(LADDFLAGS)

research.exe : $(addprefix $(DOUT),mem_manager.o arrays.o matrix.o csc_matrix.o csr_matrix.o matrices_operations.o research.o test_research.o)
	$(CC) $^ -o $@ $(LFLAGS) $(LADDFLAGS)

# Создание объектных файлов из директории с исходниками.
$(DOUT)%.o : $(DSRC)%.c
	mkdir -p $(DOUT)
	$(CC) -I $(DINC) $(CFLAGS) $(CADDFLAGS) -c $< -o $@

$(DOUT)%.d : $(DSRC)%.c
	mkdir -p $(DOUT)
	echo -n $(DOUT) > $@
	$(CC) -I $(DINC) -MM $< >> $@

# Создание объектных файлов из директории с модульными тестами.
$(DOUT)%.o : $(DUNIT)%.c
	mkdir -p $(DOUT)
	$(CC) -I $(DINC) -I $(DUNIT) $(CFLAGS) $(CADDFLAGS) -c $< -o $@

$(DOUT)%.d : $(DUNIT)%.c
	mkdir -p $(DOUT)
	echo -n $(DOUT) > $@
	$(CC) -I $(DINC) -I $(DUNIT) -MM $< >> $@

%.lastbuildstate:
	make clean
	touch $@

ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
include $(DFILES) $(DUNITFILES)
endif

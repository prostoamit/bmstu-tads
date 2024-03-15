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
# Директория с функциональным тестированием.
DFUNC := ./func_tests/

# Имена исходных файлов (для собздания имён объектных файлов).
SRCS := $(wildcard $(DSRC)*.c)
# Именя объектных файлов.
OUTS := $(SRCS:$(DSRC)%.c=$(DOUT)%.o)
# Объектные файлы (для сбора информации о покрытии)
CLEANOUTS := $(OUTS:$(DOUT)%.o=%.o)
# Имена d-файлов.
DFILES := $(OUTS:.o=.d)

# Общие ключи компиляции.
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror -Wfloat-equal -Wfloat-conversion -Wfloat-conversion -Wvla
# Дополнительные ключи компиляции.
CADDFLAGS = 
# Общие ключи компоновки.
LFLAGS = -lm
# Дополнительные ключи компоновки.
LADDFLAGS = 

.PHONY : debug
debug :: debug.lastbuildstate
debug :: CADDFLAGS = -g
debug :: app.exe

.PHONY : release
release :: release.lastbuildstate 
release :: CADDFLAGS = -g0
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
	$(DFUNC)scripts/func_tests.sh --valgrind

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

app.exe : $(addprefix $(DOUT),main.o big_float.o big_int.o)
	$(CC) $^ -o $@ $(LFLAGS) $(LADDFLAGS)

# Создание объектных файлов из директории с исходниками.
$(DOUT)%.o : $(DSRC)%.c
	mkdir -p $(DOUT)
	$(CC) -I $(DINC) $(CFLAGS) $(CADDFLAGS) -c $< -o $@

$(DOUT)%.d : $(DSRC)%.c
	mkdir -p $(DOUT)
	echo -n $(DOUT) > $@
	$(CC) -I $(DINC) -MM $< >> $@


%.lastbuildstate:
	make clean
	touch $@

ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
include $(DFILES)
endif


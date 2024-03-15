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
# Все исходники.
SRCS := $(wildcard $(DSRC)*.c)
UNITSRCS := $(wildcard $(DUNIT)*.c)
# Именя o-файлов.
OUTS := $(SRCS:$(DSRC)%.c=$(DOUT)%.o)
UNITOUTS := $(UNITSRCS:$(DUNIT)%.c=$(DOUT)%.o)

CLEANOUTS := $(OUTS:$(DOUT)%.o=%.o)

# Имена d-файлов.
DFILES := $(OUTS:.o=.d)
DUNITFILES := $(UNITOUTS:.o=.d)
# Общие ключи компиляции.
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -Werror
# Дополнительные ключи компиляции.
CADDFLAGS = 
# Общие ключи линковки.
LFLAGS = 
# Дополнительные ключи линковки.
LADDFLAGS = 

.PHONY : debug
debug :: debug.lasbuildstate
debug :: CADDFLAGS = -g
debug :: app.exe

.PHONY : noopt
noopt :: noopt.lasbuildstate
noopt :: CADDFLAGS = -O0
noopt :: app.exe

.PHONY : release
release :: release.lasbuildstate 
release :: app.exe

.PHONY : coverage
coverage :: coverage.lasbuildstate
coverage :: CADDFLAGS = --coverage
coverage :: LADDFLAGS = -lgcov 
coverage :: app.exe

.PHONY : valgrind
valgrind :: valgrind.lasbuildstate 
valgrind :: CADDFLAGS = -g -O0
valgrind :: app.exe
	$(DFUNC)scripts/func_tests.sh --valgrind

.PHONY : test_merge
test_merge :: CADDFLAGS = -g3
test_merge :: test_merge.exe

.PHONY : func
func : app.exe
	$(DFUNC)scripts/func_tests.sh

.PHONY : clean
clean :
	$(RM) ./*.exe $(DOUT) out.txt ./*.lasbuildstate

.PHONY : collect_coverage
collect_coverage : 
	cd $(DOUT) && gcov -r $(CLEANOUTS)

app.exe : $(OUTS)
	$(CC) $^ -o $@ $(LFLAGS) $(LADDFLAGS)

test_merge.exe : $(DOUT)test_merge.o $(DOUT)mem_manager.o
	$(CC) $^ -g3 -o $@ $(LFLAGS) $(LADDFLAGS)

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

%.lasbuildstate:
	make clean
	touch $@

ifneq (clean,$(filter clean,$(MAKECMDGOALS)))
include $(DFILES) $(DUNITFILES)
endif

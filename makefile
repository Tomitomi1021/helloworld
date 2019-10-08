#ファイル定義
#TODO:ファイルを定義する。
TARGET=helloworld
OBJS=main.o

#ビルドの過程で生成されるファイル
PRODUCTS=*.o *.out

#コンパイラ設定
LIBS=
GCC_COMPILEOPTION=
GXX_COMPILEOPTION=
LINKOPTION=


.PHONY:all clean rebuild
all: $(TARGET)

#ターゲットの作成
$(TARGET):$(OBJS)
	g++ $(LINKOPTION) $^  $(LIBS) -o $@

#サフィックスルール
.SUFFIXES:.c .o .cpp
.c.o:
	gcc $(GCC_COMPILEOPTION) $< -c

.cpp.o:
	g++ $(GXX_COMPILEOPTION) $< -c

#ユーティリティコマンド定義
rebuild:
	make clean
	make

clean:
	rm -rf $(PRODUCTS) $(TARGET)

run:all
	./$(TARGET)

debug:all
	$(DEBUGGER) $(TARGET)

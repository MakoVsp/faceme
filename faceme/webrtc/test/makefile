CC = g++ #CC定义成gcc
DEFINES=
INCLUDEDIR=
OBJS= voipTest.o

CFLAGS = -g -L. -shared -fPIC
ECFLAGS = -g -L.
LDFLAGS = -leimwebrtc
TARGET = voipTest#生成的可执行文件
RM = rm -f                              #删除的命令

$(TARGET): $(OBJS)#TARGET目标，需要先生成OBJS目标
	$(CC) ${DEFINES} $(INCLUDEDIR) $(ECFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) #生成可执行文件
$(OBJS):voipTest.cpp
	$(CC) ${DEFINES} $(INCLUDEDIR) -c voipTest.cpp $(CFLAGS) $(LDFLAGS)

clean:                               #清理
	-$(RM) $(TARGET) $(OBJS)           #删除所有的目标文件和可执行文件

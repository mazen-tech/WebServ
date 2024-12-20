CC = c++
CFLAGS = -Wall -Wextra -Werror
# SRCS = src/main.cpp
# SRCS = src/main_jp_test.cpp
SRCS = src/main_content_length.cpp src/method_get.cpp src/read_conf.cpp src/server.cpp src/find_file.cpp

OBJDIR = obj
OBJS = $(SRCS:src/%.cpp=$(OBJDIR)/%.o)

EXEC = WebServer

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC)

re: fclean all

.PHONY: all clean fclean re

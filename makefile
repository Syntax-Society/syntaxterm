PKGS    := form ncurses tinfo
CC      := gcc
CFLAGS  := -g -Og -Wall -Wextra -std=c99 -no-pie $(shell pkg-config --cflags $(PKGS))
LDFLAGS := $(shell pkg-config --libs $(PKGS))
SQLFLAGS:= $(shell mariadb_config --include --libs)
RM      := rm

BIN     := $(notdir $(shell pwd))
SRC     := $(wildcard *.c)
OBJ     := $(SRC:.c=.o)
HEADERS := $(wildcard *.h)

SSH_HOST := syntax

SSH_PID := .ssh-tunnel.PID

.PHONY: all clean start-tunnel stop-tunnel

all: $(BIN)

run: start-tunnel $(BIN)
	./$(BIN)

start-tunnel: .ssh-tunnel.PID

stop-tunnel:
	@[ -e "$(SSH_PID)" ] && (kill "$(shell cat "$(SSH_PID)" 2>/dev/null)" ; rm -f "$(SSH_PID)") || echo "SSH tunnel not started"

.ssh-tunnel.PID:
	@echo "starting SSH tunnel..."
	@{ ssh -NL 3306:localhost:3306 "$(SSH_HOST)" & echo $$! > $@; }
	sleep 1

clean: stop-tunnel
	$(RM) -f *.o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN): $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(SQLFLAGS) -o $@

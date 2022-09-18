PKGS    := form ncurses tinfo
CC      := gcc
CFLAGS  := -g -Og -Wall -Wextra -std=c99 -no-pie $(shell pkg-config --cflags $(PKGS)) -I.
LDFLAGS := $(shell pkg-config --libs $(PKGS)) -I.
SQLFLAGS:= $(shell mariadb_config --include --libs)
RM      := rm

BIN     := $(notdir $(shell pwd))
SRC     := $(wildcard *.c */*.c)
OBJ     := $(SRC:.c=.o)
HEADERS := $(wildcard *.h)

ROWS=$(shell stty -a | grep -oE 'rows [0-9]+')
COLS=$(shell stty -a | grep -oE 'columns [0-9]+')

SSH_HOST := syntax

SSH_PID := .ssh-tunnel.PID

.PHONY: all clean start-tunnel stop-tunnel

all: $(BIN)

run: start-tunnel $(BIN)
	stty rows 25 cols 80 && ./$(BIN)
	stty sane $(ROWS) $(COLS)

start-tunnel: .ssh-tunnel.PID

stop-tunnel:
	@[ -e "$(SSH_PID)" ] && (kill "$(shell cat "$(SSH_PID)" 2>/dev/null)" ; rm -f "$(SSH_PID)") || echo "SSH tunnel not started"

.ssh-tunnel.PID:
	@echo "starting SSH tunnel..."
	@{ ssh -NL 3306:localhost:3306 "$(SSH_HOST)" & echo $$! > $@; }
	sleep 1

clean: stop-tunnel
	$(RM) -f $(OBJ) $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN): $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(SQLFLAGS) -o $@

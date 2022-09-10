# syntaxterm
Terminal application for the Tandberg terminal at Syntax Society

## Compiling

To compile on a Debian based system, you need at least the following:

 - libncurses-dev
 - libc6-dev
 - libmysqlclient-dev
 - gcc
 - make
 - pkg-config

To install these, you can use the following command:

```
sudo apt-get install gcc make pkg-config libncurses-dev libc6-dev libmysqlclient-dev
```

To compile the program, just execute `make` in the root of this project.

## Running

Runtime libraries that are needed are the following:

 - libncurses6
 - libmariadb3
 - libtinfo6

NOTE: These should be automatically installed when you install the compilation dependencies listed in the section above.

If you want to both compile and run the program in one go, execute `make run` in the root of this project.

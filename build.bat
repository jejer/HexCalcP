rplcomp aplet.s aplet.a
sasm -B -W -a aplet.l -o aplet.o aplet.a
sload -H aplet.m
del aplet.a
del aplet.l
del aplet.o
del aplet.lr
# Terminal
C frame of a shell that allows to write commands, reads them and executes them. It does this by employing pipes.
For instance, the command "ls -l < a.txt >b.txt !>c.tx" would result in:
a.txt:
a
b
c
b.txt: 3
c.txt:

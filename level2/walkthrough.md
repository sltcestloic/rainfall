# level2

On ne vois rien qui lance un shell, et à priori la seule fonction exploitable sera `gets` avec un buffer overflow

En utilisant entre autres [cette resource](<https://medium.com/codex/buffer-overflows-shellcode-and-memory-corruption-51f546ab00ab>) on comprend qu'on va devoir injecter un shellcode dans notre buffer pour faire exécuter du code arbitraire à notre exécutable pour lui faire lancer un shell
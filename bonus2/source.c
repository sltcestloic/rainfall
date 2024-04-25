int greetuser(char src)
{
  char dest[80];

  switch (language)
  {
    case 1:
      strcpy(dest, "Hyvää päivää ");
      break;
    case 2:
      strcpy(dest, "Goedemiddag! ");
      break;
    default:
      strcpy(dest, "Hello ");
      break;
  }

  strcat(dest, &src);
  return puts(dest);
}

int main(int argc, const char **argv, const char **envp)
{
  char dest[76];
  char *lang;

  if (argc != 3)
    return 1;

  strncpy(dest, argv[1], 40);
  strncpy(&dest[40], argv[2], 32);

  lang = getenv("LANG");
  if (lang)
  {
    if (!memcmp(lang, "fi", 2))
    {
      language = 1;
    }
    else if (!memcmp(lang, "nl", 2))
    {
      language = 2;
    }
  }

  char v4[76];
  memcpy(v4, dest, sizeof(v4));
  return greetuser(v4[0]);
}
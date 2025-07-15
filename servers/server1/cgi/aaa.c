#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char    *ft_itoa(int n)
{
    size_t    a;
    long    nb;
    char    *str;

    nb = n;
    a = (n <= 0);
    while (n != 0 && ++a > 0)
        n = n / 10;
    str = malloc(sizeof(char) * (a + 1));
    if (str == NULL)
        return (NULL);
    if (nb < 0)
    {
        str[0] = '-';
        nb *= -1;
    }
    str[a--] = '\0';
    if (nb == 0)
        str[0] = '0';
    while (nb)
    {
        str[a--] = (nb % 10) + '0';
        nb = nb / 10;
    }
    return (str);
}

int main(void)
{
    char *name = getenv("NAME");
    if (!name)
        name = "Who";
    char *said = getenv("SAY");
    if (!said)
        said = "Hi";

    size_t len = strlen("<html lang=\"en\"><head><title>WhoSayHi</title></head><body><main><h1>") + strlen(name) + strlen(" say ") + strlen(said) + strlen("!</h1></main></body></html>");
    char * content = (char*) malloc(len + 1);
    if (content == NULL)
        return(1);
    
    strcpy(content, "<html lang=\"en\"><head><title>WhoSayHi</title></head><body><main><h1>");
    strcat(content, name);
    strcat(content, " say ");
    strcat(content, said);
    strcat(content, "!</h1></main></body></html>");

    write(1, "HTTP/1.1 200 ok\r\n", strlen("HTTP/1.1 200 ok\r\n"));
    write(1, "Content-Type: text/html\r\n", strlen("Content-Type: text/html\r\n"));
    write(1, "Content-Length: ", strlen("Content-Length: "));
    char *temp = ft_itoa(len);
    if (temp == NULL)
        return 1;
    write(1, temp , strlen(temp));
    write(1, "\r\n", 2);
    write(1, "\r\n", 2);
    write(1, content, len);

    free(temp);
    free(content);

    return (0);
}
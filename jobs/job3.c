#include <unistd.h>

int main()
{
  int i, j;
  char a[100];

  for (i = 0; i < 100; i++)
  {
    for (j = 0; j < 100; j++)
      a[i] += a[j];
  }

  sleep(120);

  return 0;
}
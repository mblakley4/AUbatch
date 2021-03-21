#include <unistd.h>

int main()
{

  int i, sum;

  sum = 0;

  for (i = 0; i < 1000; i++)
  {
    sum += sum;
  }

  sleep(30);

  return 0;
}
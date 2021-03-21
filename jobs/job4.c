#include <unistd.h>

int main()
{

  int i, sum;

  sum = 0;

  for (i = 0; i < 5000; i++)
  {
    sum = sum + 2 ^ i;
  }

  sleep(45);

  return 0;
}
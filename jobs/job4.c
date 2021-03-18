int main()
{

  int i, sum;

  sum = 0;

  for (i = 0; i < 500; i++)
  {
    sum = sum + 2 ^ i;
  }

  return 0;
}
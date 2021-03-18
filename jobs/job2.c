int main()
{

  int i, sum;

  sum = 0;

  for (i = 0; i < 10000; i++)
  {
    sum += sum;
  }

  return 0;
}
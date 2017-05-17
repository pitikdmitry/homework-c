#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct bignum
{
  int* inverse_number;
  int amount;
  bool is_minus;
}bignum;

bignum expr_in_brackets(bool* mistake, bool* is_end_of_file);
bignum expr(bool* mistake, bool* is_end_of_file);
bignum multiplication(bignum* num1, bignum* num2);
bignum residual(bignum* num1, bignum* num2);
bignum summ(bignum* num1, bignum* num2);
bignum division( bignum* num1, bignum* num2);
bool smaller(bignum const *num1, bignum const *num2);
bool bigger_module(bignum const *num1, bignum const *num2);
bignum multi_with_number( bignum* num1, int x);
bignum number(bool* mistake, bool* is_end_of_file);
bignum brackets(bool* mistake, bool* is_end_of_file);
bignum factor(bool* mistake, bool* is_end_of_file);
void LevelUp(bignum* num);
void create(int number, bignum* num);
void input(bignum* num, bool* mistake, bool* is_end_of_file);

int main()
{
    bool mistake = false;
    bool end_of_file = false;
    bignum num3 = expr(&mistake, &end_of_file);
    if(mistake == true)
    {
      printf("[error]");
      if(num3.inverse_number != NULL){
        free(num3.inverse_number);
      }
      return 0;
    }

    if(num3.inverse_number != NULL){
      if(num3.is_minus == true){
        printf("-");
      }
      for(int i = num3.amount - 1; i >= 0;--i)
      {
        printf("%d",num3.inverse_number[i]);
      }
      free(num3.inverse_number);
    }
    return 0;
}

//Создание пустого объекта
void create(int number, bignum* num)
{
  num -> is_minus = false;
  num -> inverse_number = (int*)calloc(number, sizeof(int));
  num -> amount = number;
}

//Создание объекта с клавиатуры
void input(bignum* num, bool* mistake, bool* is_end_of_file)
{
  num->inverse_number = NULL;
  num -> is_minus = false;
  int current_number_of_symbols = 0;
  int current_buffer_size = 5;
  int* buffer = (int*)calloc(current_buffer_size, sizeof(int));
  if(buffer == NULL){
    *mistake = true;
  }
  num -> amount = 0;
  char c;
  bool first_iteration_for_minus = false;
  for(;;)
  {
    if(scanf("%c", &c) == EOF){
      *is_end_of_file = true;
      break;
    }
    if(current_number_of_symbols - current_buffer_size == 0){
      current_buffer_size *= 2;
      int* new_buffer = (int*)realloc(buffer, current_buffer_size * sizeof(int));
      if(new_buffer == NULL){
        *mistake = true;
      }
      buffer = new_buffer;
    }
    if(c == ' ')
    {
      ++current_number_of_symbols;
      continue;
    }
    else if(c == '-' && first_iteration_for_minus == false)
    {
      num -> is_minus = true;
    }
    else if(c >= '0' && c <= '9')
    {
      ++current_number_of_symbols;
      buffer[num -> amount] = c - '0';
      num -> amount++;
    }
    else
    {
      ungetc(c,stdin);
      break;
    }
    first_iteration_for_minus = true;
  }
  if(num->amount != 0)
  {
    num -> inverse_number = (int*)calloc(num -> amount, sizeof(int));
    if(num->inverse_number == NULL){
      *mistake = true;
    }
    int j = 0;
    int k = num -> amount - 1;
    for(;k >= 0; --k, ++j)
    {
      num -> inverse_number[j] = buffer[k];
    }
  }
  free(buffer);
  buffer = NULL;
}

//вызывает метод создания большого числа с клавиатуры
bignum number(bool* mistake, bool* is_end_of_file)
{
  bignum num1;
  input(&num1,mistake, is_end_of_file);
  return num1;
}

//возвращает либо результат операции в скобках, либо число
bignum brackets(bool* mistake, bool* is_end_of_file)
{
  char c;
  if(scanf("%c", &c) == EOF){
    *is_end_of_file = true;
  }
  if(c == '(')
  {
    bignum x = expr_in_brackets(mistake, is_end_of_file);
    int temp = scanf("%c", &c);


    if(c != ')')
    {
      *mistake = true;
    }
    else if(temp == EOF)
    {
      *is_end_of_file = true;
      *mistake = true;
      return x;
    }

    return x;
  }
  else
  {
    ungetc(c,stdin);
    return number(mistake, is_end_of_file);
  }
}

//возвращает результат умножения или деления выражений в скобках или чисел
bignum factor(bool* mistake, bool* is_end_of_file)
{
  bignum x = brackets(mistake, is_end_of_file);
  if(*is_end_of_file == true)
  {
      return x;
  }
  for(;;)
  {
    char c;
    if(scanf("%c", &c) == EOF)
    {
      *is_end_of_file = true;
      return x;
    }

    switch(c)
    {
      case ' ':
      {
        continue;
      }
      case '*':
      {
        bignum z = brackets(mistake, is_end_of_file);
        bignum result;
        result = multiplication(&x, &z);
        x = result;
        break;
      }
      case '/':
      {
        bignum z = brackets(mistake, is_end_of_file);
        bignum result;
        result = division(&x, &z);
        x = result;
        break;
      }
      default:
        ungetc(c,stdin);
        return x;
    }
  }
}

//возвращает результат вычисления всего результирующего выражения выражения
bignum expr(bool* mistake, bool* is_end_of_file)
{
  bignum x = factor(mistake, is_end_of_file);//считали число
  if(*is_end_of_file == true)
  {
      return x;
  }

  for(;;)
  {
    char c;
    if(scanf("%c", &c) == EOF)
    {
      *is_end_of_file = true;
      return x;
    }

    switch(c)
    {
      case ' ':
      {
        continue;
      }
      case '+':
      {
        //x += factor();
        bignum z = factor(mistake, is_end_of_file);

        bignum result;
        result = summ(&x, &z);
        x = result;
        break;
      }
      case '-':
      {
        bignum z = factor(mistake, is_end_of_file);

        bignum result;
        result = residual(&x, &z);
        x = result;
        // x -= factor();
        break;
      }
      default:
      if(c != '\n'){
        *mistake = true;
      }
      ungetc(c,stdin);
        return x;
    }
  }
}

//возвращает результат вычисления выражения внутри скобок
bignum expr_in_brackets(bool* mistake, bool* is_end_of_file)
{
  bignum x = factor(mistake, is_end_of_file);//считали число
  if(*is_end_of_file == true)
  {
      return x;
  }

  for(;;)
  {
    char c;
    if(scanf("%c", &c) == EOF)
    {
      *is_end_of_file = true;
      return x;
    }

    switch(c)
    {
      case ' ':
      {
        continue;
      }
      case '+':
      {
        bignum z = factor(mistake, is_end_of_file);

        bignum result;
        result = summ(&x, &z);
        x = result;
        break;
      }
      case '-':
      {
        bignum z = factor(mistake, is_end_of_file);

        bignum result;
        result = residual(&x, &z);
        x = result;
        break;
      }
      default:
      ungetc(c,stdin);
        return x;
    }
  }
}

// умножение двух больших чисел
bignum multiplication(bignum* num1, bignum* num2)
{
  bignum result;
  if(smaller(num1, num2)){
    bignum temp;
    temp = *num1;
    *num1 = *num2;
    *num2 = temp;
  }
  create(num1->amount + num2->amount, &result);// 99 * 99 = 9801 4 znaka

  if(num1->is_minus == true && num2->is_minus == false){
    result.is_minus = true;
  }
  else if(num1->is_minus == false && num2->is_minus == true){
    result.is_minus = true;
  }
  else if(num1->is_minus == true && num2->is_minus == true){
    result.is_minus = false;
  }
  else{
    result.is_minus = false;
  }

  for(int i = 0; i < num1->amount; ++i)
  {
    int r = 0;
    for(int j = 0; j < num2->amount || r; ++j)
    {
      if(j == num2->amount)
      {
            result.inverse_number[i+j] += r;
           r = 0;
           continue;
      }
      else{
      result.inverse_number[i+j] += num1->inverse_number[i] * num2->inverse_number[j] + r;
      r = result.inverse_number[i + j] / 10;
      result.inverse_number[i + j] -= r * 10;
      }
    }
  }
  int pos = num1->amount + num2->amount;
  while(pos > 0 && !result.inverse_number[pos-1])
  {
    pos--;
  }
  result.amount = pos;
  free(num1->inverse_number);
  free(num2->inverse_number);
  num1->inverse_number = 0;
  num2->inverse_number = 0;

  return result;
}

//сложение двух больших чисел
bignum summ(bignum* num1, bignum* num2)
{
  bignum result;
  if(num1->is_minus == true && num2->is_minus == false){
    num1->is_minus = false;
    num2->is_minus = false;
    return residual(num2, num1);
  }
  else if(num1->is_minus == false && num2->is_minus == true){
    num1->is_minus = false;
    num2->is_minus = false;
    return residual(num1, num2);
  }
  else if(num1->is_minus == true && num2->is_minus == true){
    if(num1->amount >= num2->amount)
    {
      create(num1->amount + 1, &result);//999+999=1998
      result.amount = num1->amount;
    }
    else
    {
      if(smaller(num1, num2)){
        bignum temp;
        temp = *num1;
        *num1 = *num2;
        *num2 = temp;
      }
      create(num1->amount + 1, &result);
      result.amount = num1->amount;
    }
    result.is_minus = true;
  }
  else{
    if(num1->amount >= num2->amount)
    {
      create(num1->amount + 1, &result);//999+999=1998
      result.amount = num1->amount;
      result.is_minus = false;
    }
    else
    {
      if(smaller(num1, num2)){
        bignum temp;
        temp = *num1;
        *num1 = *num2;
        *num2 = temp;
      }
      create(num1->amount + 1, &result);
      result.amount = num1->amount;
      // create(num2->amount + 1, &result);
      // result.amount = num2->amount;
    }
    result.is_minus = false;
  }

  int r = 0;
  for(int i = 0; i < num1->amount || r; ++i)// ошбка была в -1(не было)
  {
    if(i < num2->amount){
      result.inverse_number[i] = num1->inverse_number[i] + num2->inverse_number[i] + r;
    }
    else if(i == num1->amount){
      result.inverse_number[i] = r;
      r = 0;
      continue;
    }
    else
    {
      result.inverse_number[i] = num1->inverse_number[i] + r;
    }
    if(result.inverse_number[i] >= 10)
    {
      result.inverse_number[i] -= 10;
      r = 1;
    }
    else
    {
      r = 0;
    }
  }
  if(result.inverse_number[result.amount])//если последний пустой
  {
    ++result.amount;
  }
  free(num1->inverse_number);
  free(num2->inverse_number);
  num1->inverse_number = 0;
  num2->inverse_number = 0;

  return result;
}

//умножение большого числа на маленькое(<10) нужно для реализации деления
bignum multi_with_number(bignum* num1, int x)
{
  bignum result;
  create(num1->amount + 1, &result);
  result.amount = num1->amount;
  int r = 0;

  for(int i = 0; i < num1->amount || r; ++i)
  {
    if(i == num1->amount){
      result.inverse_number[i] = r;
      r = 0;
    }
    else{
    result.inverse_number[i] = num1->inverse_number[i] * x + r;
    r = result.inverse_number[i] / 10;
    result.inverse_number[i] -= r * 10;
    }
  }

  if(result.inverse_number[result.amount]){
    ++result.amount;
  }
  int pos = result.amount;
  while(pos > 0 && !result.inverse_number[pos-1])
  {
    pos--;
  }
  result.amount = pos;
  return result;
}

//Возвращает true если |num1|<=|num2|
bool smaller(bignum const *num1, bignum const *num2)
{
  if (num1->amount != num2->amount){
    return num1->amount < num2->amount;
  }
  else{
    for (int i = num1->amount - 1; i >= 0; i--)
    {
      if (num1->inverse_number[i] != num2->inverse_number[i])
        return num1->inverse_number[i] <  num2->inverse_number[i];
    }
    for (int i = num1->amount - 1; i >= 0; i--)
    {
      if (num1->inverse_number[i] != num2->inverse_number[i])
        return false;
    }
    return true;
  }
}

//Возвращает true если |num1|>=|num2|
bool bigger_module(bignum const *num1, bignum const *num2)
{
    if (num1->amount!=num2->amount){
      return num1->amount > num2->amount;
    }
    for (int i = num1->amount - 1; i >= 0; i--)
    {
      if (num1->inverse_number[i] != num2->inverse_number[i]){
        return num1->inverse_number[i] > num2->inverse_number[i];
      }
    }
    return true;
}

//сдвигает в массиве временныъ остатков от деления на один порядок
void LevelUp(bignum* num)
{
  for (int i = num -> amount;i>=1;i--)
    num -> inverse_number[i] = num -> inverse_number[i-1];
  if (num -> inverse_number[num->amount])
    num -> amount++;
}

//деление
bignum residual(bignum* num1, bignum* num2)
{
  bignum result;
  result = *num1;
  if( bigger_module(num1, num2) )//num1 >= num2
  {
    if(num1->is_minus == false && num2->is_minus == false){
      result.is_minus = false;
    }
    else if(num1->is_minus == false && num2->is_minus == true){
      bignum res = summ(num1, num2);
      res.is_minus = false;
      return res;
    }
    else if(num1->is_minus == true && num2->is_minus == false){
      bignum res = summ(num1, num2);
      res.is_minus = true;
      return res;
    }
    else if(num1->is_minus == true && num2->is_minus == true){
      result.is_minus = true;
    }
  }
  else//num1 < num2
  {
    if(num1->is_minus == false && num2->is_minus == false){
       bignum temp;
       temp = *num1;
       *num1 = *num2;
       *num2 = temp;
       result = *num1;
       result.is_minus = true;
    }
    else if(num1->is_minus == false && num2->is_minus == true){
      bignum res = summ(num1, num2);
      res.is_minus = false;
      return res;
    }
    else if(num1->is_minus == true && num2->is_minus == false){
      bignum res = summ(num1,num2);
      res.is_minus = true;
      return res;
    }
    else if(num1->is_minus == true && num2->is_minus == true){
      bignum temp = *num1;
      *num1 = *num2;
      *num2 = temp;
      result = *num1;
      result.is_minus = false;
    }
  }
  for(int i = 0; i < result.amount ; ++i)
  {
    if(i < num2->amount){
      result.inverse_number[i] -= num2->inverse_number[i];
    }
    if(result.inverse_number[i] < 0)
    {
      result.inverse_number[i] += 10;
      result.inverse_number[i + 1] --;
    }
  }
  int pos = result.amount - 1;
  while(pos > 0 && !result.inverse_number[pos])
  {
    pos--;
  }
  result.amount = pos + 1;
  free(num2->inverse_number);
  return result;
}

//вычитание
bignum division(bignum* num1, bignum* num2)
{
  bignum res;
  if(smaller(num1,num2)){
    create(1,&res);
    free(num1->inverse_number);
    free(num2->inverse_number);
    return res;
  }
  create(num1->amount, &res);
  if(num1->is_minus == true && num2->is_minus == false){
    res.is_minus = true;
  }
  else if(num1->is_minus == false && num2->is_minus == true){
    res.is_minus = true;
  }
  else if(num1->is_minus == true && num2->is_minus == true){
    res.is_minus = false;
  }
  else{
    res.is_minus = false;
  }
  bignum curValue;
  create(num1->amount, &curValue);
  curValue.amount = 1;
  for (int i = num1->amount - 1; i >= 0; i--)
  {
    LevelUp(&curValue); //сдвигаем предыдущий остаток на 1
    curValue.inverse_number[0] = num1->inverse_number[i];//берем первое число

    // подбираем максимальное число x, такое что num2 * x <= curValue
    int x = 0;
    int osn = 10;
    int l = 0, r = osn;
    while (l <= r)
    {
      int m = (l + r) >> 1;

      bignum cur = multi_with_number(num2, m);// bignum cur = num2 * m;
      //пытаемся подобрать num2*m под то что сейчас находится в curValue

      if(smaller(&cur,&curValue) == true)//(cur <= curValue)
      {
        x = m;
        l = m+1;
      }
      else{
        r = m-1;
      }

      free(cur.inverse_number);
    }
    res.inverse_number[i] = x;
    // curValue = curValue - num2 * x;
    bignum temp = multi_with_number(num2, x);
    bignum foo;
    foo = residual(&curValue, &temp);
    curValue = foo;
  }

  int pos = num1->amount - 1;
  while (pos>=0 && !res.inverse_number[pos])
    pos--;
  res.amount = pos+1;

  free(curValue.inverse_number);
  free(num1->inverse_number);
  free(num2->inverse_number);
  return res;
}

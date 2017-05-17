#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define max_bit 65
#define decimal 10
#define min_base 2
#define max_base 36

char int_to_char(int foo)
{
    if(foo >= 0 && foo <= 9){
        return foo + '0';
    }

    if(foo >= 10 && foo <= 36){
        return foo + 'A' + decimal;
    }

    return 0;
}

char* dec_to_Q(int q, size_t result)
{
    char* buffer = (char*)malloc(max_bit * sizeof(char));
    if(buffer == NULL){
        return NULL;
    }

    int counter = 0;

    if(result == 0){
        buffer[counter] = '0';
        counter++;
    }

    while(result != 0){
        int foo = result % q;

        buffer[counter] = int_to_char(foo);
        if(buffer[counter] == 0){
             return NULL;
        }

        result /= q;
        counter++;
    }

    buffer[counter] = '\0';

    int length = strlen(buffer);

    char* inverse_buffer = (char*)malloc((length + 1) * sizeof(char));
    if(inverse_buffer == NULL){
        return NULL;
    }

    int i = 0;
    int j = length - 1;
    for(; j >= 0; i++, j--){
        inverse_buffer[i]=buffer[j];
    }
    inverse_buffer[length]='\0';

    free(buffer);
    return inverse_buffer;
}

int char_to_int(int p, char temp)
{
    if(p <= decimal){
        if(temp - '0' >= p){
            return -1;
        }
    }
    else if(p > 10){
        if(temp - 'a' + 10 >= p){
          return -1;
      }
    }

    if(temp >= 'A' && temp <= 'Z'){
        return temp - 'A' + decimal;
    }

    if(temp >= 'a' && temp <= 'z'){
      return temp - 'a' + decimal;
    }

    if(temp >= '0' && temp <= '9'){
        return temp - '0';
    }

    return -1;
}

size_t P_to_dec(int p, char* S)
{
    int mod = 0;
    size_t result_dec = 0;
    int counter = strlen(S) - 1;

    while(counter >= 0){
        char temp = S[counter];

        int number = char_to_int(p,temp);
        if(number == -1){
            return -1;
        }

        result_dec += number * pow(p,mod);
        mod++;
        counter--;
    }

    return result_dec;
}

char* P_to_Q(int* P,int* Q,char* S)
{
    int p = *P;
    int q = *Q;

    if(!(q >= 2 && q < p && p <= 36) || *S == '\0'){
        return NULL;
    }

    size_t result_dec = P_to_dec(p ,S);
    if(result_dec == -1){
        return NULL;
    }

    char* answer = dec_to_Q(q, result_dec);
    if(answer == NULL){
        return NULL;
    }

    return answer;
}

int main()
{
    int* P = (int*)malloc(sizeof(int));
    int* Q = (int*)malloc(sizeof(int));
    char* S = (char*)malloc(max_bit * sizeof(char));

    if(P == NULL || Q == NULL || S == NULL){
        printf("[error]");
        free(S);
        free(P);
        free(Q);
        return 0;
    }

    if(scanf("%d%d%s", P, Q, S) != 3){
        printf("[error]");
        free(S);
        free(P);
        free(Q);
        return 0;
    }


    char* answer = P_to_Q(P, Q, S);
    if(answer == NULL){
        printf("[error]");
        free(S);
        free(P);
        free(Q);
        return 0;
    }

    printf("%s",answer);

    free(answer);
    free(S);
    free(P);
    free(Q);

    return 0;
}

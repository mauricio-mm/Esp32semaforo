#include "semaphoro.h"

void semaphoro_on(SemaphoroPins *semaphoro, int num, int color)
{
    digitalWrite(semaphoro->in[num],    LOW);
    digitalWrite(semaphoro->out[color], HIGH);
}

//coloca IN como HIGH ficar no mesmo nivel de tensao que o OUT
//coloca OUT como LOW para nao ligar o led forçando led travar corrente oposta
void semaphoro_off(SemaphoroPins *semaphoro)
{
    for(int i = 0; i < SEMAPHORE_IN_COUNT; i++)
        digitalWrite(semaphoro->in[i], HIGH);

    for(int i = 0; i < SEMAPHORE_OUT_COUNT; i++)
        digitalWrite(semaphoro->out[i], LOW);
        
}

void semaphoro_cicle(SemaphoroPins *semaphoro)
{
    for (int i = 0;i < SEMAPHORE_IN_COUNT; i++)
    {
        for(int k = 0;k < SEMAPHORE_OUT_COUNT; k++)
        { 
            semaphoro_off(semaphoro);
            delay(10 - DUTY_CICLE);
            semaphoro_on(semaphoro, i, k);
            delay(DUTY_CICLE);
        }
    }
}
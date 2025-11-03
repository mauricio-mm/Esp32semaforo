#pragma once

#ifndef SEMAPHORO_H
#define SEMAPHORO_H

#include <Arduino.h>

#define DUTY_CICLE 5 // ON  = DUTY_CICLE && 
                     // OFF = (10 - DUTY_CICLE)

#define SEMAPHORE_STATE     12 
#define SEMAPHORE_IN_COUNT  5
#define SEMAPHORE_OUT_COUNT 3
#define SEMAPHORE_BUTTON    23   

struct SemaphoroPins {
    int in[SEMAPHORE_IN_COUNT];
    int out[SEMAPHORE_OUT_COUNT];
};

const int timer_state[SEMAPHORE_STATE][SEMAPHORE_IN_COUNT][SEMAPHORE_OUT_COUNT] = 
{
    { {0,0,1}, {0,0,1}, {1,0,0}, {1,0,0}, {1,0,0}}, //1° Tempo 
    { {0,0,1}, {0,0,1}, {0,1,0}, {0,1,0}, {0,1,0}}, //Reduz velovidade
    { {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}}, //Fechar sinaleira
    { {1,0,0}, {0,0,1}, {0,0,1}, {1,0,0}, {0,0,1}}, //2° Tempo 
    { {0,1,0}, {0,0,1}, {0,0,1}, {0,1,0}, {0,0,1}}, //Reduz velovidade
    { {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}}, //Fechar sinaleira
    { {1,0,0}, {1,0,0}, {1,0,0}, {0,0,1}, {0,0,1}}, //3° Tempo 
    { {0,1,0}, {0,1,0}, {0,1,0}, {0,0,1}, {0,0,1}}, //Reduz velovidade
    { {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}}, //Fechar sinaleira
    { {1,0,0}, {0,0,1}, {1,0,0}, {1,0,0}, {1,0,0}}, //4° Tempo 
    { {0,1,0}, {0,0,1}, {0,1,0}, {0,1,0}, {0,1,0}}, //Reduz velovidade
    { {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}}  //Fechar sinaleira
};

// Funções de controle
void semaphoro_on   (SemaphoroPins *semaphoro, int num, int color);
void semaphoro_off  (SemaphoroPins *semaphoro);
void semaphoro_cicle(SemaphoroPins *semaphoro);

#endif

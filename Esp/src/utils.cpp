#include "utils.h"

//  OUT --|>|-- IN

void config(SemaphoroPins *semaphoro)
{
  int out[3] = {21, 22, 23};
  int in [5] = {4, 16, 17, 5, 18};

  Serial.begin(9600);

  for (int i = 0; i < SEMAPHORE_OUT_COUNT; i++)
  {
    pinMode(out[i], OUTPUT);
    semaphoro->out[i] = out[i];
    digitalWrite(semaphoro->out[i], LOW);
  }

  for (int i = 0; i < SEMAPHORE_IN_COUNT; i++)
  {
    pinMode(in[i], OUTPUT);
    semaphoro->in[i] = in[i];
    digitalWrite(semaphoro->in[i], LOW);
  }

  pinMode(SEMAPHORE_BUTTON, INPUT);
}

int verify_state(const int *arr) 
{
    return arr[0]*0 + arr[1]*1 + arr[2]*2;
}

String timerStateToString(int state)
{
    String result = "{ "; 

    for (int i = 0; i < SEMAPHORE_IN_COUNT; i++) {
        result += "{";
        for (int j = 0; j < SEMAPHORE_OUT_COUNT; j++) {
            result += String(timer_state[state][i][j]);
            if (j < SEMAPHORE_OUT_COUNT - 1)
                result += ",";
        }
        result += "}";
        if (i < SEMAPHORE_IN_COUNT - 1)
            result += ",";
        result += " ";
    }

    result += "}";

    return result;
}

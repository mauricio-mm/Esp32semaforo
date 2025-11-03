#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "semaphoro.h"
#include "FS.h"
#include "SPIFFS.h"

void config(SemaphoroPins *semaphoro);
int  verify_state(const int *arr);

#endif

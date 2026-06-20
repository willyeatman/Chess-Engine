#pragma once

#include <iostream>
#include <inttypes.h>
#include <array>
#include <csignal>
#include <unistd.h>

void signalHandler(int sig);

void startScreen();

void endScreen();

void clearScreen();
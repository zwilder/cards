/*
* Cards
* Copyright (C) Zach Wilder 2022
* 
* This file is a part of Cards
*
* Cards is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Cards is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Cards.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CARDS_H
#define CARDS_H

/********
 * System
 ********/
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h> 
#include <stdarg.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

/*********
 * Project
 *********/
#include <draw.h>
#include <flags.h>
#include <input.h>
#include <mt19937.h>
#include <deck.h>
#include <button.h>

/*******
 * Games
 *******/
#include <cribbage.h>
#include <cribbage_events.h>
#include <cribbage_update.h>
#include <cribbage_draw.h>
#include <klondike.h>

#endif

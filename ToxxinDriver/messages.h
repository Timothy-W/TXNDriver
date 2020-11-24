#pragma once

#ifndef MESSAGES_H
#define MESSAGES_H
#define STR1 "[TXND] "

#define DEBUG_MESSAGE(x, ...) DbgPrintEx(0,0, STR1 x, __VA_ARGS__)

#endif


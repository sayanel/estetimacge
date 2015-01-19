#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "notifications.cpp"






void error_func (GPContext *, const char *, va_list , void *);

void message_func (GPContext *, const char *, va_list, void *);
#include "main.h"

static enum 
{
	SM_WAIT = 0,
	SM_FIRST, 
	SM_NULL,
	SM_LAST,
} state;

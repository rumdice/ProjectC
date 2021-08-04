#include "Util.h"

void	WriteConsoleLog(char *lpszFmt, ...)
{
	char szOutText[256];
	va_list Vargs;

	va_start(Vargs, lpszFmt);
	vsprintf(szOutText, lpszFmt, Vargs);
	va_end(Vargs);
	strcat(szOutText, "\n");

	printf(szOutText);

}
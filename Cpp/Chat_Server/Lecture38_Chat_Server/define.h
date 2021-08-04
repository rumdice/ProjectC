#pragma once

#define delay(n) Sleep(n)								
#define randomize() srand((unsigned)time(NULL))			
#define random(n) (rand() % (n))						
#define random1(n) (rand() % (n) + 1)					

#define MAX 1024
#define dfNETWORK_MAX_USER 63							
#define DEFAULT_RING_BUF_SIZE 1000						
#define DEFAULT_RING_BUF_SPACE_SIZE 1				



#include "ludoMacro.h"
unsigned char colm,Row;
void FindLocation(unsigned char position)
{
	if(position < 6 )
	{colm = 1;Row = position;}
	
	else if((position > 5)&&(position <12))
	{colm = 12;Row = (11-position);}
	
	else if(position == 12)
	{colm = 11;Row = 0;}
	
	else if((position > 12)&&(position < 19))
	{colm = 10;Row = (5-(18-position));}
	
	else if((position > 18)&&(position < 25))
	{colm = 9;Row = 24-position;}
	
	else if((position == 25))
	{colm = 8;Row = 0;}
	
	else if((position > 25)&&(position < 32))
	{colm = 7;Row = (5-(31-position));}
	
	else if((position > 31)&&(position < 38))
	{colm = 6;Row = 37-position;}
	
	else if((position == 38))
	{colm = 5;Row = 0;}
	
	else if((position > 38)&&(position <45))
	{colm = 4;Row = (5-(44-position));}
	
	else if((position > 44)&&(position < 51))
	{colm = 3;Row = 50-position;}
	
	else if((position == 51))
	{colm = 2;Row = 0;}

}

//void findRow()


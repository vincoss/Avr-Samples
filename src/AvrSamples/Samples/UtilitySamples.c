/*
 * UtilitySamples.c
 *
 * Created: 23/02/2018 7:53:10 AM
 *  Author: Ferdinand Lukasak
 */ 

void UtilitySamples_IntToStringNew()
{
	char buffer[50];
	
	char * result = IntToStringNew(0, "", buffer, sizeof(buffer));
	
	UsartWriteCharString(result);
	
}
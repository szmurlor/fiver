#include <stdio.h>
#include <field.h>
#include <stdlib.h>


int main( int argc, char ** argv)
{
	Field f;

	f.attribs["NAME"] = "Pole skalarne";

	printf("Wartość zapisana w obiekcie pola: %s\n", f.attribs["NAME"]);
	
	return 0;
}


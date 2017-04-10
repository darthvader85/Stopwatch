/*
 * Utils.c
 *
 * Created: 18.10.2012 21:36:55
 *  Author: Stefan
 */ 

void int2str(char str[], int i, unsigned char length)
{
	unsigned char neg=0;
if(i<0){neg=1;i^=-1;i++;}
signed char n=length;
str[n--]=0;
if (i==0)str[n--]='0';
	while(i&&n>=0){str[n--]=i%10+'0';i/=10;}
	if(neg&&n>=0)str[n--]='-';
	while(n>=0)str[n--]=' ';		
}
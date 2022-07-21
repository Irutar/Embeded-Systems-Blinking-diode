#define PIO_PERB (volatile unsigned int*) 0xFFFFF400
#define PIO_PERC (volatile unsigned int*) 0xFFFFF600
#define PIO_OERB (volatile unsigned int*) 0xFFFFF410
#define PIO_OERC (volatile unsigned int*) 0xFFFFF610
#define PIO_CODRB (volatile unsigned int*) 0xFFFFF434
#define PIO_CODRC (volatile unsigned int*) 0xFFFFF634
#define PIO_SODRB (volatile unsigned int*) 0xFFFFF430
#define PIO_SODRC (volatile unsigned int*) 0xFFFFF630
#define PIO_ODSRC (volatile unsigned int*) 0xFFFFF638
#define PIO_ODRC (volatile unsigned int*) 0xFFFFF614
#define PIO_PDSR (volatile unsigned int*) 0xFFFFF63C
#define PCM_PCER (volatile unsigned int*) 0xFFFFFC10
#define PIO_PUER (volatile unsigned int*) 0xFFFFF664

#define USERLED1 (volatile unsigned int) 1UL<<8
#define USERLED2 (volatile unsigned int) 1UL<<29
#define USERCLIC1 (volatile unsigned int) 1UL<<4
#define USERCLIC2 (volatile unsigned int) 1UL<<5
#define PCM_CDE (volatile unsigned int) 1UL<<4

#define HALFHz 1000000
#define MS HALFHz / 500

void dbgu_print_ascii(const char* a){}

void initLED()
{
  *(PIO_PERB) = USERLED1;
  *(PIO_OERB) = USERLED1;
  *(PIO_SODRB) = USERLED1;
  
  *(PIO_PERC) = USERLED2;
  *(PIO_OERC) = USERLED2;
  *(PIO_SODRC) = USERLED2;
}

void delay_ms(unsigned int delay_ms)
{
  volatile unsigned int i = 0;
  for (; i< delay_ms * MS; ++i)
    ;
}

void initBUTTONS()
{
  *(PIO_PERC) = USERCLIC1 | USERCLIC2;
  *(PIO_ODRC) = USERCLIC1 | USERCLIC2;
  *(PCM_PCER) = PCM_CDE;
  *(PIO_PUER) = USERCLIC1 | USERCLIC2;
}

int main()
{
  initLED();
  initBUTTONS();  
  
  unsigned int delay = 0;
  unsigned char led_enabled = 0;
  for (; 1; ++delay) {
    if (((*PIO_PDSR) & USERCLIC1) == 0UL)
      *(PIO_SODRC) = USERLED2;
    else if (((*PIO_PDSR) & USERCLIC2) == 0UL)
      *(PIO_CODRC) = USERLED2;
    if (delay > HALFHz) {
      delay = 0;
      if (led_enabled == 0) {
	led_enabled = 1;
	*(PIO_CODRB) = USERLED1;
      } else {
	led_enabled = 0;
	*(PIO_SODRB) = USERLED1;
      }
    }
  }
}

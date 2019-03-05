#include <16f84a.h>	// Contiene todas las definiciones necesarias para este dispositivo

#use delay (clock = 1MHz)

#use fast_io(a)
#use fast_io(b)

int divisor = 0;
int contador = 0;

#INT_TIMER0
void int_tmr0(){
	set_timer0(12);

	divisor++;

	if(divisor == 4){
		contador++;

		output_b(contador);

		divisor = 0;
	}
}

void main(){
	set_tris_a(0xFF);	// Configurar pines
	set_tris_b(0xF0);

	output_b(contador);

	setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256);
	set_timer0(12);

	enable_interrupts(INT_TIMER0);
	enable_interrupts(GLOBAL);

	while(TRUE){
		if(!input(PIN_A0)){
			disable_interrupts(INT_TIMER0);
			contador = 0;

			output_b(contador);

			while(!input(PIN_A0)){}

			divisor = 0;
			set_timer0(12);
			enable_interrupts(INT_TIMER0);
		}
	}
}


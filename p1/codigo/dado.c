#include <16f84a.h>	// Contiene todas las definiciones necesarias para este dispositivo

#use fast_io(a)
#use fast_io(b)

void main(){
	int contador = 0;

	setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2);	// Inicializar temporizador para conseguir n�meros "aleatorios"

	set_tris_a(0xFF);	// Configurar pines
	set_tris_b(0xF0);

	output_b(contador);

	while(TRUE){
		while(input(PIN_A0) && input(PIN_A1)){}

		contador = (get_timer0() % 6) + 1;

		output_b(contador);

		while(!(input(PIN_A0) && input(PIN_A1))){}
	}
}


#include "tarjeta.h"
#include "movimiento.h"
#use delay(clock=48000000)
#include "srf02.h"

#define ADE 0
#define ATR 1
#define IZQ 2
#define DER 3
#define PAR 4

#define BUSQUEDA 0
#define ESQUINA 1
#define DELANTE_NEGRO 2
#define ATRAS_NEGRO 3
#define ESQUINADO 4
#define ACECHAR 5
#define ATACAR 6
#define EMPUJAR_ADELANTE 7
#define EMPUJAR_ATRAS 8
#define ATACAR_ATRAS 9

int aux = 0, estado, tipo, vel, duracion;

// interrumpe cada 1s
/*
#INT_TIMER0
void int_tmr0(){
	set_timer0(18661);
}*/

#INT_TIMER2
void int_tmr2(){
	if(aux == 0){
		if(duracion < 255){
			duracion--;
		}

		if(duracion == 0){

		}
	}

	if(aux++ <= vel){
		switch(tipo){
			case ADE:
				adelante();
				break;
			case ATR:
				atras();
				break;
			case IZQ:
				izquierda();
				break;
			case DER:
				derecha();
				break;
			case PAR:
				para();
				break;
		}
	}
	else{
		para();
	}
}

void main() {
	delay_ms(2100);
	unsigned int16 m;
	setup_timer_2(T2_DIV_BY_16, 0, 16);

	enable_interrupts(INT_TIMER2);
	enable_interrupts(GLOBAL);

	// Busqueda
	estado=BUSQUEDA;
	m=255;
	int urgente=0, urgActual=0;

	while(1){
		if(urgente>urgActual || duracion==0 || duracion==255) {
			urgActual=urgente;
			switch (estado) {
				case BUSQUEDA: // busqueda
					m = srf_measure_cm();
					tipo = IZQ;
					vel = 128;
					duracion = 255;

					if (m<50) {
						estado = ACECHAR;
						urgente = 0;
					}

					if (!IN1 && !IN2){
						estado=ESQUINA; // esquina
						urgente = 1;
					}
					else {
						if (!IN1) {
							estado=ATRAS_NEGRO; // atras negro
							urgente=1;
						}
						else if (!IN2) {
							estado=DELANTE_NEGRO; // delante negro
							urgente=1;
						}
					}

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}
					break;
				case ACECHAR:
					m = srf_measure_cm();
					tipo=ADE;
					vel=150;
					duracion=255;

					if (m>=50){
						estado = BUSQUEDA;
						urgente = 0;
					}
					else if(m<35) {
						estado = ATACAR;
						urgente = 0;
					}

					if (!IN1 && !IN2){
						estado=ESQUINA; // esquina
						urgente = 1;
					}
					else {
						if (!IN1) {
							estado=ATRAS_NEGRO; // atras negro
							urgente=1;
						}
						else if (!IN2) {
							estado=DELANTE_NEGRO; // delante negro
							urgente=1;
						}
					}

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}
					break;
				case ATACAR:
					tipo=ADE;
					vel=255;
					duracion=255;
					led_on();

					if (!IN1 && !IN2){
						led_off();
						estado=ESQUINA; // esquina
						urgente = 1;
					}
					else {
						if (!IN1) {
							led_off();
							estado=ATRAS_NEGRO; // atras negro
							urgente=1;
						}
						else if (!IN2) {
							led_off();
							estado=DELANTE_NEGRO; // delante negro
							urgente=1;
						}
					}

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}

					break;
				case EMPUJAR_ADELANTE:
					tipo=ADE;
					vel=255;
					duracion=255;
					led_on();

					if (!IN3) {
						led_off();
						estado = ATACAR;
						urgente = 0;
					}
					break;
				case EMPUJAR_ATRAS:
					tipo=ATR;
					vel=255;
					duracion=255;
					led_on();

					if (!IN4) {
						led_off();
						estado = ATACAR_ATRAS;
						urgente = 0;
					}
					break;
				case ESQUINA: // esquina
					tipo = DER;
					vel = 200;
					duracion = 130;
					estado = BUSQUEDA;
					urgente=0;

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}

					break;
				case DELANTE_NEGRO: // delante negro
					tipo = ATR;
					vel = 255;
					duracion = 100;
					if (!IN1) { // sensor atras
						estado=ESQUINADO;
						urgente=2;
					}
					else{
						estado=BUSQUEDA;
						urgente=0;
					}

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}

					break;
				case ATRAS_NEGRO: // atr�s negro
					tipo = ADE;
					vel = 255;
					duracion = 100;
					if (!IN2) { // sensor delante
						estado=ESQUINADO;
						urgente=2;
					}
					else{
						estado=BUSQUEDA;
						urgente=0;
					}

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}

					break;
				case ESQUINADO:
					tipo = DER;
					vel = 200;
					duracion = 130;
					estado = BUSQUEDA;
					urgente=0;

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}

					break;
				case ATACAR_ATRAS:
					tipo = ATR;
					vel=255;
					duracion=255;
					led_on();

					if (!IN1 && !IN2){
						led_off();
						estado=ESQUINA; // esquina
						urgente = 1;
					}
					else {
						if (!IN1) {
							led_off();
							estado=ATRAS_NEGRO; // atras negro
							urgente=1;
						}
						else if (!IN2) {
							led_off();
							estado=DELANTE_NEGRO; // delante negro
							urgente=1;
						}
					}

					if (IN3) {
						estado = EMPUJAR_ADELANTE;
						urgente = 3;
					}
					else if (IN4) {
						estado = EMPUJAR_ATRAS;
						urgente = 3;
					}

					break;
			}
		}
	}
}

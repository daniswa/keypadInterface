





#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void orgData();
void sendDispData();

const int HIGH = 1;
const int LOW = 0;
int inputSignal;
int segmentArrayIn[33];
int segmentArrayOut[8][4];
int count = 0;
int input;

int main(void){
	DDRA = 0x00;
	DDRB = 0x05;
	DDRC = 0x01;
	DDRD = 0x03;
	sei();
	EICRA = 0x02;
	EIMSK = 0x01;
	
	while(1){
		PORTC = PORTC | 0x01;
		_delay_ms(1000);
		PORTC = PORTC | 0x00;
		_delay_ms(1000);
	}
}

ISR(INT0_vect){
	input = PIND & 0x08;
	int data;

	if(count <= 32){

		if(input == 0x08){
			data = 0x02;
		}else{
			data = 0x00;
		}
		segmentArrayIn[count] = data;
	}

	if(count == 33){
		orgData();
		sendDispData();
	}
	count++;

	if(count == 35){
		count = 0;
	}
}

void orgData(){
	int i;
	int arrayCounter = 1;

	for(i = 4;i > 0;i--){
        segmentArrayOut[4][i] = segmentArrayIn[arrayCounter];
	    arrayCounter++;
	    segmentArrayOut[6][i] = segmentArrayIn[arrayCounter];
	    arrayCounter++;
	    segmentArrayOut[2][i] = segmentArrayIn[arrayCounter];
	    arrayCounter++;
	    segmentArrayOut[7][i] = segmentArrayIn[arrayCounter];
	    arrayCounter++;
	    segmentArrayOut[0][i] = segmentArrayIn[arrayCounter];
	    arrayCounter++;
	    segmentArrayOut[5][i] = segmentArrayIn[arrayCounter];
	    arrayCounter++;
	    segmentArrayOut[1][i] = segmentArrayIn[arrayCounter];
	    arrayCounter++;
		segmentArrayOut[3][i] = segmentArrayIn[arrayCounter];
        arrayCounter++;
	}
}

void sendDispData(){
	int j = 0;
	int k = 0;
	int l = 0;
	PORTD = 0x03;
	_delay_us(1);
	PORTD = (0x00 | 0x02);
	PORTD = (0x01 | 0x02);

	for(j = 0;j < 4;j++){

		for(k = 0;k < 8;k++){
			PORTD = segmentArrayOut[k][j];
			PORTD = (0x00 | segmentArrayOut[k][j]);
			PORTD = (0x01 | segmentArrayOut[k][j]);
		}
	}

	for(l = 0;l < 3;l++){
		PORTD = 0x02;
		PORTD = 0x03;
	}
}
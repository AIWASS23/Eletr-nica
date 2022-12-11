/* Marcelo De Araújo */

#include <p18f4520.h>

void delay_ms(unsigned long int);
void animar(int);

// array com cada uma das letras (L A I S) que será mostrado no LED
char letras [26][8] =
{
{
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
},
{
0b00000000,
0b00000001,
0b00000001,
0b00000001,
0b00000001,
0b00000001,
0b00000001,
0b00000000
},
{
0b00000000,
0b00000010,
0b00000010,
0b00000010,
0b00000010,
0b00000010,
0b00000011,
0b00000000
},
{
0b00000000,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b00000111,
0b00000000
},
{
0b00000000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001111,
0b00000000
},
{
0b00000000,
0b00010000,
0b00010000,
0b00010000,
0b00010000,
0b00010000,
0b00011110,
0b00000000
},
{
0b00000000,
0b00100001,
0b00100001,
0b00100001,
0b00100001,
0b00100001,
0b00111101,
0b00000000
},
{
0b00000000,
0b01000011,
0b01000010,
0b01000010,
0b01000011,
0b01000010,
0b01111010,
0b00000000
},
{
0b00000000,
0b10000111,
0b10000100,
0b10000100,
0b10000111,
0b10000100,
0b11110100,
0b00000000
},
{
0b00000000,
0b00001111,
0b00001001,
0b00001001,
0b00001111,
0b00001001,
0b11101001,
0b00000000
},
{
0b00000000,
0b00011110,
0b00010010,
0b00010010,
0b00011110,
0b00010010,
0b11010010,
0b00000000
},
{
0b00000000,
0b00111101,
0b00100101,
0b00100101,
0b00111101,
0b00100101,
0b10100101,
0b00000000
},
{
0b00000000,
0b01111010,
0b01001010,
0b01001010,
0b01111010,
0b01001010,
0b01001010,
0b00000000
},
{
0b00000000,
0b11110101,
0b10010101,
0b10010101,
0b11110100,
0b10010100,
0b10010101,
0b00000000
},
{
0b00000000,
0b11101011,
0b00101010,
0b00101011,
0b11101000,
0b00101000,
0b00101011,
0b00000000
},
{
0b00000000,
0b11010111,
0b01010100,
0b01010111,
0b11010000,
0b01010000,
0b01010111,
0b00000000
},
{
0b00000000,
0b10101111,
0b10101000,
0b10101111,
0b10100001,
0b10100001,
0b10101111,
0b00000000
},
{
0b00000000,
0b01011110,
0b01010000,
0b01011110,
0b01000010,
0b01000010,
0b01011110,
0b00000000
},
{
0b00000000,
0b10111100,
0b10100000,
0b10111100,
0b10000100,
0b10000100,
0b10111100,
0b00000000
},
{
0b00000000,
0b01111000,
0b01000000,
0b01111000,
0b00001000,
0b00001000,
0b01111000,
0b00000000
},
{
0b00000000,
0b11110000,
0b10000000,
0b11110000,
0b00010000,
0b00010000,
0b11110000,
0b00000000
},
{
0b00000000,
0b11100000,
0b00000000,
0b11100000,
0b00100000,
0b00100000,
0b11100000,
0b00000000
},
{
0b00000000,
0b11000000,
0b00000000,
0b11000000,
0b01000000,
0b01000000,
0b11000000,
0b00000000
},
{
0b00000000,
0b10000000,
0b00000000,
0b10000000,
0b10000000,
0b10000000,
0b10000000,
0b00000000
},
{
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
}
};

// array que habilitará cada uma das linhas do LED
char habilitar[8] =
{
0b01111111,
0b10111111,
0b11011111,
0b11101111,
0b11110111,
0b11111011,
0b11111101,
0b11111110
};

main()
{
	TRISB = 0; // Configura a porta B como saida
	TRISC = 0; // Configura a porta C como saida
	TRISD = 0; // Configura a porta D como saida

	while(1)
	{
		// Serão os 26 frames mostrados no LED da direita + 8 frames de delay para que o LED da 
		//esquerda começe a mostrar os 26 frames também, totalizando 34 frames
		animar(34); 
	}
}

// Função que demora uma quantidade de milisegundos dada como entrada
void delay_ms(unsigned long int ms)
{
	unsigned long int i;
	for(ms; ms >0; ms--)
	{
		for(i=0; i<79; i++) {};
	}
}

// Função que realizará a animação nos displays
void animar(int f)
{
int i, q, v;  //f-frames, q-quadros, v-velocidade

for(i = 0; i<f; i++) // Cada um dos frames representa uma imagem da array desenho
{
	for(v = 0; v<3; v++) // a velocidade é dada para que cada uma das imagens seja feita 3 vezes, deixando-a mais visível
	{
		for(q = 0; q<8; q++) // cada um dos quadros = cada uma das linhas do LED onde será mostrado o desenho
		{
			LATC = 0b11111111; //Mantém os LEDS apagados
			delay_ms(1);
			if(i+8  < 34) //Se essa condição é satisfeita, há coisas para serem mostradas no LED da direita
			{
			LATD = letras[i][q];
			}
			if(i > 7) //Se essa condição é satisfeita, há coisas para serem mostradas no LED da esquerda
			{
			LATB = letras[i-8][q];
			}
			LATC = habilitar[q]; // Habilita os LEDS e mostra o respectivo quadro
			delay_ms(2);
		}
	}
}
}

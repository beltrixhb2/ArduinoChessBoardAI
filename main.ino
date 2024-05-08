#include <LiquidCrystal.h>

#include <Adafruit_NeoPixel.h>		// importa libreria

Adafruit_NeoPixel strips = Adafruit_NeoPixel(256, 0, NEO_GRB + NEO_KHZ800);	// creacion de objeto "tira"
// con una cantidad de 8 pixeles, sobre pin 2 de Arduino y configurado para WS2812


#define W while
#define M 0x88
#define S 128
#define I 8000

#define MYRAND_MAX 65535     /* 16bit pseudo random generator */
long  N, T;                  /* N=evaluated positions+S, T=recursion limit */
short Q, O, K, R, k=16;      /* k=moving side */
char *p, c[5], Z;            /* p=pointer to c, c=user input, computer output, Z=recursion counter */

char L,
w[]={0,2,2,7,-1,8,12,23},                             /* relative piece values    */
o[]={-16,-15,-17,0,1,16,0,1,16,15,17,0,14,18,31,33,0, /* step-vector lists */
     7,-1,11,6,8,3,6,                                 /* 1st dir. in o[] per piece*/
     6,3,5,7,4,5,3,6};                                /* initial piece setup      */
/* board is left part, center-pts table is right part, and dummy */  

char b[]={     
  22, 19, 21, 23, 20, 21, 19, 22, 28, 21, 16, 13, 12, 13, 16, 21,
  18, 18, 18, 18, 18, 18, 18, 18, 22, 15, 10,  7,  6,  7, 10, 15,
   0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
   0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
   0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
   0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
   9,  9,  9,  9,  9,  9,  9,  9, 22, 15, 10,  7,  6,  7, 10, 15,
  14, 11, 13, 15, 12, 13, 11, 14, 28, 21, 16, 13, 12, 13, 16, 21, 0
};

char bk[16*8+1];

unsigned int seed=0;
uint32_t  byteBoard[8];

char sym[17] = {".?pnkbrq?P?NKBRQ"};
int mn=1;
char lastH[5], lastM[5];
unsigned short ledv=1;

String inputString = "";  //Variable para guardar el inputString y pasar a la IA
bool stringComplete = false;  // whether the string is complete

const int buttonIt1 = 2,  // Pin donde está conectado el botón del contador de 1 en 1
          buttonIt4 = 7,  // Pin donde está conectado el botón del contador de 4 en 4
          buttonNext = 4, // Pin donde está conectado el botón de finalizar
          rs = 8, 
          en = 9, 
          d4 = 10, 
          d5 = 11, 
          d6 = 12, 
          d7 = 13;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int contador = 1, posicion = 0, digito;//Variables para llevar la cuenta del digito, guardar el digito, numero de inputString y en que posicion estamos
char letra;  //Variable para guardar la letra

void setup() {
  strips.begin();				// inicializacion de la tira
  strips.show();				// muestra datos en pixel
  lcd.begin(16, 2);
  pinMode(buttonIt1, INPUT);  // Configuramos el pin del botón del contador como entrada sin resistencia pull-up
  pinMode(buttonIt4, INPUT);  // Configuramos el pin del botón del contador como entrada sin resistencia pull-up
  pinMode(buttonNext, INPUT); // Configuramos el pin del botón de finalizar como entrada sin resistencia pull-up
  lastH[0] = 0;
  lcd.home();
  lcd.blink();
  lcd.print("Haga movimiento");
  lcd.setCursor(0, 1);
}

void light_casilla(char l, int n, int R, int G, int B){
    
    int i = (n-1)*32+(l-'a')*2;
    strips.setPixelColor(i, R, G, B);   // cada pixel en color azul (posicion,R,G,B)
    strips.setPixelColor(i+1, R, G, B);   // cada pixel en color azul (posicion,R,G,B)
    strips.setPixelColor(i+30-4*(l-'a'), R, G, B);   // cada pixel en color azul (posicion,R,G,B)
    strips.setPixelColor(i+31-4*(l-'a'), R, G, B);   // cada pixel en color azul (posicion,R,G,B)
    strips.setBrightness(150);
    strips.show();

}

void apagarTodo() {
    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            int index = y * 16 + x; // Calcular el índice del píxel en la tira
            strips.setPixelColor(index, 0); // Establecer el píxel a negro (0,0,0)
        }
    }
    strips.show(); // Mostrar los cambios
}

void InvalidAnimation (){
        //Casillas que queremos colorear
        int pixelIndices[] = {0,15,30,17,34,45, 60, 51, 68, 75, 90, 85, 102, 105, 120, 119, 135, 136, 153, 150, 165, 170, 187, 180, 195, 204, 221, 210, 225, 238, 255,240};
        //bucle para colorearlo de rojo
        for (int i = 0; i < sizeof(pixelIndices) / sizeof(pixelIndices[0]); i++) {
            strips.setPixelColor(pixelIndices[i], strips.Color(255,0,0));}

        strips.show();
        strips.setBrightness(100);
        delay(750);
    }

void winAnimation(){
        //W
    strips.setPixelColor(35, strips.Color(0, 255, 0));
    strips.setPixelColor(44, strips.Color(0, 255, 0));
    strips.setPixelColor(62, strips.Color(0, 255, 0));
    strips.setPixelColor(61, strips.Color(0, 255, 0));
    strips.setPixelColor(59, strips.Color(0, 255, 0));
    strips.setPixelColor(58, strips.Color(0, 255, 0));
    strips.setPixelColor(53, strips.Color(0, 255, 0));
    strips.setPixelColor(52, strips.Color(0, 255, 0));
    strips.setPixelColor(50, strips.Color(0, 255, 0));
    strips.setPixelColor(49, strips.Color(0, 255, 0));
    strips.setPixelColor(65, strips.Color(0, 255, 0));
    strips.setPixelColor(66, strips.Color(0, 255, 0));
    strips.setPixelColor(68, strips.Color(0, 255, 0));
    strips.setPixelColor(69, strips.Color(0, 255, 0));
    strips.setPixelColor(74, strips.Color(0, 255, 0));
    strips.setPixelColor(75, strips.Color(0, 255, 0));
    strips.setPixelColor(77, strips.Color(0, 255, 0));
    strips.setPixelColor(78, strips.Color(0, 255, 0));
    strips.setPixelColor(94, strips.Color(0, 255, 0));
    strips.setPixelColor(93, strips.Color(0, 255, 0));
    strips.setPixelColor(91, strips.Color(0, 255, 0));
    strips.setPixelColor(90, strips.Color(0, 255, 0));
    strips.setPixelColor(85, strips.Color(0, 255, 0));
    strips.setPixelColor(84, strips.Color(0, 255, 0));
    strips.setPixelColor(82, strips.Color(0, 255, 0));
    strips.setPixelColor(81, strips.Color(0, 255, 0));
    strips.setPixelColor(97, strips.Color(0, 255, 0));
    strips.setPixelColor(98, strips.Color(0, 255, 0));
    strips.setPixelColor(100, strips.Color(0, 255, 0));
    strips.setPixelColor(101, strips.Color(0, 255, 0));
    strips.setPixelColor(106, strips.Color(0, 255, 0));
    strips.setPixelColor(107, strips.Color(0, 255, 0));
    strips.setPixelColor(109, strips.Color(0, 255, 0));
    strips.setPixelColor(110, strips.Color(0, 255, 0));
    strips.setPixelColor(126, strips.Color(0, 255, 0));
    strips.setPixelColor(125, strips.Color(0, 255, 0));
    strips.setPixelColor(121, strips.Color(0, 255, 0));
    strips.setPixelColor(118, strips.Color(0, 255, 0));
    strips.setPixelColor(114, strips.Color(0, 255, 0));
    strips.setPixelColor(113, strips.Color(0, 255, 0));
    strips.setPixelColor(129, strips.Color(0, 255, 0));
    strips.setPixelColor(130, strips.Color(0, 255, 0));
    strips.setPixelColor(135, strips.Color(0, 255, 0));
    strips.setPixelColor(136, strips.Color(0, 255, 0));
    strips.setPixelColor(141, strips.Color(0, 255, 0));
    strips.setPixelColor(142, strips.Color(0, 255, 0));
    strips.setPixelColor(158, strips.Color(0, 255, 0));
    strips.setPixelColor(157, strips.Color(0, 255, 0));
    strips.setPixelColor(146, strips.Color(0, 255, 0));
    strips.setPixelColor(145, strips.Color(0, 255, 0));
    strips.setPixelColor(161, strips.Color(0, 255, 0));
    strips.setPixelColor(162, strips.Color(0, 255, 0));
    strips.setPixelColor(173, strips.Color(0, 255, 0));
    strips.setPixelColor(174, strips.Color(0, 255, 0));
    strips.setPixelColor(190, strips.Color(0, 255, 0));
    strips.setPixelColor(189, strips.Color(0, 255, 0));
    strips.setPixelColor(178, strips.Color(0, 255, 0));
    strips.setPixelColor(177, strips.Color(0, 255, 0));
    strips.setPixelColor(193, strips.Color(0, 255, 0));
    strips.setPixelColor(194, strips.Color(0, 255, 0));
    strips.setPixelColor(205, strips.Color(0, 255, 0));
    strips.setPixelColor(206, strips.Color(0, 255, 0));
    strips.setPixelColor(222, strips.Color(0, 255, 0));
    strips.setPixelColor(221, strips.Color(0, 255, 0));
    strips.setPixelColor(210, strips.Color(0, 255, 0));
    strips.setPixelColor(209, strips.Color(0, 255, 0));
    strips.setPixelColor(225, strips.Color(0, 255, 0));
    strips.setPixelColor(226, strips.Color(0, 255, 0));
    strips.setPixelColor(237, strips.Color(0, 255, 0));
    strips.setPixelColor(238, strips.Color(0, 255, 0));
    strips.show();
    strips.setBrightness(100);
    delay(750);
    
    //llamar a apagar todo
    apagarTodo();
    delay(250);
    //I
    strips.setPixelColor(34, strips.Color(0, 255, 0));
    strips.setPixelColor(35, strips.Color(0, 255, 0));
    strips.setPixelColor(36, strips.Color(0, 255, 0));
    strips.setPixelColor(37, strips.Color(0, 255, 0));
    strips.setPixelColor(38, strips.Color(0, 255, 0));
    strips.setPixelColor(39, strips.Color(0, 255, 0));
    strips.setPixelColor(40, strips.Color(0, 255, 0));
    strips.setPixelColor(41, strips.Color(0, 255, 0));
    strips.setPixelColor(42, strips.Color(0, 255, 0));
    strips.setPixelColor(43, strips.Color(0, 255, 0));
    strips.setPixelColor(44, strips.Color(0, 255, 0));
    strips.setPixelColor(45, strips.Color(0, 255, 0));
    strips.setPixelColor(61, strips.Color(0, 255, 0));
    strips.setPixelColor(60, strips.Color(0, 255, 0));
    strips.setPixelColor(59, strips.Color(0, 255, 0));
    strips.setPixelColor(58, strips.Color(0, 255, 0));
    strips.setPixelColor(57, strips.Color(0, 255, 0));
    strips.setPixelColor(56, strips.Color(0, 255, 0));
    strips.setPixelColor(55, strips.Color(0, 255, 0));
    strips.setPixelColor(54, strips.Color(0, 255, 0));
    strips.setPixelColor(53, strips.Color(0, 255, 0));
    strips.setPixelColor(52, strips.Color(0, 255, 0));
    strips.setPixelColor(51, strips.Color(0, 255, 0));
    strips.setPixelColor(50, strips.Color(0, 255, 0));
    strips.setPixelColor(71, strips.Color(0, 255, 0));
    strips.setPixelColor(72, strips.Color(0, 255, 0));
    strips.setPixelColor(88, strips.Color(0, 255, 0));
    strips.setPixelColor(87, strips.Color(0, 255, 0));
    strips.setPixelColor(103, strips.Color(0, 255, 0));
    strips.setPixelColor(104, strips.Color(0, 255, 0));
    strips.setPixelColor(120, strips.Color(0, 255, 0));
    strips.setPixelColor(119, strips.Color(0, 255, 0));
    strips.setPixelColor(135, strips.Color(0, 255, 0));
    strips.setPixelColor(136, strips.Color(0, 255, 0));
    strips.setPixelColor(152, strips.Color(0, 255, 0));
    strips.setPixelColor(151, strips.Color(0, 255, 0));
    strips.setPixelColor(167, strips.Color(0, 255, 0));
    strips.setPixelColor(168, strips.Color(0, 255, 0));
    strips.setPixelColor(184, strips.Color(0, 255, 0));
    strips.setPixelColor(183, strips.Color(0, 255, 0));
    strips.setPixelColor(194, strips.Color(0, 255, 0));
    strips.setPixelColor(195, strips.Color(0, 255, 0));
    strips.setPixelColor(196, strips.Color(0, 255, 0));
    strips.setPixelColor(197, strips.Color(0, 255, 0));
    strips.setPixelColor(198, strips.Color(0, 255, 0));
    strips.setPixelColor(199, strips.Color(0, 255, 0));
    strips.setPixelColor(200, strips.Color(0, 255, 0));
    strips.setPixelColor(201, strips.Color(0, 255, 0));
    strips.setPixelColor(202, strips.Color(0, 255, 0));
    strips.setPixelColor(203, strips.Color(0, 255, 0));
    strips.setPixelColor(204, strips.Color(0, 255, 0));
    strips.setPixelColor(205, strips.Color(0, 255, 0));
    strips.setPixelColor(221, strips.Color(0, 255, 0));
    strips.setPixelColor(220, strips.Color(0, 255, 0));
    strips.setPixelColor(219, strips.Color(0, 255, 0));
    strips.setPixelColor(218, strips.Color(0, 255, 0));
    strips.setPixelColor(217, strips.Color(0, 255, 0));
    strips.setPixelColor(216, strips.Color(0, 255, 0));
    strips.setPixelColor(215, strips.Color(0, 255, 0));
    strips.setPixelColor(214, strips.Color(0, 255, 0));
    strips.setPixelColor(213, strips.Color(0, 255, 0));
    strips.setPixelColor(212, strips.Color(0, 255, 0));
    strips.setPixelColor(211, strips.Color(0, 255, 0));
    strips.setPixelColor(210, strips.Color(0, 255, 0));
    strips.show();
    strips.setBrightness(100);
    delay(750);
    //llamar apagar todo
    apagarTodo ();
    delay(250);
    //encender N
    strips.setPixelColor(34, strips.Color(0, 255, 0));
    strips.setPixelColor(35, strips.Color(0, 255, 0));
    strips.setPixelColor(42, strips.Color(0, 255, 0));
    strips.setPixelColor(43, strips.Color(0, 255, 0));
    strips.setPixelColor(44, strips.Color(0, 255, 0));
    strips.setPixelColor(45, strips.Color(0, 255, 0));
    strips.setPixelColor(61, strips.Color(0, 255, 0));
    strips.setPixelColor(60, strips.Color(0, 255, 0));
    strips.setPixelColor(53, strips.Color(0, 255, 0));
    strips.setPixelColor(52, strips.Color(0, 255, 0));
    strips.setPixelColor(51, strips.Color(0, 255, 0));
    strips.setPixelColor(50, strips.Color(0, 255, 0));
    strips.setPixelColor(66, strips.Color(0, 255, 0));
    strips.setPixelColor(67, strips.Color(0, 255, 0));
    strips.setPixelColor(73, strips.Color(0, 255, 0));
    strips.setPixelColor(74, strips.Color(0, 255, 0));
    strips.setPixelColor(76, strips.Color(0, 255, 0));
    strips.setPixelColor(77, strips.Color(0, 255, 0));
    strips.setPixelColor(93, strips.Color(0, 255, 0));
    strips.setPixelColor(92, strips.Color(0, 255, 0));
    strips.setPixelColor(87, strips.Color(0, 255, 0));
    strips.setPixelColor(86, strips.Color(0, 255, 0));
    strips.setPixelColor(83, strips.Color(0, 255, 0));
    strips.setPixelColor(82, strips.Color(0, 255, 0));
    strips.setPixelColor(98, strips.Color(0, 255, 0));
    strips.setPixelColor(99, strips.Color(0, 255, 0));
    strips.setPixelColor(103, strips.Color(0, 255, 0));
    strips.setPixelColor(104, strips.Color(0, 255, 0));
    strips.setPixelColor(105, strips.Color(0, 255, 0));
    strips.setPixelColor(108, strips.Color(0, 255, 0));
    strips.setPixelColor(109, strips.Color(0, 255, 0));
    strips.setPixelColor(125, strips.Color(0, 255, 0));
    strips.setPixelColor(124, strips.Color(0, 255, 0));
    strips.setPixelColor(120, strips.Color(0, 255, 0));
    strips.setPixelColor(119, strips.Color(0, 255, 0));
    strips.setPixelColor(115, strips.Color(0, 255, 0));
    strips.setPixelColor(114, strips.Color(0, 255, 0));
    strips.setPixelColor(130, strips.Color(0, 255, 0));
    strips.setPixelColor(131, strips.Color(0, 255, 0));
    strips.setPixelColor(135, strips.Color(0, 255, 0));
    strips.setPixelColor(136, strips.Color(0, 255, 0));
    strips.setPixelColor(140, strips.Color(0, 255, 0));
    strips.setPixelColor(141, strips.Color(0, 255, 0));
    strips.setPixelColor(157, strips.Color(0, 255, 0));
    strips.setPixelColor(156, strips.Color(0, 255, 0));
    strips.setPixelColor(153, strips.Color(0, 255, 0));
    strips.setPixelColor(152, strips.Color(0, 255, 0));
    strips.setPixelColor(147, strips.Color(0, 255, 0));
    strips.setPixelColor(146, strips.Color(0, 255, 0));
    strips.setPixelColor(162, strips.Color(0, 255, 0));
    strips.setPixelColor(163, strips.Color(0, 255, 0));
    strips.setPixelColor(166, strips.Color(0, 255, 0));
    strips.setPixelColor(167, strips.Color(0, 255, 0));
    strips.setPixelColor(172, strips.Color(0, 255, 0));
    strips.setPixelColor(173, strips.Color(0, 255, 0));
    strips.setPixelColor(189, strips.Color(0, 255, 0));
    strips.setPixelColor(188, strips.Color(0, 255, 0));
    strips.setPixelColor(186, strips.Color(0, 255, 0));
    strips.setPixelColor(185, strips.Color(0, 255, 0));
    strips.setPixelColor(179, strips.Color(0, 255, 0));
    strips.setPixelColor(178, strips.Color(0, 255, 0));
    strips.setPixelColor(194, strips.Color(0, 255, 0));
    strips.setPixelColor(195, strips.Color(0, 255, 0));
    strips.setPixelColor(196, strips.Color(0, 255, 0));
    strips.setPixelColor(197, strips.Color(0, 255, 0));
    strips.setPixelColor(204, strips.Color(0, 255, 0));
    strips.setPixelColor(205, strips.Color(0, 255, 0));
    strips.setPixelColor(221, strips.Color(0, 255, 0));
    strips.setPixelColor(220, strips.Color(0, 255, 0));
    strips.setPixelColor(219, strips.Color(0, 255, 0));
    strips.setPixelColor(218, strips.Color(0, 255, 0));
    strips.setPixelColor(211, strips.Color(0, 255, 0));
    strips.setPixelColor(210, strips.Color(0, 255, 0));
    strips.show();
    strips.setBrightness(100);
    delay(750);
    //apagar
    apagarTodo();  
}

void loseAnimation(){
//MAKE LETTER "L"
    strips.setPixelColor(0, strips.Color(255, 0, 0));
    strips.setPixelColor(1, strips.Color(255, 0, 0));
    strips.setPixelColor(2, strips.Color(255, 0, 0));
    strips.setPixelColor(3, strips.Color(255, 0, 0));
    strips.setPixelColor(4, strips.Color(255, 0, 0));
    strips.setPixelColor(5, strips.Color(255, 0, 0));
    strips.setPixelColor(6, strips.Color(255, 0, 0));
    strips.setPixelColor(7, strips.Color(255, 0, 0));
    strips.setPixelColor(8, strips.Color(255, 0, 0));
    strips.setPixelColor(9, strips.Color(255, 0, 0));
    strips.setPixelColor(10, strips.Color(255, 0, 0));
    strips.setPixelColor(31, strips.Color(255, 0, 0));
    strips.setPixelColor(30, strips.Color(255, 0, 0));
    strips.setPixelColor(29, strips.Color(255, 0, 0));
    strips.setPixelColor(28, strips.Color(255, 0, 0));
    strips.setPixelColor(27, strips.Color(255, 0, 0));
    strips.setPixelColor(26, strips.Color(255, 0, 0));
    strips.setPixelColor(25, strips.Color(255, 0, 0));
    strips.setPixelColor(24, strips.Color(255, 0, 0));
    strips.setPixelColor(23, strips.Color(255, 0, 0));
    strips.setPixelColor(22, strips.Color(255, 0, 0));
    strips.setPixelColor(21, strips.Color(255, 0, 0));
    strips.setPixelColor(32, strips.Color(255, 0, 0));
    strips.setPixelColor(33, strips.Color(255, 0, 0));
    strips.setPixelColor(63, strips.Color(255, 0, 0));
    strips.setPixelColor(62, strips.Color(255, 0, 0));
    strips.setPixelColor(64, strips.Color(255, 0, 0));
    strips.setPixelColor(65, strips.Color(255, 0, 0));
    strips.setPixelColor(95, strips.Color(255, 0, 0));
    strips.setPixelColor(94, strips.Color(255, 0, 0));
    strips.setPixelColor(96, strips.Color(255, 0, 0));
    strips.setPixelColor(97, strips.Color(255, 0, 0));
    strips.setPixelColor(127, strips.Color(255, 0, 0));
    strips.setPixelColor(126, strips.Color(255, 0, 0));
    strips.setPixelColor(128, strips.Color(255, 0, 0));
    strips.setPixelColor(129, strips.Color(255, 0, 0));
    strips.setPixelColor(159, strips.Color(255, 0, 0));
    strips.setPixelColor(158, strips.Color(255, 0, 0));
    strips.setPixelColor(160, strips.Color(255, 0, 0));
    strips.setPixelColor(161, strips.Color(255, 0, 0));
    strips.setPixelColor(191, strips.Color(255, 0, 0));
    strips.setPixelColor(190, strips.Color(255, 0, 0));
    strips.setPixelColor(192, strips.Color(255, 0, 0));
    strips.setPixelColor(193, strips.Color(255, 0, 0));
    strips.setPixelColor(223, strips.Color(255, 0, 0));
    strips.setPixelColor(222, strips.Color(255, 0, 0));
    strips.setPixelColor(224, strips.Color(255, 0, 0));
    strips.setPixelColor(225, strips.Color(255, 0, 0));
    strips.setPixelColor(255, strips.Color(255, 0, 0));
    strips.setPixelColor(254, strips.Color(255, 0, 0));
    strips.setBrightness(100);
    strips.show();
    delay(750);

    //MAKE LETTER "O"
    strips.setPixelColor(34, strips.Color(255, 0, 0));
    strips.setPixelColor(35, strips.Color(255, 0, 0));
    strips.setPixelColor(36, strips.Color(255, 0, 0));
    strips.setPixelColor(37, strips.Color(255, 0, 0));
    strips.setPixelColor(38, strips.Color(255, 0, 0));
    strips.setPixelColor(39, strips.Color(255, 0, 0));
    strips.setPixelColor(40, strips.Color(255, 0, 0));
    strips.setPixelColor(41, strips.Color(255, 0, 0));
    strips.setPixelColor(42, strips.Color(255, 0, 0));
    strips.setPixelColor(43, strips.Color(255, 0, 0));
    strips.setPixelColor(44, strips.Color(255, 0, 0));
    strips.setPixelColor(45, strips.Color(255, 0, 0));
    strips.setPixelColor(61, strips.Color(255, 0, 0));
    strips.setPixelColor(60, strips.Color(255, 0, 0));
    strips.setPixelColor(59, strips.Color(255, 0, 0));
    strips.setPixelColor(58, strips.Color(255, 0, 0));
    strips.setPixelColor(57, strips.Color(255, 0, 0));
    strips.setPixelColor(56, strips.Color(255, 0, 0));
    strips.setPixelColor(55, strips.Color(255, 0, 0));
    strips.setPixelColor(54, strips.Color(255, 0, 0));
    strips.setPixelColor(53, strips.Color(255, 0, 0));
    strips.setPixelColor(52, strips.Color(255, 0, 0));
    strips.setPixelColor(51, strips.Color(255, 0, 0));
    strips.setPixelColor(50, strips.Color(255, 0, 0));
    strips.setPixelColor(66, strips.Color(255, 0, 0));
    strips.setPixelColor(67, strips.Color(255, 0, 0));
    strips.setPixelColor(76, strips.Color(255, 0, 0));
    strips.setPixelColor(77, strips.Color(255, 0, 0));
    strips.setPixelColor(93, strips.Color(255, 0, 0));
    strips.setPixelColor(92, strips.Color(255, 0, 0));
    strips.setPixelColor(83, strips.Color(255, 0, 0));
    strips.setPixelColor(82, strips.Color(255, 0, 0));
    strips.setPixelColor(98, strips.Color(255, 0, 0));
    strips.setPixelColor(99, strips.Color(255, 0, 0));
    strips.setPixelColor(108, strips.Color(255, 0, 0));
    strips.setPixelColor(109, strips.Color(255, 0, 0));
    strips.setPixelColor(125, strips.Color(255, 0, 0));
    strips.setPixelColor(124, strips.Color(255, 0, 0));
    strips.setPixelColor(115, strips.Color(255, 0, 0));
    strips.setPixelColor(114, strips.Color(255, 0, 0));
    strips.setPixelColor(130, strips.Color(255, 0, 0));
    strips.setPixelColor(131, strips.Color(255, 0, 0));
    strips.setPixelColor(140, strips.Color(255, 0, 0));
    strips.setPixelColor(141, strips.Color(255, 0, 0));
    strips.setPixelColor(157, strips.Color(255, 0, 0));
    strips.setPixelColor(156, strips.Color(255, 0, 0));
    strips.setPixelColor(147, strips.Color(255, 0, 0));
    strips.setPixelColor(146, strips.Color(255, 0, 0));
    strips.setPixelColor(162, strips.Color(255, 0, 0));
    strips.setPixelColor(163, strips.Color(255, 0, 0));
    strips.setPixelColor(172, strips.Color(255, 0, 0));
    strips.setPixelColor(173, strips.Color(255, 0, 0));
    strips.setPixelColor(189, strips.Color(255, 0, 0));
    strips.setPixelColor(188, strips.Color(255, 0, 0));
    strips.setPixelColor(179, strips.Color(255, 0, 0));
    strips.setPixelColor(178, strips.Color(255, 0, 0));
    strips.setPixelColor(194, strips.Color(255, 0, 0));
    strips.setPixelColor(195, strips.Color(255, 0, 0));
    strips.setPixelColor(196, strips.Color(255, 0, 0));
    strips.setPixelColor(197, strips.Color(255, 0, 0));
    strips.setPixelColor(198, strips.Color(255, 0, 0));
    strips.setPixelColor(199, strips.Color(255, 0, 0));
    strips.setPixelColor(200, strips.Color(255, 0, 0));
    strips.setPixelColor(201, strips.Color(255, 0, 0));
    strips.setPixelColor(202, strips.Color(255, 0, 0));
    strips.setPixelColor(203, strips.Color(255, 0, 0));
    strips.setPixelColor(204, strips.Color(255, 0, 0));
    strips.setPixelColor(205, strips.Color(255, 0, 0));
    strips.setPixelColor(221, strips.Color(255, 0, 0));
    strips.setPixelColor(220, strips.Color(255, 0, 0));
    strips.setPixelColor(219, strips.Color(255, 0, 0));
    strips.setPixelColor(218, strips.Color(255, 0, 0));
    strips.setPixelColor(217, strips.Color(255, 0, 0));
    strips.setPixelColor(216, strips.Color(255, 0, 0));
    strips.setPixelColor(215, strips.Color(255, 0, 0));
    strips.setPixelColor(214, strips.Color(255, 0, 0));
    strips.setPixelColor(213, strips.Color(255, 0, 0));
    strips.setPixelColor(212, strips.Color(255, 0, 0));
    strips.setPixelColor(211, strips.Color(255, 0, 0));
    strips.setPixelColor(210, strips.Color(255, 0, 0));
    strips.setBrightness(100);
    strips.show();
    delay(750);

    //MAKE LETTER "S"
    strips.setPixelColor(34, strips.Color(255, 0, 0));
    strips.setPixelColor(35, strips.Color(255, 0, 0));
    strips.setPixelColor(36, strips.Color(255, 0, 0));
    strips.setPixelColor(37, strips.Color(255, 0, 0));
    strips.setPixelColor(38, strips.Color(255, 0, 0));
    strips.setPixelColor(39, strips.Color(255, 0, 0));
    strips.setPixelColor(40, strips.Color(255, 0, 0));
    strips.setPixelColor(41, strips.Color(255, 0, 0));
    strips.setPixelColor(42, strips.Color(255, 0, 0));
    strips.setPixelColor(43, strips.Color(255, 0, 0));
    strips.setPixelColor(61, strips.Color(255, 0, 0));
    strips.setPixelColor(60, strips.Color(255, 0, 0));
    strips.setPixelColor(59, strips.Color(255, 0, 0));
    strips.setPixelColor(58, strips.Color(255, 0, 0));
    strips.setPixelColor(57, strips.Color(255, 0, 0));
    strips.setPixelColor(56, strips.Color(255, 0, 0));
    strips.setPixelColor(55, strips.Color(255, 0, 0));
    strips.setPixelColor(54, strips.Color(255, 0, 0));
    strips.setPixelColor(53, strips.Color(255, 0, 0));
    strips.setPixelColor(52, strips.Color(255, 0, 0));
    strips.setPixelColor(74, strips.Color(255, 0, 0));
    strips.setPixelColor(75, strips.Color(255, 0, 0));
    strips.setPixelColor(85, strips.Color(255, 0, 0));
    strips.setPixelColor(84, strips.Color(255, 0, 0));
    strips.setPixelColor(106, strips.Color(255, 0, 0));
    strips.setPixelColor(107, strips.Color(255, 0, 0));
    strips.setPixelColor(117, strips.Color(255, 0, 0));
    strips.setPixelColor(116, strips.Color(255, 0, 0));
    strips.setPixelColor(130, strips.Color(255, 0, 0));
    strips.setPixelColor(131, strips.Color(255, 0, 0));
    strips.setPixelColor(132, strips.Color(255, 0, 0));
    strips.setPixelColor(133, strips.Color(255, 0, 0));
    strips.setPixelColor(134, strips.Color(255, 0, 0));
    strips.setPixelColor(135, strips.Color(255, 0, 0));
    strips.setPixelColor(136, strips.Color(255, 0, 0));
    strips.setPixelColor(137, strips.Color(255, 0, 0));
    strips.setPixelColor(138, strips.Color(255, 0, 0));
    strips.setPixelColor(139, strips.Color(255, 0, 0));
    strips.setPixelColor(157, strips.Color(255, 0, 0));
    strips.setPixelColor(156, strips.Color(255, 0, 0));
    strips.setPixelColor(155, strips.Color(255, 0, 0));
    strips.setPixelColor(154, strips.Color(255, 0, 0));
    strips.setPixelColor(153, strips.Color(255, 0, 0));
    strips.setPixelColor(152, strips.Color(255, 0, 0));
    strips.setPixelColor(151, strips.Color(255, 0, 0));
    strips.setPixelColor(150, strips.Color(255, 0, 0));
    strips.setPixelColor(149, strips.Color(255, 0, 0));
    strips.setPixelColor(148, strips.Color(255, 0, 0));
    strips.setPixelColor(162, strips.Color(255, 0, 0));
    strips.setPixelColor(163, strips.Color(255, 0, 0));
    strips.setPixelColor(189, strips.Color(255, 0, 0));
    strips.setPixelColor(188, strips.Color(255, 0, 0));
    strips.setPixelColor(194, strips.Color(255, 0, 0));
    strips.setPixelColor(195, strips.Color(255, 0, 0));
    strips.setPixelColor(196, strips.Color(255, 0, 0));
    strips.setPixelColor(197, strips.Color(255, 0, 0));
    strips.setPixelColor(198, strips.Color(255, 0, 0));
    strips.setPixelColor(199, strips.Color(255, 0, 0));
    strips.setPixelColor(200, strips.Color(255, 0, 0));
    strips.setPixelColor(201, strips.Color(255, 0, 0));
    strips.setPixelColor(202, strips.Color(255, 0, 0));
    strips.setPixelColor(203, strips.Color(255, 0, 0));
    strips.setPixelColor(204, strips.Color(255, 0, 0));
    strips.setPixelColor(205, strips.Color(255, 0, 0));
    strips.setPixelColor(221, strips.Color(255, 0, 0));
    strips.setPixelColor(220, strips.Color(255, 0, 0));
    strips.setPixelColor(219, strips.Color(255, 0, 0));
    strips.setPixelColor(218, strips.Color(255, 0, 0));
    strips.setPixelColor(217, strips.Color(255, 0, 0));
    strips.setPixelColor(216, strips.Color(255, 0, 0));
    strips.setPixelColor(215, strips.Color(255, 0, 0));
    strips.setPixelColor(214, strips.Color(255, 0, 0));
    strips.setPixelColor(213, strips.Color(255, 0, 0));
    strips.setPixelColor(212, strips.Color(255, 0, 0));
    strips.setPixelColor(211, strips.Color(255, 0, 0));
    strips.setPixelColor(210, strips.Color(255, 0, 0));
    strips.setBrightness(100);
    strips.show();
    delay(750);

    //MAKE LETTER "E"
    strips.setPixelColor(34, strips.Color(255, 0, 0));
    strips.setPixelColor(35, strips.Color(255, 0, 0));
    strips.setPixelColor(36, strips.Color(255, 0, 0));
    strips.setPixelColor(37, strips.Color(255, 0, 0));
    strips.setPixelColor(38, strips.Color(255, 0, 0));
    strips.setPixelColor(39, strips.Color(255, 0, 0));
    strips.setPixelColor(40, strips.Color(255, 0, 0));
    strips.setPixelColor(41, strips.Color(255, 0, 0));
    strips.setPixelColor(42, strips.Color(255, 0, 0));
    strips.setPixelColor(43, strips.Color(255, 0, 0));
    strips.setPixelColor(44, strips.Color(255, 0, 0));
    strips.setPixelColor(45, strips.Color(255, 0, 0));
    strips.setPixelColor(61, strips.Color(255, 0, 0));
    strips.setPixelColor(60, strips.Color(255, 0, 0));
    strips.setPixelColor(59, strips.Color(255, 0, 0));
    strips.setPixelColor(58, strips.Color(255, 0, 0));
    strips.setPixelColor(57, strips.Color(255, 0, 0));
    strips.setPixelColor(56, strips.Color(255, 0, 0));
    strips.setPixelColor(55, strips.Color(255, 0, 0));
    strips.setPixelColor(54, strips.Color(255, 0, 0));
    strips.setPixelColor(53, strips.Color(255, 0, 0));
    strips.setPixelColor(52, strips.Color(255, 0, 0));
    strips.setPixelColor(51, strips.Color(255, 0, 0));
    strips.setPixelColor(50, strips.Color(255, 0, 0));
    strips.setPixelColor(66, strips.Color(255, 0, 0));
    strips.setPixelColor(67, strips.Color(255, 0, 0));
    strips.setPixelColor(93, strips.Color(255, 0, 0));
    strips.setPixelColor(92, strips.Color(255, 0, 0));
    strips.setPixelColor(98, strips.Color(255, 0, 0));
    strips.setPixelColor(99, strips.Color(255, 0, 0));
    strips.setPixelColor(125, strips.Color(255, 0, 0));
    strips.setPixelColor(124, strips.Color(255, 0, 0));
    strips.setPixelColor(123, strips.Color(255, 0, 0));
    strips.setPixelColor(122, strips.Color(255, 0, 0));
    strips.setPixelColor(121, strips.Color(255, 0, 0));
    strips.setPixelColor(120, strips.Color(255, 0, 0));
    strips.setPixelColor(119, strips.Color(255, 0, 0));
    strips.setPixelColor(130, strips.Color(255, 0, 0));
    strips.setPixelColor(131, strips.Color(255, 0, 0));
    strips.setPixelColor(132, strips.Color(255, 0, 0));
    strips.setPixelColor(133, strips.Color(255, 0, 0));
    strips.setPixelColor(134, strips.Color(255, 0, 0));
    strips.setPixelColor(135, strips.Color(255, 0, 0));
    strips.setPixelColor(136, strips.Color(255, 0, 0));
    strips.setPixelColor(157, strips.Color(255, 0, 0));
    strips.setPixelColor(156, strips.Color(255, 0, 0));
    strips.setPixelColor(162, strips.Color(255, 0, 0));
    strips.setPixelColor(163, strips.Color(255, 0, 0));
    strips.setPixelColor(189, strips.Color(255, 0, 0));
    strips.setPixelColor(188, strips.Color(255, 0, 0));
    strips.setPixelColor(194, strips.Color(255, 0, 0));
    strips.setPixelColor(195, strips.Color(255, 0, 0));
    strips.setPixelColor(196, strips.Color(255, 0, 0));
    strips.setPixelColor(197, strips.Color(255, 0, 0));
    strips.setPixelColor(198, strips.Color(255, 0, 0));
    strips.setPixelColor(199, strips.Color(255, 0, 0));
    strips.setPixelColor(200, strips.Color(255, 0, 0));
    strips.setPixelColor(201, strips.Color(255, 0, 0));
    strips.setPixelColor(202, strips.Color(255, 0, 0));
    strips.setPixelColor(203, strips.Color(255, 0, 0));
    strips.setPixelColor(204, strips.Color(255, 0, 0));
    strips.setPixelColor(205, strips.Color(255, 0, 0));
    strips.setPixelColor(221, strips.Color(255, 0, 0));
    strips.setPixelColor(220, strips.Color(255, 0, 0));
    strips.setPixelColor(219, strips.Color(255, 0, 0));
    strips.setPixelColor(218, strips.Color(255, 0, 0));
    strips.setPixelColor(217, strips.Color(255, 0, 0));
    strips.setPixelColor(216, strips.Color(255, 0, 0));
    strips.setPixelColor(215, strips.Color(255, 0, 0));
    strips.setPixelColor(214, strips.Color(255, 0, 0));
    strips.setPixelColor(213, strips.Color(255, 0, 0));
    strips.setPixelColor(212, strips.Color(255, 0, 0));
    strips.setPixelColor(211, strips.Color(255, 0, 0));
    strips.setPixelColor(210, strips.Color(255, 0, 0));
    strips.setBrightness(100);
    strips.show();
    delay(750);
    //apagar
    apagarTodo(); 
}

void topeDigito(){  //Resetear contador de digitos o aumentarlo
    contador++;
    if (contador >= 9)
        contador %= 8;
}

void topeDigito4(){  ////Resetear contador de digitos de 4 en 4 o aumentarlo
    contador+=4;
    if (contador >= 9)
        contador %= 8;
}

void print_movement(char lm[5]){
    apagarTodo();
    char letra_aux;
    int num_aux;
    letra_aux = lm[0];
    num_aux = lm[1] - '0';
    light_casilla(letra_aux, num_aux, 0, 0, 255);
    letra_aux = lm[2];
    num_aux = lm[3] - '0'; 
    light_casilla(letra_aux, num_aux, 127, 0, 255); 
    String aux = String(lm);  
    lcd.setCursor(7, 0);
    lcd.print(aux.substring(0, 4));
    lcd.setCursor(0, 1);
}

void topePosicion(){  //Aumentar contador de posicion y setCursor diplay, o resetearlo que implica nuevo inputString luego clear de display
    posicion++;
    if (posicion >= 5){  //Aqui solo se llega cuando ya se ha finalizado un inputString y ha sido aceptado
      lcd.clear();
      lcd.print(mn);
      lcd.print(".");
      lcd.print(inputString.substring(0, 4));
      print_movement(lastM);
      posicion = 0;
    }
    lcd.setCursor(posicion, posicion + 1);
}

void anulado(){//Vaciar String
    inputString.remove(0);
    lcd.clear();
    lcd.print("Repitalo");
    lcd.setCursor(0, 1);
    posicion = 0;
}

void invalid(){//Vaciar String
    lcd.clear();
    apagarTodo();
    InvalidAnimation();
    lcd.print("Incorrecto");
    lcd.setCursor(0, 1);
    posicion = 0;
}

void lose(){
  lcd.clear();
  lcd.print("Has perdido");
  loseAnimation();
}

void win(){
  lcd.clear();
  lcd.print("Has ganado");
  winAnimation();
}

void accionTecla1() { //Iterar digitos/letras de 1 en 1 o aceptar inputString
    if(posicion == 4){
        inputString += '\n';
        //Aqui el String ya está completo y se pasa a la IA
        /*Comprobar si inputString incorrecto, si es incorrecto la IA NO lo guarda y se llama a invalid(), si es correcto si lo guarda
        if (inputString incorrecto){
          invalid();
        }else{las 4 lineas de abajo}*/
        lcd.clear();
        lcd.print("Aceptado");
        lcd.setCursor(0, 1);
        lcd.print("Espere");
        delay(3000); //Delay para poder leer
        topePosicion();
    }else if(posicion % 2 == 0){
        topeDigito();
        letra = 'a' + (contador-1);
        lcd.print(letra);
        lcd.setCursor(posicion, posicion + 1);
    }else{
        topeDigito();
        lcd.print(contador);
        lcd.setCursor(posicion, posicion + 1);
    }
}

void accionTecla4() { //Iterar digitos/letras de 4 en 4 o rechazar inputString
    if(posicion == 4){
        lcd.clear();
        lcd.print("Anulado");
        delay(3000); //Delay para poder leer
        anulado(); //Se resetea posicion y vuelta a empezar
    }else if(posicion % 2 == 0){
        topeDigito4();
        letra = 'a' + (contador-1);
        lcd.print(letra);
        lcd.setCursor(posicion, posicion + 1);
    }else{
        topeDigito4();
        lcd.print(contador);
        lcd.setCursor(posicion, posicion + 1);
    }
}

void accionTeclaSiguiente() { //Introducir siguiente letra/digito y control de posicion
    if(posicion!=4){
        if(posicion % 2 == 0){
            letra = 'a' + (contador-1);
            inputString += letra;
        }else{
            digito = contador;
            char digito_char = digito + '0'; // Convertir el entero a su representación de caracter
            inputString += digito_char;
            if(posicion == 3){
              lcd.clear();
              lcd.print("Es correcto?");
              lcd.setCursor(posicion, posicion + 1);
              lcd.print(inputString);
            }
        }
        topePosicion(); //Avanzar posición o resetearla a 0 si alcanzo el tope
        contador=1; //Siempre que se introduzca una letra/numero se resetea el contador
    }
}

void read_movement(){
    // Verificamos si se ha pulsado el botón del contador de 1 en 1
  if (digitalRead(buttonIt1) == HIGH) {
    delay(50);  // Esperamos un breve periodo para evitar rebotes
    if (digitalRead(buttonIt1) == HIGH) {
      accionTecla1();
      // Esperamos a que se suelte el botón 
      while (digitalRead(buttonIt1) == HIGH) {
        delay(50);
      }
    }
  }

// Verificamos si se ha pulsado el botón del contador de 4 en 4
  if (digitalRead(buttonIt4) == HIGH) {
    delay(50);  // Esperamos un breve periodo para evitar rebotes
    if (digitalRead(buttonIt4) == HIGH) {
      accionTecla4();
      // Esperamos a que se suelte el botón de finalizar
      while (digitalRead(buttonIt4) == HIGH) {
        delay(50);
      }
    }
  }
  
  // Verificamos si se ha pulsado el botón de finalizar
  if (digitalRead(buttonNext) == HIGH) {
    delay(50); // Esperamos un breve periodo para evitar rebotes
    if (digitalRead(buttonNext) == HIGH) {
      accionTeclaSiguiente();
      while (digitalRead(buttonNext) == HIGH) {
        delay(50);
      }
    }
  }
}

void loop() {
  int r;
  // Take move from human
  while (inputString.length() < 5) {
    read_movement();
  }
  c[0] = inputString.charAt(0);
  c[1] = inputString.charAt(1);
  c[2] = inputString.charAt(2);
  c[3] = inputString.charAt(3);
  c[4] = 0;
  // clear the string:
  inputString = "";
  stringComplete = false;
  /* Turn for ARDUINO */

  K = *c - 16 * c[1] + 799, L = c[2] - 16 * c[3] + 799; /* parse entered move */
  N = 0;
  T = 0x3F;                                 /* T=Computer Play strength */
  bkp();                                    /* Save the board just in case */
  r = D(-I, I, Q, O, 1, 3);                 /* Check & do the human movement */
  if ( !(r > -I + 1) ) {
    lose();
    gameOver();
  }
  if (k == 0x10) {                          /* The flag turn must change to 0x08 */
    invalid();
    return;
  }

  strcpy(lastH, c);                         /* Valid human movement */

  mn++;                                     /* Next move */

  K = I;
  N = 0;
  T = 0x3F;                                 /* T=Computer Play strength */
  r = D(-I, I, Q, O, 1, 3);                 /* Think & do*/
  if ( !(r > -I + 1) ) {
    win();
    gameOver();
  }

  strcpy(lastM, c);                         /* Valid ARDUINO movement */
  r = D(-I, I, Q, O, 1, 3);
  if ( !(r > -I + 1) ) {
    print_movement(lastM);
    lose();
    gameOver();
  }
   print_movement(lastM);
}


/* User interface routines */

unsigned short myrand(void) {
  unsigned short r = (unsigned short)(seed % MYRAND_MAX);
  return r = ((r << 11) + (r << 7) + r) >> 1;
}
/* recursive minimax search */
/* (q,l)=window, e=current eval. score, */
/* E=e.p. sqr.z=prev.dest, n=depth; return score */
short D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n) {
  short m, v, i, P, V, s;
  unsigned char t, p, u, x, y, X, Y, H, B, j, d, h, F, G, C;
  signed char r;
  if (++Z > 30) {                                   /* stack underrun check */
    --Z; return e;
  }
  q--;                                          /* adj. window: delay bonus */
  k ^= 24;                                      /* change sides             */
  d = Y = 0;                                    /* start iter. from scratch */
  X = myrand() & ~M;                            /* start at random field    */
  W(d++ < n || d < 3 ||                         /* iterative deepening loop */
    z & K == I && (N < T & d < 98 ||            /* root: deepen upto time   */
                   (K = X, L = Y & ~M, d = 3)))                /* time's up: go do best    */
  { x = B = X;                                   /* start scan at prev. best */
    h = Y & S;                                   /* request try noncastl. 1st*/
    P = d < 3 ? I : D(-l, 1 - l, -e, S, 0, d - 3); /* Search null move         */
    m = -P < l | R > 35 ? d > 2 ? -I : e : -P;   /* Prune or stand-pat       */
    ++N;                                         /* node count (for timing)  */
    do {
      u = b[x];                                   /* scan board looking for   */
      if (u & k) {                                /*  own piece (inefficient!)*/
        r = p = u & 7;                             /* p = piece type (set r>0) */
        j = o[p + 16];                             /* first step vector f.piece*/
        W(r = p > 2 & r < 0 ? -r : -o[++j])        /* loop over directions o[] */
        { A:                                        /* resume normal after best */
          y = x; F = G = S;                         /* (x,y)=move, (F,G)=castl.R*/
          do {                                      /* y traverses ray, or:     */
            H = y = h ? Y ^ h : y + r;               /* sneak in prev. best move */
            if (y & M)break;                         /* board edge hit           */
            m = E - S & b[E] && y - E < 2 & E - y < 2 ? I : m; /* bad castling             */
            if (p < 3 & y == E)H ^= 16;              /* shift capt.sqr. H if e.p.*/
            t = b[H]; if (t & k | p < 3 & !(y - x & 7) - !t)break; /* capt. own, bad pawn mode */
            i = 37 * w[t & 7] + (t & 192);           /* value of capt. piece t   */
            m = i < 0 ? I : m;                       /* K capture                */
            if (m >= l & d > 1)goto C;               /* abort on fail high       */
            v = d - 1 ? e : i - p;                   /* MVV/LVA scoring          */
            if (d - !t > 1)                          /* remaining depth          */
            { v = p < 6 ? b[x + 8] - b[y + 8] : 0;    /* center positional pts.   */
              b[G] = b[H] = b[x] = 0; b[y] = u | 32;  /* do move, set non-virgin  */
              if (!(G & M))b[F] = k + 6, v += 50;     /* castling: put R & score  */
              v -= p - 4 | R > 29 ? 0 : 20;           /* penalize mid-game K move */
              if (p < 3)                              /* pawns:                   */
              { v -= 9 * ((x - 2 & M || b[x - 2] - u) + /* structure, undefended    */
                          (x + 2 & M || b[x + 2] - u) - 1  /*        squares plus bias */
                          + (b[x ^ 16] == k + 36))          /* kling to non-virgin King */
                     - (R >> 2);                       /* end-game Pawn-push bonus */
                V = y + r + 1 & S ? 647 - p : 2 * (u & y + 16 & 32); /* promotion or 6/7th bonus */
                b[y] += V; i += V;                     /* change piece, add score  */
              }
              v += e + i; V = m > q ? m : q;          /* new eval and alpha       */
              C = d - 1 - (d > 5 & p > 2 & !t & !h);
              C = R > 29 | d < 3 | P - I ? C : d;     /* extend 1 ply if in check */
              do
                s = C > 2 | v > V ? -D(-l, -V, -v,     /* recursive eval. of reply */
                                       F, 0, C) : v;    /* or fail low if futile    */
              W(s > q&++C < d); v = s;
              if (z && K - I && v + I && x == K & y == L) /* move pending & in root:  */
              { Q = -e - i; O = F;                     /*   exit if legal & found  */
                R += i >> 7; --Z; return l;            /* captured non-P material  */
              }
              b[G] = k + 6; b[F] = b[y] = 0; b[x] = u; b[H] = t; /* undo move,G can be dummy */
            }
            if (v > m)                               /* new best, update max,best*/
              m = v, X = x, Y = y | S & F;            /* mark double move with S  */
            if (h) {
              h = 0;  /* redo after doing old best*/
              goto A;
            }
            if (x + r - y | u & 32 |                 /* not 1st step,moved before*/
                p > 2 & (p - 4 | j - 7 ||             /* no P & no lateral K move,*/
                         b[G = x + 3 ^ r >> 1 & 7] - k - 6     /* no virgin R in corner G, */
                         || b[G ^ 1] | b[G ^ 2])               /* no 2 empty sq. next to R */
               )t += p < 5;                           /* fake capt. for nonsliding*/
            else F = y;                              /* enable e.p.              */
          } W(!t);                                  /* if not capt. continue ray*/
        }
      }
    } W((x = x + 9 & ~M) - B);                 /* next sqr. of board, wrap */
C: if (m > I - M | m < M - I)d = 98;           /* mate holds to any depth  */
    m = m + I | P == I ? m : 0;                  /* best loses K: (stale)mate*/
    if (z && d > 2)
    { *c = 'a' + (X & 7); c[1] = '8' - (X >> 4); c[2] = 'a' + (Y & 7); c[3] = '8' - (Y >> 4 & 7); c[4] = 0;
      char buff[150];
    }
  }                                             /*    encoded in X S,8 bits */
  k ^= 24;                                      /* change sides back        */
  --Z; return m += m < e;                       /* delayed-loss bonus       */
}
void gameOver() {
  for (;;);
}
void bkp() {
  for (int i = 0; i < 16 * 8 + 1; i++) {
    bk[i] = b[i];
  }
}

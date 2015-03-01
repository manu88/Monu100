//
//  Chars.h
//  MicMonu100
//
//  Created by Manuel Deneu on 25/02/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_Chars_h
#define MicMonu100_Chars_h





/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Test grandes lettres
*/
#define CHAR_WIDTH  5
#define CHAR_HEIGHT 9

uint8_t letter_a[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b01110000,
    0b00001000,
    0b01111000,
    0b10001000,
    0b01111000,
    0b00000000,
};

uint8_t letter_b[CHAR_HEIGHT] =
{
    0b00000000,
    0b10000000,
    0b10000000,
    0b11110000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b11110000,
    0b00000000,
};

uint8_t letter_c[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b01110000,
    0b10001000,
    0b10000000,
    0b10001000,
    0b01110000,
    0b00000000,
};

uint8_t letter_d[CHAR_HEIGHT] =
{
    0b00000000,
    0b00001000,
    0b00001000,
    0b01111000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b01111000,
    0b00000000,
};

uint8_t letter_e[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b01110000,
    0b10001000,
    0b11111000,
    0b10000000,
    0b01111000,
    0b00000000,
};

uint8_t letter_f[CHAR_HEIGHT] =
{
    0b00000000,
    0b00110000,
    0b01000000,
    0b11100000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00000000,
};

uint8_t letter_g[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b01111000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b01111000,
    0b00001000,
    0b00011000,
};

uint8_t letter_h[CHAR_HEIGHT] =
{
    0b00000000,
    0b10000000,
    0b10000000,
    0b11110000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b00000000,
};

uint8_t letter_i[CHAR_HEIGHT] =
{
    0b00000000,
    0b00100000,
    0b00000000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00000000,
};

uint8_t letter_l[CHAR_HEIGHT] =
{
    0b00000000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00000000,
};

uint8_t letter_n[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b11110000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b00000000,
};

uint8_t letter_r[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b10110000,
    0b11000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b00000000,
};

uint8_t letter_u[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b01110000,
    0b00000000,
};

/* CAPITALS */

uint8_t letter_C[CHAR_HEIGHT] =
{
    0b00000000,
    0b01110000,
    0b10001000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10001000,
    0b01110000,
    0b00000000,
};

uint8_t letter_D[CHAR_HEIGHT] =
{
    0b00000000,
    0b11110000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b11110000,
    0b00000000,
};


uint8_t letter_I[CHAR_HEIGHT] =
{
    0b00000000,
    0b01110000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b01110000,
    0b00000000,
};

uint8_t letter_M[CHAR_HEIGHT] =
{
    0b00000000,
    0b10001000,
    0b11011000,
    0b10101000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b00000000,
};

/* NUMBERS */

uint8_t letter_0[CHAR_HEIGHT] =
{
    0b00000000,
    0b01110000,
    0b11001000,
    0b10011000,
    0b10101000,
    0b11001000,
    0b10001000,
    0b01110000,
    0b00000000,
};

uint8_t letter_1[CHAR_HEIGHT] =
{
    0b00000000,
    0b00110000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000,
};

uint8_t letter_2[CHAR_HEIGHT] =
{
    0b00000000,
    0b11110000,
    0b00001000,
    0b00001000,
    0b01110000,
    0b10000000,
    0b10000000,
    0b11111000,
    0b00000000,
};

uint8_t letter_3[CHAR_HEIGHT] =
{
    0b00000000,
    0b11110000,
    0b00001000,
    0b00001000,
    0b01110000,
    0b00001000,
    0b00001000,
    0b11110000,
    0b00000000,
};

uint8_t letter_4[CHAR_HEIGHT] =
{
    0b00000000,
    0b00010000,
    0b00110000,
    0b01010000,
    0b10010000,
    0b11111000,
    0b00010000,
    0b00010000,
    0b00000000,
};

uint8_t letter_5[CHAR_HEIGHT] =
{
    0b00000000,
    0b11111000,
    0b10000000,
    0b10000000,
    0b11110000,
    0b00001000,
    0b00001000,
    0b11110000,
    0b00000000,
};

uint8_t letter_6[CHAR_HEIGHT] =
{
    0b00000000,
    0b01110000,
    0b10000000,
    0b10000000,
    0b11110000,
    0b10001000,
    0b10001000,
    0b01110000,
    0b00000000,
};

uint8_t letter_7[CHAR_HEIGHT] =
{
    0b00000000,
    0b11111000,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b01000000,
    0b00000000,
};


uint8_t letter_8[CHAR_HEIGHT] =
{
    0b00000000,
    0b01110000,
    0b10001000,
    0b10001000,
    0b01110000,
    0b10001000,
    0b10001000,
    0b01110000,
    0b00000000,
};

uint8_t letter_9[CHAR_HEIGHT] =
{
    0b00000000,
    0b01110000,
    0b10001000,
    0b10001000,
    0b01111000,
    0b00001000,
    0b00001000,
    0b01110000,
    0b00000000,
};

/* SPECIALS */

uint8_t letter_space[CHAR_HEIGHT] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
};

uint8_t letter_NULL[CHAR_HEIGHT] =
{
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
    0b11111000,
};



inline uint8_t* getChar(const char c)
{
    switch (c)
    {
        case '0':
            return letter_0;
            break;
            
        case '1':
            return letter_1;
            break;
            
        case '2':
            return letter_2;
            break;
            
        case '3':
            return letter_3;
            break;
            
        case '4':
            return letter_4;
            break;
        case '5':
            return letter_5;
            break;
        case '6':
            return letter_6;
            break;
        case '7':
            return letter_7;
            break;
        case '8':
            return letter_8;
            break;
        case '9':
            return letter_9;
            break;
            
        case 'a':
            return letter_a;
            break;
            
        case 'b':
            return letter_b;
            break;
            
        case 'c':
            return letter_c;
            break;
            
        case 'C':
            return letter_C;
            break;
            
            
            
        case 'd':
            return letter_d;
            break;
            
        case 'D':
            return letter_D;
            break;
            
        case 'e':
            return letter_e;
            break;
            
        case 'f':
            return letter_f;
            break;
            
        case 'g':
            return letter_g;
            break;
            
        case 'h':
            return letter_h;
            break;
            
        case 'i':
            return letter_i;
            break;
            
        case 'I':
            return letter_I;
            break;
            
        case 'l':
            return letter_l;
            break;
            
        case 'M':
            return letter_M;
            break;
            
        case 'n':
            return letter_n;
            break;


        case 'r':
            return letter_r;
            break;

            
        case 'u':
            return letter_u;
            break;

            
/* TO DO*/
            /*
        case 'k':
            return letter_k;
            break;
            

        case 'j':
            return letter_j;
            break;
            
        case 'm':
            return letter_m;
            break;
            
        case 'o':
            return letter_o;
            break;
            
        case 'p':
            return letter_p;
            break;
            
        case 'q':
            return letter_q;
            break;
            
        case 's':
            return letter_s;
            break;
        case 't':
            return letter_t;
            break;
            
        case 'v':
            return letter_v;
            break;
            
        case 'w':
            return letter_w;
            break;
        case 'x':
            return letter_x;
            break;
            
        case 'y':
            return letter_y;
            break;
            
        case 'z':
            return letter_z;
            break;
             */
            
        default:
            return letter_NULL;
            break;
    }
}




#endif

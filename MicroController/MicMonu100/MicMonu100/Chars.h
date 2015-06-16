//
//  Chars.h
//  MicMonu100
//
//  Created by Manuel Deneu on 25/02/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_Chars_h
#define MicMonu100_Chars_h



#include <string.h>

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Test grandes lettres
*/
#define CHAR_WIDTH  8
#define CHAR_HEIGHT 10

#define CHAR_ADVANCE 8

typedef struct
{
    uint8_t buf[CHAR_HEIGHT];
    uint8_t advance;
    
} Glyphe;

/* ********************************************************************** */
const Glyphe c_0 =
{
	{
		0b00111100,
		0b01111110,
		0b01100110,
		0b01100110,
		0b01100110,
		0b01100110,
		0b01100110,
		0b01111110,
		0b00111100,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_1 =
{
	{
		0b00111100,
		0b00111100,
		0b00001100,
		0b00001100,
		0b00001100,
		0b00001100,
		0b00001100,
		0b00001100,
		0b00001100,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_2 =
{
	{
		0b01111110,
		0b01111110,
		0b01100110,
		0b00001100,
		0b00011000,
		0b00110000,
		0b01100000,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_3 =
{
	{
		0b01111110,
		0b01111110,
		0b01000110,
		0b00011110,
		0b00011100,
		0b00011110,
		0b01000110,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_4 =
{
	{
		0b01100110,
		0b01100110,
		0b01100110,
		0b01111110,
		0b01111110,
		0b00000110,
		0b00000110,
		0b00000110,
		0b00000110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_5 =
{
	{
		0b01111110,
		0b01111110,
		0b01100000,
		0b01111100,
		0b01111110,
		0b00000110,
		0b00000110,
		0b01111110,
		0b01111100,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_6 =
{
	{
		0b01111110,
		0b01111110,
		0b01100000,
		0b01111110,
		0b01111110,
		0b01100110,
		0b01100110,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_7 =
{
	{
		0b01111110,
		0b01111110,
		0b00000110,
		0b00001110,
		0b00011100,
		0b00111000,
		0b00110000,
		0b00110000,
		0b00110000,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_8 =
{
	{
		0b01111110,
		0b01111110,
		0b01100110,
		0b01111110,
		0b01111110,
		0b01100110,
		0b01100110,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_9 =
{
	{
		0b01111110,
		0b01111110,
		0b01100110,
		0b01111110,
		0b01111110,
		0b00000110,
		0b00000110,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_A =
{
	{
		0b00111100,
		0b00111100,
		0b11000011,
		0b11000011,
		0b11111111,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11000011,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_B =
{
	{
		0b11111100,
		0b11111110,
		0b11000110,
		0b11111110,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11111111,
		0b11111111,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_C =
{
	{
		0b00111110,
		0b01111110,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01111110,
		0b00111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_D =
{
	{
		0b11111110,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11111111,
		0b11111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_E =
{
	{
		0b01111110,
		0b01111110,
		0b01100000,
		0b01111100,
		0b01111100,
		0b01100000,
		0b01100000,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_F =
{
	{
		0b01111110,
		0b01111110,
		0b01100000,
		0b01111100,
		0b01111100,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01100000,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_G =
{
	{
		0b01111111,
		0b11111111,
		0b11000000,
		0b11000000,
		0b11001110,
		0b11001111,
		0b11000011,
		0b11111111,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_H =
{
	{
		0b11000011,
		0b11000011,
		0b11000011,
		0b11111111,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_I =
{
	{
		0b01111110,
		0b01111110,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_J =
{
	{
		0b01111110,
		0b01111110,
		0b00000110,
		0b00000110,
		0b00000110,
		0b00000110,
		0b01100110,
		0b01111110,
		0b00111100,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_K =
{
	{
		0b01100110,
		0b01100110,
		0b01100110,
		0b01111100,
		0b01111100,
		0b01111100,
		0b01100110,
		0b01100110,
		0b01100110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_L =
{
	{
		0b01100000,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01100000,
		0b01111110,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_M =
{
	{
		0b11111111,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_N =
{
	{
		0b11000011,
		0b11000011,
		0b11100011,
		0b11110011,
		0b11011011,
		0b11001111,
		0b11000111,
		0b11000011,
		0b11000011,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_O =
{
	{
		0b01111110,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11111111,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_P =
{
	{
		0b11111110,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11111111,
		0b11111110,
		0b11000000,
		0b11000000,
		0b11000000,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_Q =
{
	{
		0b01111110,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11011011,
		0b11111111,
		0b01111110,
		0b00001100,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_R =
{
	{
		0b11111110,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11111111,
		0b11111110,
		0b11000110,
		0b11000111,
		0b11000111,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_S =
{
	{
		0b01111111,
		0b11111111,
		0b11000000,
		0b11111110,
		0b11111111,
		0b01111111,
		0b00000011,
		0b11111111,
		0b11111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_T =
{
	{
		0b11111111,
		0b11111111,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_U =
{
	{
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11111111,
		0b01111110,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_V =
{
	{
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11100111,
		0b01111110,
		0b00111100,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_W =
{
	{
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11111111,
		0b11111111,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_X =
{
	{
		0b11000011,
		0b11000011,
		0b01100110,
		0b00111100,
		0b00011000,
		0b00111100,
		0b01100110,
		0b11000011,
		0b11000011,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_Y =
{
	{
		0b11000011,
		0b11000011,
		0b11000011,
		0b11111111,
		0b01111111,
		0b00000011,
		0b00000011,
		0b11111111,
		0b11111111,
		0b00000000,
        
	},
	CHAR_ADVANCE
};
const Glyphe c_Z = 
{
	{
		0b11111111,
		0b11111111,
		0b00000110,
		0b00001100,
		0b00011000,
		0b00110000,
		0b01100000,
		0b11111111,
		0b11111111,
		0b00000000,
        
	},
	CHAR_ADVANCE
};

/* ********************************************************************** */

const Glyphe c_NULL =
{
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
		0b00000000,
        
	},
	CHAR_ADVANCE
};



inline const Glyphe* getChar(const char c)
{
    switch (c)
    {
        case '0':
            return &c_0;
            break;
            
        case '1':
            return &c_1;
            break;
            
        case '2':
            return &c_2;
            break;
            
        case '3':
            return &c_3;
            break;
            
        case '4':
            return &c_4;
            break;
        case '5':
            return &c_5;
            break;
        case '6':
            return &c_6;
            break;
        case '7':
            return &c_7;
            break;
        case '8':
            return &c_8;
            break;
        case '9':
            return &c_9;
            break;
            
        //
            
        case 'a':
            return &c_A;
            break;
            
        case 'A':
            return &c_A;
            break;
            
        case 'b':
            return &c_B;
            break;
            
        case 'B':
            return &c_B;
            break;
            
        case 'c':
            return &c_C;
            break;
            
        case 'C':
            return &c_C;
            break;
            
        case 'd':
            return &c_D;
            break;
            
        case 'D':
            return &c_D;
            break;
            
        case 'e':
            return &c_E;
            break;
            
        case 'E':
            return &c_E;
            break;
            
        case 'f':
            return &c_F;
            break;
            
        case 'F':
            return &c_F;
            break;
            
        case 'g':
            return &c_G;
            break;
            
        case 'G':
            return &c_G;
            break;
            
        case 'h':
            return &c_H;
            break;
            
        case 'H':
            return &c_H;
            break;
            
        case 'i':
            return &c_I;
            break;
            
        case 'I':
            return &c_I;
            break;

        case 'j':
            return &c_J;
            break;
            
        case 'J':
            return &c_J;
            break;
            
        case 'k':
            return &c_K;
            break;
        case 'K':
            return &c_K;
            break;
            
            
        case 'l':
            return &c_L;
            break;
            
        case 'L':
            return &c_L;
            break;

        case 'm':
            return &c_M;
            break;
            
        case 'M':
            return &c_M;
            break;
            
        case 'n':
            return &c_N;
            break;
            
        case 'N':
            return &c_N;
            break;

        case 'o':
            return &c_O;
            break;
            

            
        case 'O':
            return &c_O;
            break;
            
        case 'p':
            return &c_P;
            break;
            
        case 'P':
            return &c_P;
            break;
            
        case 'q':
            return &c_Q;
            break;
        case 'Q':
            return &c_Q;
            break;

        case 'r':
            return &c_R;
            break;
            
        case 'R':
            return &c_R;
            break;
            
        case 's':
            return &c_S;
            break;
            
        case 'S':
            return &c_S;
            break;
            
        case 't':
            return &c_T;
            break;
            
        case 'T':
            return &c_T;
            break;
            
        case 'u':
            return &c_U;
            break;
            
        case 'U':
            return &c_U;
            break;
            
        case 'v':
            return &c_V;
            break;
        case 'V':
            return &c_V;
            break;
            
        case 'w':
            return &c_W;
            break;
            
        case 'W':
            return &c_W;
            break;
            
        case 'x':
            return &c_X;
            break;
        case 'X':
            return &c_X;
            break;
            
        case 'y':
            return &c_Y;
            break;
        case 'Y':
            return &c_Y;
            break;
            
        case 'z':
            return &c_Z;
            break;
            
        case 'Z':
            return &c_Z;
            break;

            

            
        default:
            return &c_NULL;
            break;
    }
}




#endif

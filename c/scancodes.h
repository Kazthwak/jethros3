//COMPLETE LIST OF EVERY KEY, AND IT'S UNIQUE ID BASED ON ITS POSITION ON A UK KEYBOARD. TO SWAP OUT FR ANOTHER LAYOUT, A DIFFERENT TRANSLATION LAYER WOULD HAVE TO BE USED BOTH BEFORE AND AFTER THIS.'
//	SEE KEYBOARD IMAGES AND SPREADSHEET IN DESIGN FOR MORE INFO

#define KEY_ESC 		0b00000000
#define KEY_TILDE 		0b00000001
#define KEY_TAB 		0b00000010
#define KEY_CAPS 		0b00000011
#define KEY_SHIFT_L 	0b00000100
#define KEY_CTRL_L 		0b00000101

#define KEY_1 			0b00001001
#define KEY_q 			0b00001010
#define KEY_a 			0b00001011
#define KEY_BACKSLASH 	0b00001100
#define KEY_SUPER_L 	0b00001101

#define KEY_F1 			0b00010000
#define KEY_2 			0b00010001
#define KEY_w 			0b00010010
#define KEY_s 			0b00010011
#define KEY_z 			0b00010100

#define KEY_F2 			0b00011000
#define KEY_3 			0b00011001
#define KEY_e 			0b00011010
#define KEY_d 			0b00011011
#define KEY_x 			0b00011100
#define KEY_SPACE 		0b00011101

#define KEY_F3 			0b00100000
#define KEY_4 			0b00100001
#define KEY_r 			0b00100010
#define KEY_f 			0b00100011
#define KEY_c 			0b00100100

#define KEY_F4 			0b00101000
#define KEY_5 			0b00101001
#define KEY_t 			0b00101010
#define KEY_g 			0b00101011
#define KEY_v 			0b00101100

#define KEY_6 			0b00110001
#define KEY_y 			0b00110010
#define KEY_h 			0b00110011
#define KEY_b 			0b00110100

#define KEY_F5 			0b00111000
#define KEY_7 			0b00111001
#define KEY_u 			0b00111010
#define KEY_j 			0b00111011
#define KEY_n 			0b00111100

#define KEY_F6 			0b01000000
#define KEY_8 			0b01000001
#define KEY_i 			0b01000010
#define KEY_k 			0b01000011
#define KEY_m 			0b01000100

#define KEY_F7 			0b01001000
#define KEY_9 			0b01001001
#define KEY_o 			0b01001010
#define KEY_l 			0b01001011
#define KEY_COMMA 		0b01001100

#define KEY_F8 			0b01010000
#define KEY_0 			0b01010001
#define KEY_p 			0b01010010
#define KEY_SEMICOLON 	0b01010011
#define KEY_FULL_STOP 	0b01010100
#define KEY_ALT_GR 		0b01010101

#define KEY_F9 			0b01011000
#define KEY_DASH 		0b01011001
#define KEY_SQR_BRCK_L 	0b01011010
#define KEY_APOSTROPHE 	0b01011011
#define KEY_FWD_SLASH 	0b01011100
#define KEY_FN 			0b01011101

#define KEY_F10 		0b01100000
#define KEY_EQUALS 		0b01100001
#define KEY_SQR_BRCK_R 	0b01100010
#define KEY_HASH 		0b01100011
#define KEY_CLICK_R 	0b01100100

#define KEY_F11 		0b01101000

#define KEY_F12 		0b01110000
#define KEY_BACKSPACE 	0b01110001
#define KEY_ENTER 		0b01110010
//Bottom of enter Key 	0b01110011
#define KEY_SHIFT_R 	0b01110100
#define KEY_CTRL_R 		0b01110101

#define KEY_PRNT_SCRN 	0b01111000
#define KEY_INS 		0b01111001
#define KEY_DEL 		0b01111010
#define KEY_LEFT_ARROW 	0b01111101

#define KEY_SCRL_LCK 	0b10000000
#define KEY_HOME 		0b10000001
#define KEY_END 		0b10000010
#define KEY_UP_ARROW	0b10000100
#define KEY_DOWN_ARROW 	0b10000101

#define KEY_PAUSE 		0b10001000
#define KEY_PGUP 		0b10001001
#define KEY_PGDWN 		0b10001010
#define KEY_RIGHT_ARROW	0b10001101

#define KEY_NUMLCK 		0b10010001
#define KEY_KP7 		0b10010010
#define KEY_KP4 		0b10010011
#define KEY_KP1 		0b10010100
#define KEY_KP0 		0b10010101

#define KEY_KP_DIV 		0b10011001
#define KEY_KP8 		0b10011010
#define KEY_KP5 		0b10011011
#define KEY_KP2 		0b10011100

#define KEY_KP_MUL 		0b10100001
#define KEY_KP9 		0b10100010
#define KEY_KP6 		0b10100011
#define KEY_KP3 		0b10100100
#define KEY_KP_DEL 		0b10100101

#define KEY_KP_SUB 		0b10101001
#define KEY_KP_ADD 		0b10101010
#define KEY_KP_ENTER 	0b10101100




//A VERY :ARGE TABLE THAT CAN BE USED TO TRANSLATE SET 3 SCANCODES INTO UNIQUE IDs

uint8_t scancode_lookup_table[] = {
	0,	//00 nothing
	0,	//01 nothing
	0,	//02 nothing
	0,	//03 nothing
	0,	//04 nothing
	0,	//05 nothing
	0,	//06 nothing
	0,	//07 F1
	0,	//08 ESC
	0,	//09 nothing
	0,	//0A nothing
	0,	//0B nothing
	0,	//0C nothing
	0,	//0D TAB
	0,	//0E `
	0,	//0F F2

	0,	//10 nothing
	0,	//11 L CTRL
	0,	//12 L SHIFT
	0,	//13 nothing
	0,	//14 CAPSLOCK
	0,	//15 q
	0,	//16 1
	0,	//17 F3
	0,	//18 nothing
	0,	//19 L ALT
	0,	//1A z
	0,	//1B s
	0,	//1C a
	0,	//1D w
	0,	//1E 2
	0,	//1F F4

	0,	//20 nothing
	0,	//21 c
	0,	//22 x
	0,	//23 d
	0,	//24 e
	0,	//25 4
	0,	//26 3
	0,	//27 F5
	0,	//28 nothing
	0,	//29 SPACE
	0,	//2A v
	0,	//2B f
	0,	//2C t
	0,	//2D r
	0,	//2E 5
	0,	//2F F6

	0,	//30 nothing
	0,	//31 n
	0,	//32 b
	0,	//33 h
	0,	//34 g
	0,	//35 y
	0,	//36 6
	0,	//37 F7
	0,	//38 nothing
	0,	//39 R ALT
	0,	//3A m
	0,	//3B j
	0,	//3C u
	0,	//3D 7
	0,	//3E 8
	0,	//3F F8

	0,	//40 nothing
	0,	//41 ,
	0,	//42 k
	0,	//43 i
	0,	//44 q
	0,	//45 0
	0,	//46 9
	0,	//47 F9
	0,	//48 nothing (says used in I release, nut I think it's a typo)
	0,	//49 .
	0,	//4A /
	0,	//4B l
	0,	//4C ;
	0,	//4D p
	0,	//4E -
	0,	//4F F10

	0,	//50 nothing
	0,	//51 nothing
	0,	//52 '
	0,	//53 nothing
	0,	//54 [
	0,	//55 =
	0,	//56 F11
	0,	//57 PRNT SCRN
	0,	//58 R CTRL
	0,	//59 R SHFT
	0,	//5A ENTER
	0,	//5B ]
	0,	//5C \
	0,	//5D nothing
	0,	//5E F12
	0,	//5F SCROLL LOCK

	0,	//60 DOWN ARW
	0,	//61 LEFT ARW
	0,	//62 PAUSE
	0,	//63 UP ARROW
	0,	//64 DELETE
	0,	//65 END
	0,	//66 BCKSPCE
	0,	//67 INSERT
	0,	//68 nothing
	0,	//69 KP1
	0,	//6A RGHT ARW
	0,	//6B KP 4
	0,	//6C KP 7
	0,	//6D PAGE DWN
	0,	//6E HOME
	0,	//6F PAGE UP

	0,	//70 KP 0
	0,	//71 KP .
	0,	//72 KP 2
	0,	//73 KP 5
	0,	//74 KP 6
	0,	//75 KP 8
	0,	//76 NUM LOCK
	0,	//77 nothing
	0,	//78 nothing
	0,	//79 KP END
	0,	//7A KP 3
	0,	//7B nothing
	0,	//7C KP +
	0,	//7D KP 9
	0,	//7E KP *
	0,	//7F nothing

	0,	//80 nothing
	0,	//81 nothing
	0,	//82 nothing
	0,	//83 nothing
	0,	//84 nothing
	0,	//85 nothing
	0,	//86 nothing
	0,	//87 nothing
	0,	//88 nothing
	0,	//89 nothing
	0,	//8A nothing
	0,	//8B L WIN
	0,	//8C R WIN
	0,	//8D APPS
	0,	//8E nothing
	0 	//8F nothing
/*
	0,	//00 
	0,	//01 
	0,	//02 
	0,	//03 
	0,	//04 
	0,	//05 
	0,	//06 
	0,	//07 
	0,	//08 
	0,	//09 
	0,	//0A 
	0,	//0B 
	0,	//0C 
	0,	//0D 
	0,	//0E 
	0,	//0F 
*/
};

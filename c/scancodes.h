//COMPLETE LIST OF EVERY KEY, AND IT'S UNIQUE ID BASED ON ITS POSITION ON A UK KEYBOARD. TO SWAP OUT FOR ANOTHER LAYOUT, A DIFFERENT TRANSLATION LAYER WOULD HAVE TO BE USED BOTH BEFORE AND AFTER THIS.
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
#define KEY_ALT			0b00010101

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
//22 gaps
//from line 4 to 130
//therfore 127 lines -22 gaps = 105 chars

#define KEY_INVALID		0b11111111



//A VERY :ARGE TABLE THAT CAN BE USED TO TRANSLATE SET 3 SCANCODES INTO UNIQUE IDs

char ascii_id_lookup_table[] = {
	0,  //escape
	'`',  //tilde
	0,  //tab
	0,  //caps
	0,  //shift
	0,  //ctrl
	0,  //nothing
	0,  //nothing

	0,  //nothing
	'1',  //1
	'q',  //q
	'a',  //a
	'\\',  //\   asd 
	0,  //super
	0,  //nothing
	0,  //nothing

	0,  //F1
	'2',  //2
	'w',  //w
	's',  //s
	'z',  //z
	0,  //alt
	0,  //nothing
	0,  //nothing

	0,  //F2
	'3',  //3
	'e',  //e
	'd',  //d
	'x',  //x
	' ',  //space
	0,  //nothing
	0,  //nothing

	0,  //F3
	'4',  //4
	'r',  //r
	'f',  //f
	'c',  //c
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //F4
	'5',  //5
	't',  //t
	'g',  //g
	'v',  //v
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //nothing
	'6',  //6
	'y',  //y
	'h',  //h
	'b',  //b
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //f5
	'7',  //7
	'u',  //u
	'j',  //j
	'n',  //n
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //f6
	'8',  //8
	'i',  //i
	'k',  //k
	'm',  //m
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //f7
	'9',  //9
	'O',  //o
	'l',  //l
	',',  //,
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //f8
	'0',  //0
	'p',  //p
	';',  //;
	'.',  //.
	0,  //alt
	0,  //nothing
	0,  //nothing
	
	0,  //f9
	'-',  //-
	'[',  //[
	'\'',  //'
	0x2F,  ///
	0,  //FN
	0,  //nothing
	0,  //nothing

	0,  //f10
	'=',  //=
	']',  //]
	'#',  //#
	0,  //Right Click
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //f11
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //f12
	0x88,  //backspace
	'\n' | 0x80,  //enter
	0,  //nothing
	0,  //RIght Shift
	0,  //Right Ctrl
	0,  //nothing
	0,  //nothing
	
	0,  //Print Screen
	0,  //insert
	0,  //delete
	0x84,  //left_arrow
	0,  //Nothing
	0,  //Nothing
	0,  //Nothing
	0,  //Nothing

	0,  //Sroll lock
	0,  //Home
	0,  //End
	0x81,  //Up arrow
	0x83,  //Down Arrow
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //Pause
	0,  //Page up
	0,  //Page down
	0x82,  //Right arrow
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	//numlock stuff
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing

	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
	0,  //nothing
};

uint8_t scancode_lookup_table[] = {
	KEY_INVALID,	//00 nothing
	KEY_INVALID,	//01 nothing
	KEY_INVALID,	//02 nothing
	KEY_INVALID,	//03 nothing
	KEY_INVALID,	//04 nothing
	KEY_INVALID,	//05 nothing
	KEY_INVALID,	//06 nothing
	KEY_F1,	//07 F1
	KEY_ESC,	//08 ESC
	KEY_INVALID,	//09 nothing
	KEY_INVALID,	//0A nothing
	KEY_INVALID,	//0B nothing
	KEY_INVALID,	//0C nothing
	KEY_TAB,	//0D TAB
	KEY_TILDE,	//0E `
	KEY_F2,	//0F F2

	KEY_INVALID,	//10 nothing
	KEY_CTRL_L,	//11 L CTRL
	KEY_SHIFT_L,	//12 L SHIFT
	KEY_INVALID,	//13 nothing
	KEY_CAPS,	//14 CAPSLOCK
	KEY_q,	//15 q
	KEY_1,	//16 1
	KEY_F3,	//17 F3
	KEY_INVALID,	//18 nothing
	KEY_ALT,	//19 L ALT
	KEY_z,	//1A z
	KEY_s,	//1B s
	KEY_a,	//1C a
	KEY_w,	//1D w
	KEY_2,	//1E 2
	KEY_F4,	//1F F4

	KEY_INVALID,	//20 nothing
	KEY_c,	//21 c
	KEY_x,	//22 x
	KEY_d,	//23 d
	KEY_e,	//24 e
	KEY_4,	//25 4
	KEY_3,	//26 3
	KEY_F5,	//27 F5
	KEY_INVALID,	//28 nothing
	KEY_SPACE,	//29 SPACE
	KEY_v,	//2A v
	KEY_f,	//2B f
	KEY_t,	//2C t
	KEY_r,	//2D r
	KEY_5,	//2E 5
	KEY_F6,	//2F F6

	KEY_INVALID,	//30 nothing
	KEY_n,	//31 n
	KEY_b,	//32 b
	KEY_h,	//33 h
	KEY_g,	//34 g
	KEY_y,	//35 y
	KEY_6,	//36 6
	KEY_F7,	//37 F7
	KEY_INVALID,	//38 nothing
	KEY_ALT_GR,	//39 R ALT
	KEY_m,	//3A m
	KEY_j,	//3B j
	KEY_u,	//3C u
	KEY_7,	//3D 7
	KEY_8,	//3E 8
	KEY_F8,	//3F F8

	KEY_INVALID,	//40 nothing
	KEY_COMMA,	//41 ,
	KEY_k,	//42 k
	KEY_i,	//43 i
	KEY_o,	//44 o
	KEY_0,	//45 0
	KEY_9,	//46 9
	KEY_F9,	//47 F9
	KEY_INVALID,	//48 nothing (says used in i key release, but I think it's a typo)
	KEY_FULL_STOP,	//49 .
	KEY_FWD_SLASH,	//4A forward_slash
	KEY_l,	//4B l
	KEY_SEMICOLON,	//4C ;
	KEY_p,	//4D p
	KEY_DASH,	//4E -
	KEY_F10,	//4F F10

	KEY_INVALID,	//50 nothing
	KEY_INVALID,	//51 nothing
	KEY_APOSTROPHE,	//52 '
	KEY_INVALID,	//53 nothing
	KEY_SQR_BRCK_L,	//54 [
	KEY_EQUALS,	//55 =
	KEY_F11,	//56 F11
	KEY_PRNT_SCRN,	//57 PRNT SCRN
	KEY_CTRL_R,	//58 R CTRL
	KEY_SHIFT_R,	//59 R SHFT
	KEY_ENTER,	//5A ENTER
	KEY_SQR_BRCK_R,	//5B ]
	KEY_BACKSLASH,	//5C backslash
	KEY_INVALID,	//5D nothing
	KEY_F12,	//5E F12
	KEY_SCRL_LCK,	//5F SCROLL LOCK

	KEY_DOWN_ARROW,	//60 DOWN ARW
	KEY_LEFT_ARROW,	//61 LEFT ARW
	KEY_PAUSE,	//62 PAUSE
	KEY_UP_ARROW,	//63 UP ARROW
	KEY_DEL,	//64 DELETE
	KEY_END,	//65 END
	KEY_BACKSPACE,	//66 BCKSPCE
	KEY_INS,	//67 INSERT
	KEY_INVALID,	//68 nothing
	KEY_KP1,	//69 KP1
	KEY_RIGHT_ARROW,	//6A RGHT ARW
	KEY_KP4,	//6B KP 4
	KEY_KP7,	//6C KP 7
	KEY_PGDWN,	//6D PAGE DWN
	KEY_HOME,	//6E HOME
	KEY_PGUP,	//6F PAGE UP

	KEY_KP0,	//70 KP 0
	KEY_KP_DEL,	//71 KP DEL
	KEY_KP2,	//72 KP 2
	KEY_KP5,	//73 KP 5
	KEY_KP6,	//74 KP 6
	KEY_KP8,	//75 KP 8
	KEY_NUMLCK,	//76 NUM LOCK
	KEY_INVALID,	//77 nothing
	KEY_INVALID,	//78 nothing
	KEY_INVALID,	//79 KP END (WHAT ON EARTH IS THIS KEY??????????)
	KEY_KP3,	//7A KP 3
	KEY_INVALID,	//7B nothing
	KEY_KP_ADD,	//7C KP +
	KEY_KP9,	//7D KP 9
	KEY_KP_MUL,	//7E KP *
	KEY_INVALID,	//7F nothing

	KEY_INVALID,	//80 nothing
	KEY_INVALID,	//81 nothing
	KEY_INVALID,	//82 nothing
	KEY_INVALID,	//83 nothing
	KEY_INVALID,	//84 nothing
	KEY_INVALID,	//85 nothing
	KEY_INVALID,	//86 nothing
	KEY_INVALID,	//87 nothing
	KEY_INVALID,	//88 nothing
	KEY_INVALID,	//89 nothing
	KEY_INVALID,	//8A nothing
	KEY_SUPER_L,	//8B L WIN
	KEY_INVALID,	//8C R WIN
	KEY_INVALID,	//8D APPS
	KEY_INVALID,	//8E nothing
	0 	//8F nothing
/*
	KEY_INVALID,	//00
	KEY_INVALID,	//01
	KEY_INVALID,	//02
	KEY_INVALID,	//03
	KEY_INVALID,	//04
	KEY_INVALID,	//05
	KEY_INVALID,	//06
	KEY_INVALID,	//07
	KEY_INVALID,	//08
	KEY_INVALID,	//09
	KEY_INVALID,	//0A
	KEY_INVALID,	//0B
	KEY_INVALID,	//0C
	KEY_INVALID,	//0D
	KEY_INVALID,	//0E
	KEY_INVALID,	//0F
*/
};

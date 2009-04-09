#ifndef _7_SEGMENT_INCLUDED_H_
#define _7_SEGMENT_INCLUDED_H_ 1

#define DIGIT_OFF     (0x00) /* 00000000 */
#define DIGIT_MINUS   (0x40) /* 01000000 */

#define DIGIT_0 (0x3f) /* 00111111 */
#define DIGIT_1 (0x06) /* 00000110 */
#define DIGIT_2 (0x5b) /* 01011011 */
#define DIGIT_3 (0x4f) /* 01001111 */
#define DIGIT_4 (0x66) /* 01100110 */
#define DIGIT_5 (0x6d) /* 01101101 */
#define DIGIT_6 (0x7d) /* 01111101 */
#define DIGIT_7 (0x07) /* 00000111 */
#define DIGIT_8 (0x7f) /* 01111111 */
#define DIGIT_9 (0x6f) /* 01101111 */
#define DIGIT_A (0x77) /* 01110111 */
#define DIGIT_B (0x7c) /* 01111100 */
#define DIGIT_C (0x58) /* 01011000 */
#define DIGIT_D (0x5e) /* 01011110 */
#define DIGIT_E (0x79) /* 01111001 */
#define DIGIT_F (0x71) /* 01110001 */

/* Extra Alphabet Letters */
#define DIGIT_G (0x3d) /* 00111101 */
#define DIGIT_H (0x74) /* 01110100 */
/* 1 and I are not visually distinct. */
#define DIGIT_I (0x06) /* 00000110 */
#define DIGIT_J (0x1e) /* 00011110 */
/* K is a mirrored 4: |-` It's a bit odd. Try not to use it much. */
#define DIGIT_K (0x72) /* 01110010 */
#define DIGIT_L (0x38) /* 00111000 */
/* M is also a bit esoteric. Draw an A with no center bar. */
#define DIGIT_M (0x37) /* 00110111 */
#define DIGIT_N (0x54) /* 01010100 */
#define DIGIT_O (0x5c) /* 01011100 */
#define DIGIT_P (0x73) /* 01110011 */
/* Q is an upside-down G. Probably hard to read. */
#define DIGIT_Q (0x3b) /* 00111011 */
#define DIGIT_R (0x50) /* 01010000 */
/* Note: S and 5 are not visually distinct. */
#define DIGIT_S (0x6d) /* 01101101 */
#define DIGIT_T (0x78) /* 01111000 */
#define DIGIT_U (0x1c) /* 00011100 */
/* U and V are not visually distinct. */
#define DIGIT_V (0x1c) /* 00011100 */
/* W is an 8 with the top turned off. It's two u's stacked. Get it? */
#define DIGIT_W (0x7e) /* 01111110 */
/* X is a bit esoteric. Draw a blackslash `-, */
#define DIGIT_X (0x64) /* 01100100 */
#define DIGIT_Y (0x6e) /* 01101110 */
/* Z and 2 are not visually distinct. */
#define DIGIT_Z (0x5b) /* 01011011 */


#endif 


vals = <<EOS
#define DIGIT_0 (0xYY) /* 11111100 */
#define DIGIT_1 (0xYY) /* 01100000 */
#define DIGIT_2 (0xYY) /* 11011010 */
#define DIGIT_3 (0xYY) /* 11110010 */
#define DIGIT_4 (0xYY) /* 01100110 */
#define DIGIT_5 (0xYY) /* 10110110 */
#define DIGIT_6 (0xYY) /* 10111110 */
#define DIGIT_7 (0xYY) /* 11100000 */
#define DIGIT_8 (0xYY) /* 11111110 */
#define DIGIT_9 (0xYY) /* 11110110 */
#define DIGIT_A (0xYY) /* 11101110 */
#define DIGIT_B (0xYY) /* 00111110 */
#define DIGIT_C (0xYY) /* 00011010 */
#define DIGIT_D (0xYY) /* 01111010 */
#define DIGIT_E (0xYY) /* 10011110 */
#define DIGIT_F (0xYY) /* 10001110 */

/* Extra Alphabet Letters */
#define DIGIT_G (0xYY) /* 10111100 */
#define DIGIT_H (0xYY) /* 00101110 */
/* 1 and I are not visually distinct. */
#define DIGIT_I (0xYY) /* 01100000 */
#define DIGIT_J (0xYY) /* 01111000 */
/* K is a mirrored 4: |-` It's a bit odd. Try not to use it much. */
#define DIGIT_K (0xYY) /* 01001110 */
#define DIGIT_L (0xYY) /* 00011100 */
/* M is also a bit esoteric. Draw an A with no center bar. */
#define DIGIT_M (0xYY) /* 11101100 */
#define DIGIT_N (0xYY) /* 00101010 */
#define DIGIT_O (0xYY) /* 00111010 */
#define DIGIT_P (0xYY) /* 11001110 */
/* Q is an upside-down G. Probably hard to read. */
#define DIGIT_Q (0xYY) /* 11011100 */
#define DIGIT_R (0xYY) /* 00001010 */
/* Note: S and 5 are not visually distinct. */
#define DIGIT_S (0xYY) /* 10110110 */
#define DIGIT_T (0xYY) /* 00011110 */
#define DIGIT_U (0xYY) /* 00111000 */
/* U and V are not visually distinct. */
#define DIGIT_V (0xYY) /* 00111000 */
/* W is an 8 with the top turned off. It's two u's stacked. Get it? */
#define DIGIT_W (0xYY) /* 01111110 */
/* X is a bit esoteric. Draw a blackslash `-, */
#define DIGIT_X (0xYY) /* 00100110 */
#define DIGIT_Y (0xYY) /* 01110110 */
/* Z and 2 are not visually distinct. */
#define DIGIT_Z (0xYY) /* 11011010 */
EOS


vals.each_line do |line|
  if line.split.size > 4 && line.split()[4] =~ /[0-9]{8}/
    rbin = (line.split())[4]
    bin = rbin.reverse
    hex = "%02x" % bin.to_i(2)
    puts line.gsub(/YY/, hex).gsub(/#{rbin}/, bin)
  else
    puts line
  end
end

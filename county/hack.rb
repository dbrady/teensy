
vals = <<EOS
0xYY, // 0: 00000000
0xYY, // 1: 01100000
0xYY, // 2: 11011010
0xYY, // 3: 11110010
0xYY, // 4: 01100110
0xYY, // 5: 10110110
0xYY, // 6: 10111110
0xYY, // 7: 11100000
0xYY, // 8: 11111110
0xYY, // 9: 11110110
0xYY, // A: 11101110
0xYY, // b: 00111110
0xYY, // c: 00011010
0xYY, // d: 01111010
0xYY, // E: 10011110
0xYY, // F: 10001110
EOS


vals.each_line do |line|
  bin = (line.split())[3]
  hex = "%02x" % bin.to_i(2)
  puts line.gsub(/YY/, hex)
end

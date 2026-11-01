# C - Huffman Encoding

```bash
[debug] compress: input='./input.txt', output='./output.txt'
[debug] 11043640 bytes gelezen
[debug] frequentietabel:
[debug]   '\n'  freq=1440
[debug]   ' '   freq=10623960
[debug]   ','   freq=6240
[debug]   '.'   freq=9680
[debug]   'A'   freq=480
[debug]   'C'   freq=880
[debug]   'D'   freq=1040
[debug]   'E'   freq=80
[debug]   'F'   freq=400
[debug]   'I'   freq=560
[debug]   'L'   freq=160
[debug]   'M'   freq=640
[debug]   'N'   freq=1680
[debug]   'O'   freq=80
[debug]   'P'   freq=480
[debug]   'Q'   freq=240
[debug]   'S'   freq=1680
[debug]   'U'   freq=240
[debug]   'V'   freq=1040
[debug]   'a'   freq=32560
[debug]   'b'   freq=5280
[debug]   'c'   freq=15600
[debug]   'd'   freq=12000
[debug]   'e'   freq=47360
[debug]   'f'   freq=2960
[debug]   'g'   freq=5600
[debug]   'h'   freq=2320
[debug]   'i'   freq=40640
[debug]   'j'   freq=80
[debug]   'l'   freq=23520
[debug]   'm'   freq=19440
[debug]   'n'   freq=24320
[debug]   'o'   freq=16480
[debug]   'p'   freq=9120
[debug]   'q'   freq=4880
[debug]   'r'   freq=21200
[debug]   's'   freq=33040
[debug]   't'   freq=33680
[debug]   'u'   freq=35840
[debug]   'v'   freq=5920
[debug]   'x'   freq=800
[debug] build_tree: 41 unieke symbolen in heap
[debug] build_tree: boom klaar, totale freq (root) = 11043640
[debug] Huffman-codes:
[debug]   '\n'  len=9 code=010000001
[debug]   ' '   len=1 code=1
[debug]   ','   len=7 code=0101000
[debug]   '.'   len=6 code=000010
[debug]   'A'   len=11 code=01010011100
[debug]   'C'   len=10 code=0101001001
[debug]   'D'   len=10 code=0111011001
[debug]   'E'   len=13 code=0100000001000
[debug]   'F'   len=11 code=01000000011
[debug]   'I'   len=11 code=01010011111
[debug]   'L'   len=13 code=0101001110111
[debug]   'M'   len=10 code=0100000000
[debug]   'N'   len=9 code=010100110
[debug]   'O'   len=13 code=0100000001001
[debug]   'P'   len=11 code=01010011110
[debug]   'Q'   len=12 code=010000000101
[debug]   'S'   len=9 code=010100101
[debug]   'U'   len=12 code=010100111010
[debug]   'V'   len=10 code=0111011000
[debug]   'a'   len=5 code=01011
[debug]   'b'   len=7 code=0000110
[debug]   'c'   len=6 code=010101
[debug]   'd'   len=6 code=010001
[debug]   'e'   len=4 code=0011
[debug]   'f'   len=8 code=01000001
[debug]   'g'   len=7 code=0000111
[debug]   'h'   len=9 code=011101101
[debug]   'i'   len=4 code=0001
[debug]   'j'   len=13 code=0101001110110
[debug]   'l'   len=5 code=00101
[debug]   'm'   len=5 code=00000
[debug]   'n'   len=5 code=01001
[debug]   'o'   len=6 code=011100
[debug]   'p'   len=7 code=0111010
[debug]   'q'   len=8 code=01110111
[debug]   'r'   len=5 code=00100
[debug]   's'   len=5 code=01100
[debug]   't'   len=5 code=01101
[debug]   'u'   len=5 code=01111
[debug]   'v'   len=7 code=0100001
[debug]   'x'   len=10 code=0101001000
[debug] 12844520 bits geschreven (1605565 payload-bytes)
[debug] header: 2048 bytes, totaal output: 1607613 bytes
[debug] ratio (payload/input): 14.54%, gem. 1.16 bits/symbool
```

Bitboard used is the Little endian file-rank (LEFR) mapping
ref - https://www.chessprogramming.org/Square_Mapping_Considerations

Bitboard cells:

![LEFR mapping preview of a unsigned 64-bit bitboard](https://www.chessprogramming.org/images/2/27/Lefr.JPG)




Shift Directions:
```c
 noWe         nort         noEa
          -7    +1    +9
              \  |  /
  west    -8 <-  0 -> +8    east
              /  |  \
          -9    -1    +7
  soWe         sout         soEa
```
 

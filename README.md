Algoritmo de compresión de Huffman
----------------------------------

Implementación del algoritmo de compresión Huffman.

Código fuente original en C++:
http://articulos.conclase.net/?tema=algoritmos&art=huffman&pag=000

En esta versión el código fue portado a C (y encodeado a UTF-8 en vez del viejo
encoding ASCII _ISO-8859-1_).

También se agrega script de **CMake** para compilar ambos comandos `compres` y `decompres` con:

    $ cmake .
    $ make

Al tener CMake configurado pueder ser compilado en cualquier plataforma (no solo _*nix_),
y directamente importado en CLion. 

Los comandos también pueden ser compilados solo con un compilador C como _gcc_:

    $ cc compres.c -o compres
    $ cc decompres.c -o decompres

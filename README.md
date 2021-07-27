Algoritmo de compresión de Huffman
----------------------------------

Compresión de archivos usando el Algoritmo de Huffman,
código fuente original de Salvador Pozo Coronado (2000),
con mínimas correcciones de Mariano Ruiz (2018-2021).

> ℹ Branch`modo-debug`: esta version agrega unas
> funciones que imprimen en el error stream (consola)
> la tabla de frecuencias, la codificación
> Huffman usada, junto con una representación en
> árbol binario (ver [Ejecución](#ejecución)).


Código fuente original en C++:
[link original (roto)](http://articulos.conclase.net/?tema=algoritmos&art=huffman&pag=000),
[web archive](https://web.archive.org/web/20190912232030/http://articulos.conclase.net/?tema=algoritmos&art=huffman&pag=000).

### Cambios en esta versión:

- Código fue portado a C: mínimos cambios ya que el código original
  en C++ no hacía uso de programación orientada a objetos, pero hacía
  uso de referencias que no son compatibles con C.
- Fix error en cómo se leía el archivo que causaba que no
  se pudiera usar para comprimir archivos binarios o con más
  de 128 caracteres diferentes.
- Archivos fuentes re-encodeados a UTF-8 en vez del viejo
  encoding ASCII _ISO-8859-1_, que hacía in-leíble los comentarios
  con caracteres diacríticos en cualquier editor moderno.
- También se agrega script para compilar con **CMake** ambos
  comandos `compres` y `decompres`:

      $ cmake . && make

Al tener CMake configurado puede ser compilado en cualquier
plataforma (no solo _*nix_), y directamente importado en **CLion**.
Los comandos también pueden ser instalados en `/usr/local/bin`
una vez ejecutado `cmake .` con `sudo make install`.

De todos modos los comandos también pueden ser compilados
solo con un compilador C como **gcc**:

    $ cc compres.c -o compres
    $ cc decompres.c -o decompres

### Ejecución

Una vez generados los ejecutables, para comprimir un archivo
y ver la tabla de frecuencias y árbol Huffman utilizado:

    $ ./compres hello.txt hello.txt.compres 
    Tabla Frecuencias
    ------------------------------------
    Symb.: 'w' 77   Freq.: 1    Pos.:  0
    Symb.: 'r' 72   Freq.: 1    Pos.:  1
    Symb.: 'h' 68   Freq.: 1    Pos.:  2
    Symb.: 'e' 65   Freq.: 1    Pos.:  3
    Symb.: 'd' 64   Freq.: 1    Pos.:  4
    Symb.: '.' 0A   Freq.: 1    Pos.:  5
    Symb.: 'o' 6F   Freq.: 2    Pos.:  6
    Symb.: '!' 21   Freq.: 2    Pos.:  7
    Symb.: 'l' 6C   Freq.: 3    Pos.:  8
    Symb.: ' ' 20   Freq.: 3    Pos.:  9

    > Núm símbolos: 10, Tamaño archivo: 16 bytes

    Tabla Codificación
    ------------------------------------
    Symb.: '.' 0A    Bits:  4    Cod.: 1001
    Symb.: ' ' 20    Bits:  2    Cod.: 00
    Symb.: '!' 21    Bits:  3    Cod.: 110
    Symb.: 'd' 64    Bits:  4    Cod.: 1000
    Symb.: 'e' 65    Bits:  4    Cod.: 1011
    Symb.: 'h' 68    Bits:  4    Cod.: 1010
    Symb.: 'l' 6C    Bits:  3    Cod.: 111
    Symb.: 'o' 6F    Bits:  3    Cod.: 011
    Symb.: 'r' 72    Bits:  4    Cod.: 0101
    Symb.: 'w' 77    Bits:  4    Cod.: 0100

    Arbol binario
    ------------------------------------
    (16)
     `--(7)
     |   `--(3) ' ' [20]
     |   `--(4)
     |       `--(2)
     |       |   `--(1) 'w' [77]
     |       |   `--(1) 'r' [72]
     |       `--(2) 'o' [6F]
     `--(9)
         `--(4)
         |   `--(2)
         |   |   `--(1) 'd' [64]
         |   |   `--(1) '.' [0A]
         |   `--(2)
         |       `--(1) 'h' [68]
         |       `--(1) 'e' [65]
         `--(5)
             `--(2) '!' [21]
             `--(3) 'l' [6C]

El archivo comprimido `hello.txt.compres` puede ser descomprimido
luego con:

    $ ./decompres hello.txt.compres hello.txt
    $ cat hello.txt
    hello world ! !

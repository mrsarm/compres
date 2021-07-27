Algoritmo de compresión de Huffman
----------------------------------

Compresión de archivos usando el Algoritmo de Huffman,
código fuente original de Salvador Pozo Coronado (2000),
con mínimas correcciones de Mariano Ruiz (2018-2021).


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

Una vez generados los ejecutables, para comprimir un archivo:

    $ ./compres hello.txt hello.txt.compres

El archivo comprimido `hello.txt.compres` puede ser descomprimido
luego con:

    $ ./decompres hello.txt.compres hello.txt

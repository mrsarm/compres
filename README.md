Algoritmo de compresión de Huffman
----------------------------------

Compresión de archivos usando el Algoritmo de Huffman,
código fuente original de Salvador Pozo Coronado (2000),
con mínimas correcciones de Mariano Ruiz (2018).


Código fuente original en C++:
http://articulos.conclase.net/?tema=algoritmos&art=huffman&pag=000

### Cambios en esta versión:

- Código fue portado a C: mínimos cambios ya que el código original
  no hacía uso de programación orientada a objetos, pero hacía uso
  de referencias que no son compatibles con C.
- Fix error en cómo se leía el archivo que causaba que no
  se pudiera usar para comprimir archivos binarios o con más
  de 128 caracteres diferentes.
- Archivos re-encodeados a UTF-8 en vez del viejo
  encoding ASCII _ISO-8859-1_, que hacía in-leíble los comentarios
  con caracteres diacríticos en cualquier editor moderno.
- También se agrega script para compilar con **CMake** ambos
  comandos `compres` y `decompres`:

      $ cmake . && make

Al tener CMake configurado puede ser compilado en cualquier
plataforma (no solo _*nix_), y directamente importado en **CLion**. 

De todos modos los comandos también pueden ser compilados
solo con un compilador C como **gcc**:

    $ cc compres.c -o compres
    $ cc decompres.c -o decompres

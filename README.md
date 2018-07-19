Algoritmo de compresión de Huffman
----------------------------------

Implementación del algoritmo de compresión Huffman.

Código fuente original en C++:
http://articulos.conclase.net/?tema=algoritmos&art=huffman&pag=000

### Cambios en esta versión:

- Código fue portado a C: mínimos cambios ya que el código original
  no hacía uso de programación orientada a objetos, pero hacía uso
  de referencias que no son compatibles con C.
- Fix error en cómo se leía el archivo que causaba que no
  se pudiera usar para comprimir archivos binarios o con más
  de 128 diferentes caracteres.
- Re-encodeado a UTF-8 en vez del viejo encoding ASCII _ISO-8859-1_,
  que hacía inleíble los comentarios con caracteres diacríticos en
  cualquier editor moderno.
- También se agrega script de **CMake** para compilar ambos
  comandos `compres` y `decompres` con `cmake . & make`.

Al tener CMake configurado pueder ser compilado en cualquier
plataforma (no solo _*nix_), y directamente importado en CLion. 

Los comandos también pueden ser compilados solo con un
compilador C como _gcc_:

    $ cc compres.c -o compres
    $ cc decompres.c -o decompres

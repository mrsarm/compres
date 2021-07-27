/* Compresión de archivos usando el Algoritmo de Huffman: */
/* (C) Noviembre de 2000 Salvador Pozo Coronado           */
/* (C) Noviembre de 2018-2021 Mariano Ruiz (correcciones) */
/* Compresor                                              */

#include <stdio.h>
#include <stdlib.h>

/* Tipo nodo para árbol o Lista de árboles*/
/* El propósito es dual, sirve como elemento de una lista enlazada */
/* Cuando se usa el puntero sig, y como elemento de un árbol cuando */
/* se usan los punteros cero y uno */
typedef struct _nodo
{
   unsigned char letra;    /* Letra a la que hace referencia el nodo */
   int frecuencia;         /* veces que aparece la letra en el texto o las letras */
                           /* de los nodos de las ramas cero y uno */
   struct _nodo *sig;      /* Puntero a siguiente nodo de una lista enlazada */
   struct _nodo *cero;     /* Puntero a la rama cero de un árbol */
   struct _nodo *uno;      /* Puntero a la rama uno de un árbol */
} tipoNodo;                /* Nombre de tipo */

/* Nodo para construir una lista para la tabla de codigos */
typedef struct _tabla
{
   unsigned char letra;    /* Letra a la que hace referencia el nodo */
   unsigned long int bits; /* Valor de la codificación de la letra */
   unsigned char nbits;    /* Número de bits de la codificación */
   struct _tabla *sig;     /* Siguiente elemento de la tabla */
} tipoTabla;               /* Nombre del tipo */

/* Variables globales */
tipoTabla *Tabla;

/* Prototipos */
void Cuenta(tipoNodo** Lista, unsigned char c);
void Ordenar(tipoNodo** Lista);
void InsertarOrden(tipoNodo** Cabeza, tipoNodo *e);
void BorrarArbol(tipoNodo *n);
void CrearTabla(tipoNodo *n, int l, int v);
void InsertarTabla(unsigned char c, int l, int v);
tipoTabla *BuscaCaracter(tipoTabla *Tabla, unsigned char c);

int main(int argc, char *argv[])
{
   tipoNodo *Lista;       /* Lista de letras y frecuencias */
   tipoNodo *Arbol;       /* Arbol de letras y frecuencias */

   FILE *fe, *fs;         /* Ficheros de entrada y salida */
   unsigned char c;       /* variables auxiliares */
   tipoNodo *p;
   tipoTabla *t;
   int nElementos;        /* Número de elementos en tabla */
   long int Longitud = 0; /* Longitud del fichero original */

   unsigned long int dWORD; /* Soble palabra usada durante la codificación */
   int nBits;               /* Número de bits usados de dWORD */

   if(argc < 3)
   {
      printf("Usar:\n%s <fichero_entrada> <fichero_salida>\n", argv[0]);
      return 1;
   }

   Lista = NULL;
   /* Fase 1: contar frecuencias */
   fe = fopen(argv[1], "r");
   do {
      c = fgetc(fe);
      if( feof(fe) ) {
         break ;
      }
      Longitud++;        /* Actualiza la cuenta de la longitud del fichero */
      Cuenta(&Lista, c); /* Actualiza la lista de frecuencias */
   } while(1);
   fclose(fe);

   /* Ordenar la lista de menor a mayor */
   Ordenar(&Lista);

   /* Crear el arbol */
   Arbol = Lista;
   while(Arbol && Arbol->sig) /* Mientras existan al menos dos elementos en la lista */
   {
      p = (tipoNodo *)malloc(sizeof(tipoNodo)); /* Un nuevo árbol */
      p->letra = 0;                             /* No corresponde a ninguna letra */
      p->uno = Arbol;                           /* Rama uno */
      Arbol = Arbol->sig;                       /* Siguiente nodo en */
      p->cero = Arbol;                          /* Rama cero */
      Arbol = Arbol->sig;                       /* Siguiente nodo */
      p->frecuencia = p->uno->frecuencia +
                      p->cero->frecuencia;      /* Suma de frecuencias */
      InsertarOrden(&Arbol, p);                 /* Inserta en nuevo nodo */
   }                                            /* orden de frecuencia */

   /* Construir la tabla de códigos binarios */
   Tabla = NULL;
   CrearTabla(Arbol, 0, 0);

   /* Crear fichero comprimido */
   fs = fopen(argv[2], "wb");
   /* Escribir la longitud del fichero */
   fwrite(&Longitud, sizeof(long int), 1, fs);
   /* Cuenta el número de elementos de tabla */
   nElementos = 0;
   t = Tabla;
   while(t)
   {
      nElementos++;
      t = t->sig;
   }
   /* Escribir el número de elemenos de tabla */
   fwrite(&nElementos, sizeof(int), 1, fs);
   /* Escribir tabla en fichero */
   t = Tabla;
   while(t)
   {
      fwrite(&t->letra, sizeof(char), 1, fs);
      fwrite(&t->bits, sizeof(unsigned long int), 1, fs);
      fwrite(&t->nbits, sizeof(char), 1, fs);
      t = t->sig;
   }

   /* Codificación del fichero de entrada */
   fe = fopen(argv[1], "r");
   dWORD = 0; /* Valor inicial. */
   nBits = 0; /* Ningún bit */
   do {
      c = fgetc(fe);
      if( feof(fe) ) {
         break ;
      }
      /* Busca c en tabla: */
      t = BuscaCaracter(Tabla, c);
      /* Si nBits + t->nbits > 32, sacar un byte */
      while(nBits + t->nbits > 32)
      {
         c = dWORD >> (nBits-8);           /* Extrae los 8 bits de mayor peso */
         fwrite(&c, sizeof(char), 1, fs);  /* Y lo escribe en el fichero */
         nBits -= 8;                       /* Ya tenemos hueco para 8 bits más */
      }
      dWORD <<= t->nbits; /* Hacemos sitio para el nuevo caracter */
      dWORD |= t->bits;   /* Insertamos el nuevo caracter */
      nBits += t->nbits;  /* Actualizamos la cuenta de bits */
   } while(1);
   /* Extraer los cuatro bytes que quedan en dWORD*/
   while(nBits>0)
   {
      if(nBits>=8) c = dWORD >> (nBits-8);
      else c = dWORD << (8-nBits);
      fwrite(&c, sizeof(char), 1, fs);
      nBits -= 8;
   }

   fclose(fe);  /* Cierra los ficheros */
   fclose(fs);

   /* Borrar Arbol */
   BorrarArbol(Arbol);

   /* Borrar Tabla */
   while(Tabla)
   {
      t = Tabla;
      Tabla = t->sig;
      free(t);
   }

   return 0;
}

/* Actualiza la cuenta de frecuencia del carácter c */
/* El puntero a Lista se pasa por referencia, ya que debe poder cambiar */
/* Ya sea por que la lista esté vacía, o porque el nuevo elemento sea el */
/* primero */
void Cuenta(tipoNodo** Lista, unsigned char c)
{
   tipoNodo *p, *a, *q;

   if(!*Lista)  /* Si la lista está vacía, el nuevo nodo será Lista */
   {
      *Lista = (tipoNodo *)malloc(sizeof(tipoNodo));   /* Un nodo nuevo */
      (*Lista)->letra = c;                             /* Para c */
      (*Lista)->frecuencia = 1;                        /* en su 1ª aparición */
      (*Lista)->sig = (*Lista)->cero = (*Lista)->uno = NULL;
   }
   else
   {
      /* Buscar el caracter en la lista (ordenada por letra) */
      p = *Lista;
      a = NULL;
      while(p && p->letra < c)
      {
         a = p;      /* Guardamos el elemento actual para insertar */
         p = p->sig; /* Avanzamos al siguiente */
      }
      /* Dos casos: */
      /* 1) La letra es c se encontró */
      if(p && p->letra == c) p->frecuencia++; /* Actualizar frecuencia */
      else
      /* 2) La letra c no se encontró*/
      {
         /* Insertar un elemento nuevo */
         q = (tipoNodo *)malloc(sizeof(tipoNodo));
         q->letra = c;
         q->frecuencia = 1;
         q->cero = q->uno = NULL;
         q->sig = p;        /* Insertar entre los nodos p */
         if(a) a->sig = q;  /* y a */
         else *Lista = q;    /* Si a es NULL el nuevo es el primero */
      }
   }
}

/* Ordena Lista de menor a mayor por frecuencias */
/* De nuevo pasamos el puntero a lista por referencia */
void Ordenar(tipoNodo** Lista)
{
   tipoNodo *Lista2, *a;

   if(!*Lista) return; /* Lista vacia */
   Lista2 = *Lista;
   *Lista = NULL;
   while(Lista2)
   {
      a = Lista2;              /* Toma los elementos de Lista2 */
      Lista2 = a->sig;
      InsertarOrden(Lista, a); /* Y los inserta por orden en Lista */
   }
}

/* Inserta el elemento e en la Lista ordenado por frecuencia de menor a mayor */
/* El puntero a Cabeza se pasa por referencia */
void InsertarOrden(tipoNodo** Cabeza, tipoNodo *e)
{
   tipoNodo *p, *a;

   if(!*Cabeza) /* Si Cabeza es NULL, es el primer elemento */
   {
      *Cabeza = e;
      (*Cabeza)->sig = NULL;
   }
   else
   {
       /* Buscar el caracter en la lista (ordenada por frecuencia) */
       p = *Cabeza;
       a = NULL;
       while(p && p->frecuencia < e->frecuencia)
       {
          a = p;      /* Guardamos el elemento actual para insertar */
          p = p->sig; /* Avanzamos al siguiente */
       }
       /* Insertar el elemento */
       e->sig = p;
       if(a) a->sig = e;   /* Insertar entre a y p */
       else *Cabeza = e;    /* el nuevo es el primero */
    }
}

/* Función recursiva para crear Tabla */
/* Recorre el árbol cuya raiz es n y le asigna el código v de l bits */
void CrearTabla(tipoNodo *n, int l, int v)
{
   if(n->uno)  CrearTabla(n->uno, l+1, (v<<1)|1);
   if(n->cero) CrearTabla(n->cero, l+1, v<<1);
   if(!n->uno && !n->cero) InsertarTabla(n->letra, l, v);
}

/* Insertar un elemento en la tabla */
void InsertarTabla(unsigned char c, int l, int v)
{
   tipoTabla *t, *p, *a;

   t = (tipoTabla *)malloc(sizeof(tipoTabla)); /* Crea un elemento de tabla */
   t->letra = c;                               /* Y lo inicializa */
   t->bits = v;
   t->nbits = l;

   if(!Tabla)         /* Si tabla es NULL, entonces el elemento t es el 1º */
   {
      Tabla = t;
      Tabla->sig = NULL;
   }
   else
   {
       /* Buscar el caracter en la lista (ordenada por frecuencia) */
       p = Tabla;
       a = NULL;
       while(p && p->letra < t->letra)
       {
          a = p;      /* Guardamos el elemento actual para insertar */
          p = p->sig; /* Avanzamos al siguiente */
       }
       /* Insertar el elemento */
       t->sig = p;
       if(a) a->sig = t;  /* Insertar entre a y p */
       else Tabla = t;    /* el nuevo es el primero */
    }
}

/* Buscar un caracter en la tabla, devielve un puntero al elemento de la tabla */
tipoTabla *BuscaCaracter(tipoTabla *Tabla, unsigned char c)
{
   tipoTabla *t;

   t = Tabla;
   while(t && t->letra != c) t = t->sig;
   return t;
}

/* Función recursiva para borrar un arbol */
void BorrarArbol(tipoNodo *n)
{
   if(n->cero) BorrarArbol(n->cero);
   if(n->uno)  BorrarArbol(n->uno);
   free(n);
}

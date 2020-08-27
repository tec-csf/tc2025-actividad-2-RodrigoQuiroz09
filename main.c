/**
 * Autor - Rodrigo Quiroz Reyes
 * Fecha - 27/08/2020
 * Actividad individual 2: Apuntadores a funciones
 * Ejercicio - Recorrer un arreglo de manera genérica con cualquier tipo de dator (AKA iterador)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
 
#define N 5 //Tamaño predefinido de los apuntadores

int type;//Int que almacena el sizeof de cada dato para utilizarlo como parametro de filtro

typedef struct {
  char * titulo;
  int paginas;
}Libro;//Estructura libro para simbolizar el tipo de dato struct

typedef void (*print_t)(void*); //Conjunto de funciones para los prints
typedef void (*t_iterator)(void*,size_t,print_t *);//Conjunto de funciones para generalizar los iteradores
typedef void (*opcion_t)(t_iterator*); //Conjunto de funciones para los menus

//Funciones para facilitar el manejo de menus
void menu_int(t_iterator*);
void menu_double(t_iterator*);
void menu_libro(t_iterator*);
int print_it();

//Funciones para que me regresen los valores del arreglo de manera genérica mediante void*
void* begin(void*,size_t);
void* end(void*,size_t);
void* next(void*,size_t);
void* prev(void*,size_t);

//Funciones para separar de manera específica cada iterador
void forward(void*,size_t,print_t*);
void reverse(void*,size_t,print_t*);
void bidirectional(void*,size_t,print_t*);

// Funicones para hacer el llamado para recorrer el arreglo y que ayudan a la impresión de este de manera genérica
void print_function(void*);
void recorre(void*,t_iterator);

void print_int(void*);
void print_double(void*);
void print_libro(void*);

int main(){

  //Arreglo de apuntadores a funciones para facilitar la estructura de los menus
  opcion_t * menu = (opcion_t *) malloc( 3 * sizeof(opcion_t));
  *menu=menu_int;
  *(menu+1)=menu_double;
  *(menu+2)=menu_libro;

  //Arreglo para generalizar la opción del usuario para escoger un iterador
  t_iterator *iterators = (t_iterator *) malloc( 3 * sizeof(t_iterator));
  *iterators=forward;
  *(iterators+1)=reverse;
  *(iterators+2)=bidirectional;
  int i=1;
  time_t t;
  srand((unsigned) time(&t));
  
  while (i)
  {
    printf("------------------------------------\n");
    printf("Escoja un tipo de dato\n");
    printf("1.- INT\n");
    printf("2.- DOUBLE\n");
    printf("3.- Estructura Libro\n");
    printf("4.- EXIT\n");
    scanf("%d",&i);
    if(i<=3&&i>0){
      (*(menu+i-1))(iterators); //Apuntador a los diferentes tipos de menu
    }
    else{
      i=0;
    }
  }
  free(menu);
  free(iterators);
  return 0;
}

/**
 * Conjuntos de métodos para imprimir el arreglo dependiendo el tipo de dato que se le asigne
 * 
 * @param void* Elemento específico del arreglo mediante un void*
 */
void print_int(void* array){
  printf("%d\n",*((int*)array));
}
void print_double(void* array){
  printf("%f\n",*((double*)array));
}
void print_libro(void* array){
  printf("%s\n", ((Libro*)array)->titulo);
}

/**
 * Metodo genérico para recibir cualquier tipo de arreglo y tambien para que 
 * invoque cualquier tipo de iterador mediante ese arreglo dado
 * 
 * @param void* arreglo para iterar
 * @param t_iterador Seleccion del tipo de iterador para recorrer el arreglo
 */
void recorre(void* arr,t_iterator it){
  print_t * print = (print_t *) malloc( 4 * sizeof(print_t));//Conjunto de métodos para saber qué tipo de dato imprimir

  *print=print_int;
  *(print+1)=print_double;
  *(print+3)=print_libro;

  (*it)(arr,N,print); //Invocación del tipo de iterador

  free(print);

}

/**
 * Iterador para recorrer el arreglo desde su incio hasta el final
 * 
 * @param void* El arreglo a iterar 
 * @param size_t el tamaño del arreglo dado
 * @param print_t* el método de impresión de cada tipo de tipo
 */
void forward(void * arr, size_t count, print_t *pr){
  printf("------------------------------------\n");
  void * init=begin(arr,count); //Marca el inicio del arreglo
  void * final=end(arr,count); //Marca el final del arreglo

  for(;init<final;init=next(init,count)){ 
      (*(pr+(type/4)-1))(init); //Impresión genérica
  }
}

/**
 * Iterador que va en reversa, desde el final hasta el inicio
 * 
 * @param void* El arreglo a iterar 
 * @param size_t el tamaño del arreglo dado
 * @param print_t* el método de impresión de cada tipo de tipo
 */
void reverse(void * arr, size_t count, print_t *pr){
  printf("------------------------------------\n");
  void * init=end(arr,count); //Marca el final del arreglo
  void * final=begin(arr,count); //Marca el inicio del arreglo
   init=prev(init,count);
  
  for(;init>=final;init=prev(init,count)){ 
      (*(pr+(type/4)-1))(init); //Impresión genérica
  }

}

/**
 * Metodo para invocar los 2 iteradores para recorrer el arreglo
 * de las 2 maneras
 * 
 * @param void* El arreglo a iterar 
 * @param size_t el tamaño del arreglo dado
 * @param print_t* el método de impresión de cada tipo de tipo
 */
void bidirectional(void * arr, size_t count, print_t *pr)
{
  printf("Iterator Forward\n");
  forward(arr,N,pr); //Iterador forward
  printf("Iterator Reverse\n");
  reverse(arr,N,pr); //Iterador reverse
}

/**
 * Metodo para regresar el inicio del arreglo
 * 
 * @param void* apuntador de un elemento del arreglo
 * @param size_t tamaño del arreglo
 */
void *begin(void*arr,size_t count){
  void*aux=arr;
  return aux;
}

/**
 * Metodo para regresar un apuntador del último elemento del arreglo
 * 
 * @param void* apuntador de un elemento del arreglo
 * @param size_t tamaño del arreglo
 */
void*end(void*arr,size_t count){
  void*aux=(arr+(count*type)); //Se multiplica el tamaño por el sizeof acorde al tipo de dato y se le suma al apuntador que se envía
  return aux;
}

/**
 * Metodo para regresar el siguiente apuntador del arreglo
 * 
 * @param void* apuntador de un elemento del arreglo
 * @param size_t tamaño del arreglo
 */
void*next(void*arr,size_t count){
  void*aux=(arr+type); //Se suma el sizeof acorde el tipo de dato para pasar al siguiente elemento
  if(aux-type==end(arr,count)){ // Verífica si llegó al final del arreglo y regreso un NULL
    return NULL;
  }else{
    return aux;
  }
  
}

/**
 * Metodo para regresar el apuntador anterior del arreglo
 * 
 * @param void* apuntador de un elemento del arreglo
 * @param size_t tamaño del arreglo
 */
void*prev(void*arr,size_t count){
  void*aux=(arr-type);//Se resta el sizeof acorde el tipo de dato para pasar al anterior elemento
  if(aux+type==end(arr,count)){// Verífica si llegó al final del arreglo y regreso un NULL
    return NULL;
  }else{
    return aux;
  }
}

/**
 * Metodo para imprimir el menu para el tipo de dato int  
 * 
 * @param t_iterator sirve para generalizar una asignación del iterador a escoger 
 */
void menu_int(t_iterator* iter){

  int *array=(int*)malloc(N*sizeof(int)); //Arreglo dinámico de ints
  int *aux=array;
  int *final=array+N;

  for(; aux < final ; ++aux) {
    *aux= rand() % 50;//Asignacion random de valores
  }
  printf("Se ha generado un arreglo de 5 int\n");
  
  type=sizeof(int);
  int i=1;
  while (i)
  {
    i= print_it(); //Impresión genérica del menu
    if(i<4){
      recorre(array,*(iter+i-1));//Se selecciona el tipo de iterador a utilizar
    }
    else{
      i=0;
    }
  }
  
  free(array);
}

/**
 * Metodo para imprimir el menu para el tipo de dato double  
 * 
 * @param t_iterator sirve para generalizar una asignación del iterador a escoger 
 */
void menu_double(t_iterator* iter){
  double *array=(double*)malloc(N*sizeof(double));//Arreglo dinámico de double

  double *aux=array;
  double *final=array+N;

  for(; aux < final ; ++aux) {
    *aux= rand() % 50;//Asignacion random de valores
  }
  printf("Se ha generado un arreglo de 5 double\n");

  type=sizeof(double);
  int i=1;
  while (i)
  {
    i= print_it(); //Impresión genérica del menu
    if(i<4){
      recorre(array,*(iter+i-1)); //Se selecciona el tipo de iterador a utilizar
    }
    else{
      i=0;
    }
  }
  
  free(array);
}

/**
 * Metodo para imprimir el menu para el tipo de dato struct Libro 
 * 
 * @param t_iterator sirve para generalizar una asignación del iterador a escoger 
 */
void menu_libro(t_iterator* iter){

  Libro *array=(Libro*)malloc(N*sizeof(Libro)); //Arreglo dinámico de la estructura Libro

  Libro *aux=array;
  Libro *final=array+N;

  for(; aux < final ; ++aux ) {
    aux->titulo=(char*)malloc(sizeof(char)*20); //Arreglo dinámico de chars para el título del libro
    printf("Introduzca el título del libro \n");
    scanf(" %[^\n]", aux->titulo); //Asignación por parte del usuario
  }
  
  type=sizeof(Libro);
  int i=1;
  while (i)
  {
    i= print_it(); //Impresión genérica del menu
    if(i<4){
      recorre(array,*(iter+i-1)); //Se selecciona el tipo de iterador a utilizar
    }
    else{
      i=0;
    }
  }
  
  for(; aux < final ; ++aux ) {
      free(aux->titulo);
  }
  
  free(array);
}

/**
 * Metodo para la impresión del menu para que el usuario pueda escoger un iterador
 * 
 * @return int la opción que el usuario escogió
 */
int print_it(){
  int i=0;
  printf("------------------------------------\n");
  printf("Escoja un tipo de iterador\n");
  printf("1.- Forward\n");
  printf("2.- Reverse\n");
  printf("3.- Bidirectional\n");
  printf("4.- EXIT\n");
  scanf("%d",&i);

   return i;
}

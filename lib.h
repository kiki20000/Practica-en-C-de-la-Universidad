#ifndef _lib_H_
#define _lib_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>


typedef enum
{
    NUM,
    DATE,
    TEXT,
    ERR,
    EMAIL

}TYPE;

typedef struct tab
{
    char nombre[100];
    int numCampos;
    char **campos;
    TYPE *tipos;
    struct tab *sig, *ant;

}TABLE;

typedef struct line
{
    char **valores;
    struct line *sig, *ant;
    
}ROW;

void hola();
void crear_bd(char *nombre_bd);
int entrar_dir(char *nombre_bd);
void crear_meta(char *nombre_bd, char *aux2);
void crear_table(char *nombre_db, char *nombre_tabla, TABLE *tabla);
void editar_meta(char *nombre_bd, TABLE *tabla);
int contar_tablas(char *nombre_bd);
TYPE tipo(char *valor);
int entrar_tabla(char *nombre_db, char *nombre_tab);
int validar_caracteres(char *argumento);
TABLE* ultima_tabla(TABLE *tabla);
TABLE* inicializar_tabla(char *nombre, int num_campos, char **campos, TYPE *tipos);
void insertarUltima(TABLE *tabla, TABLE *taux);
TABLE* cargarTablas(char *nombre_bd);
void Trim(char *cad);
char* ObtenerCampo(char *lin,char *sep, int id, char *val);
void liberar_memoria_tabla(TABLE *tabla);
ROW* cargar_lineas(char *nombre_bd, char *nombre_tabla, int num_campos, TYPE* tipos);
void liberar_memoria_lineas(ROW *lineas, int num_campos);
TABLE* buscar_tabla(char *nombre_tabla, TABLE *tabla);
ROW* inicializar_linea(char **valores);
void insertarUltima_linea(ROW *fila, ROW *filanueva);
int FechaValidaDMA (char *cad);
int DecimalValidar (char *cad);
void actualizar_lineas(ROW *linea, TABLE *tablaactual, char *nombre_bd);
void imprimir_encabezado(TABLE *tablaactual);
void imprimir_todo(int numcampos, ROW *linea);
int FechatoNumeroDMA(char* cad);
char* NumerotoFechaDMA(int res);
void ordenar_asc(ROW *linea, TABLE *tabla, int campo);
void imprimir_campo_valor(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion);
void imprimir_todo_ASC(int numcampos, ROW *linea, TABLE *tablaactual);
void intercambio_valores(ROW *linea1, ROW *linea2);
int contar_lineas(ROW *linea);
void imprimir_todo_DES(int numcampos, ROW *linea, TABLE *tablaactual);
void ordenar_des(ROW *linea, TABLE *tablaactual, int campo);
char* MiStrTok(char *cad1, char *sep, int comillas);
void borrar_lineas(ROW *lineas, int num_campos, char *prompt, char *nombre_bd);
TABLE* borrar_tabla(TABLE *tabla, char *nombre_bd, char *tablaborrar);
ROW* borrar_linea_valor_deseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion);
void mostrar_tablas(TABLE* tabla, char *nombre_bd);
TABLE* renombrar_tabla(TABLE* tabla, char *nombre_bd, char *nombre_tabla, char *nombre_nuevo_tabla);
int FechaValidaAMD (char *cad);
int FechatoNumeroAMD(char* cad);
char* NumerotoFechaAMD(int res);
TABLE* borrar_bd (TABLE *tabla, char *nombre_bd);
int renombrar_BD(char *nombre_bd, char *nuevo_nombre_bd);
ROW* renombrar_linea_valor_deseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion, char *nuevovalor);
int EnteroValidar (char *cad);
int verDuplicados(char **cadena, int num_arg);
void mostrar_bd();
int validar_caracteres2(char *argumento);
int validar_caracteres3(char *argumento);


#endif
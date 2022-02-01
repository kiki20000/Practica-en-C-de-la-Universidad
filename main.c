#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // DIR
#include "lib.h"

//gcc lib.c main.c -o main.exe

int main()
{

    char prompt[100] = "*";
    char com[100];
    char comaux[100];
    
    int num_arg,num_arg_aux;
    char s2[8] = " \n\t";
    char barra[4] = "\\";
    char *comtok[100];
    char *comillas, *comillasaux, *paux;
    char nombre_bd[100] = "*";
    char nombre_tabla[100] = "*";
    char **nombres, **valores;
    TYPE *tipos;
    TABLE *tabla = NULL, *taux, *tablaactual;
    ROW *linea =NULL, *lineaux;
    int i,j,h, numero_tablas=0;
    int aux, aux1, aux2, aux3, aux4;

    hola();

    while (1)
    {

        printf("[%s]:>", prompt);
        gets(com);
        strcpy(comaux,com);
        num_arg = 0;
        comtok[num_arg] = strtok(com, s2); // aqui estamos cogiendo la primera palabra --> comtok [0]
        num_arg++;                         //pasamos a la siguente palabra
        while ((comtok[num_arg] = strtok(NULL, s2)) != NULL) //nos trocea el comando com por espacios
        {
            num_arg++;
        }

        if (comtok[0] == NULL) // si en el prompt no pone nada vuelve a salir el prompt
        { 
            continue;
        }
        if (strcmp(comtok[0], "exit") == 0) 
        {
            if (num_arg != 1)
            {
                printf("	## ERROR: comando 'exit' numero de parametros incorrecto\n\n");
                continue;
            }
            else if (tabla != NULL) //este caso se da cuando solo hay una db activa
            {
                if(linea != NULL){ // por si hubiese una tabla activa
                    liberar_memoria_lineas(linea, tablaactual->numCampos);
                    linea = NULL;
                }
                liberar_memoria_tabla(tabla);
                tabla = NULL; 
                return 0;
            }
            else if(tabla == NULL){ //este caso se da cuando no hay nada activo
                return 0;
            }
            else{
                printf("ERROR inesperado");
            }  
            
        }
        else if (strcmp(comtok[0], "new") == 0 && strcmp(comtok[1], "db") == 0) 
        { 
            if (num_arg != 3) // comprobamos que el numero de argumento es correcto
            {
                printf("## ERROR: comando 'new db' numero de parametros incorrecto\n\n");
                continue;
            }

            j = validar_caracteres(comtok[2]); //comprobamos que este todo bn escrito

            if(j==0){
                crear_bd(comtok[2]);
            }
        }
        else if (strcmp(comtok[0], "use") == 0 && strcmp(comtok[1], "db") == 0) 
        {   
            if (num_arg != 3) // comprobamos que el numero de argumento es correcto
            {
                printf("## ERROR: comando 'use db' numero de parametros incorrecto\n\n");
                continue;
            }
            if(strcmp(comtok[2], ".")==0 || strcmp(comtok[2], "..")==0){
                printf("ERROR esa base no se toca\n");
                continue;
            }

            aux2 = entrar_dir(comtok[2]); //comprobamos si la ruta existe si = 0, no = 1
            
            if(aux2 == 0){
                
                strcpy(prompt, comtok[2]);
                strcpy(nombre_bd, comtok[2]);
                strcpy(nombre_tabla, "*"); // por si es la segunda pasada y tiene una lista activa
                
                if(tabla != NULL){
                    //Liberar memoria
                    if(linea != NULL){ //esto lo hacemos por si hubiese tambn una tabla activa de antes
                        liberar_memoria_lineas(linea, tablaactual->numCampos);
                        linea = NULL;
                    }
                    liberar_memoria_tabla(tabla);
                    tabla = NULL;
                }
                tabla = cargarTablas(comtok[2]);

                mostrar_tablas(tabla, nombre_bd);
                
            }
        }
        else if (strcmp(comtok[0], "new") == 0 && strcmp(comtok[1], "table") == 0)
        { 
            if (num_arg % 2 == 0) // comprobamos que el numero de argumento es correcto
            {
                printf("## ERROR: comando 'new db' numero de parametros incorrecto\n\n");
                continue;
            }
            else if(num_arg<5){
                printf("## ERROR: comando 'new db' numero de parametros incorrecto\n\n");
                continue;
            }
            else if(strcmp(prompt,"*") == 0){
                printf("## ERROR: comando 'new table' necesita estar en una base activa\n\n");
                continue;
            }

            for(i=0;i<num_arg;i++){
                j = validar_caracteres(comtok[i]); //comprobamos q esta bn escrito
                if(j!=0){
                    break;
                }
            }

            if(j!=0){
                continue; // si esta algo mal escrito nos salimos para volver a escribir
            }

            nombres = malloc(sizeof(char*)*((num_arg-3)/2)); // reservamos memoria tipo puntero para los parametros
            tipos = malloc(sizeof(TYPE*)*((num_arg-3)/2));
            
            j=0;
            aux1=0;
            for(i=3; i<num_arg;i++){
                
                if(i%2==1){
                    j = validar_caracteres(comtok[i]);//COMPROBAMOS QUE ESTA BN ESCRITO
                    
                    if(j != 0){
                        break;
                    }
                    nombres[aux1]=strdup(comtok[i]); //metemos en nombres lo del comtok
                }
                else {
                    aux = tipo(comtok[i]); // COMPROBAMOS QUE ESTA BN ESCRITO                

                    printf("%d",aux);

                    if(aux == 3){
                        printf("ERROR en el tipo de dato\n"); 
                        j=1;
                        break;
                    }
                    
                    tipos[aux1]=aux; // metemos en tipos lo del comtok
                    printf("%d",tipos[aux1]);
                    aux1++;
                }
            }

            h = verDuplicados(nombres, aux1); // ESTO ME COMPRUBA QUE DOS CAMPOS NO SE LLAMEN IGUAL DENTRO DE LA MISMA TABLA

            if(j == 0 && h == 0){
                taux=inicializar_tabla(comtok[2],((i-3)/2),nombres,tipos); // creamos la tabla
                printf("%d", taux->tipos[0]);
            }
            else{

                for(i=0;i<aux1;i++){
                    free(nombres[i]);
                }

                free(nombres);
                free(tipos);
                continue;
            }

            if(taux != NULL){ // si todo sale bn nos metemos dentro
                if(tabla==NULL){ // si es la primera tabla
                    tabla=taux;
                    crear_table(nombre_bd,comtok[2], tabla); // la metemos en el meta
                }
                else{
                    insertarUltima(tabla, taux); // la insertamos la ultima
                    crear_table(nombre_bd,comtok[2], tabla); // la metemos en el meta
                }
            }
            else{
                printf("ERROR de punteros");
            }
        }
        else if (strcmp(comtok[0], "use") == 0 && strcmp(comtok[1], "table") == 0) 
        { 
            if (num_arg != 3) // comprobamos que el numero de argumento es correcto
            {
                printf("## ERROR: comando 'use table' numero de parametros incorrecto\n\n");
                continue;
            }

            aux3 = entrar_tabla(nombre_bd, comtok[2]); // comprobamos que se pueda entrar a la tabla

            if(aux3 == 1){

                strcpy(prompt, nombre_bd);
                strcpy(nombre_tabla, comtok[2]);
                strcat(prompt,barra);
                strcat(prompt,comtok[2]); 

                if(linea != NULL){
                    
                    //LIBERAR MEMORIA
                    liberar_memoria_lineas(linea, tablaactual->numCampos);
                    linea = NULL;
                }

                //BUSCAR TABLA
                tablaactual = buscar_tabla(comtok[2], tabla);

                //CARGAR TABLA
                linea = cargar_lineas(nombre_bd, comtok[2], tablaactual->numCampos, tablaactual->tipos);
                lineaux = linea;

                //ESTA PARTE COMENTADA NOS MUESTRA TODOS LOS VALORES DE LAS LINEAS
                /*while(lineaux != NULL){

                    for(i=0;i<tablaactual->numCampos;i++){

                        printf("[%s]",lineaux->valores[i]);

                    }
                    printf("\n");

                    lineaux=lineaux->sig;

                }*/
            }
        }
        else if (strcmp(comtok[0], "insert") == 0)
        { 
            aux4 = 0;
            comillas = strchr(comaux, '"');

            if(comillas != NULL){ // si hubiesen comillas entramos
                        
                comillasaux = strchr(comillas+1, '"');
                
                if(comillasaux == NULL){ // si no encontramos la pareja error
                    printf("ERROR de comillas\n");
                    aux4++;
                    continue;
                }
                else{

                    // LLamamos a mistrotok con las comillas activas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = MiStrTok(comaux, s2, 0); 
                    num_arg_aux++;

                    while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,1)) != NULL) // 1 equivale a comillas activadas
                    {
                        //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                        num_arg_aux++;   
                    }    
                }
            }
            else{

                //Llamamos a mistrotok con las comillas desactivadas
                num_arg_aux = 0;
                comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                num_arg_aux++;

                while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,0)) != NULL) // 0 equivale a comillas desactivadas
                {
                    //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                    num_arg_aux++; 
                }
            }

            if (num_arg_aux != (tablaactual->numCampos + 1)) // comprobamos que el numero de argumento es correcto
            {
                printf("## ERROR: comando 'insert' numero de parametros incorrecto\n\n");
                continue;
            }

            for(i=0;i<tablaactual->numCampos;i++){
                
                if(tablaactual->tipos[i] == NUM ){ 
                    if(DecimalValidar(comtok[i+1]) == 0){ // validamos que sea un numero bn escrito
                        printf("ERROR de NUMEROS\n");
                        aux4++;
                        break;
                    }
                }
                if(tablaactual->tipos[i] == DATE ){
                    if(FechaValidaDMA(comtok[i+1]) == 0){ // si la fecha es valida devolvera 1 y si es una fecha erronea devolvera 0
                        printf("ERROR de fechas\n");
                        aux4++;
                        break;
                    }
                }
                if(tablaactual->tipos[i] == EMAIL ){
                    if(validar_caracteres3(comtok[i+1]) == 0){ // si la fecha es valida devolvera 1 y si es una fecha erronea devolvera 0
                        
                        aux4++;
                        break;
                    }
                    
                }
                if(tablaactual->tipos[i] == TEXT ){
                    
                    // ESTO ESTA COMENTADO PORQUE PENSABA QUE DEBIAMOS VALIDAR TAMBN LO DE DENTRO DE LOS VALORES
                    // j = validar_caracteres(comtok[i+1]);

                    // if(j != 0){
                    //     aux4++;
                    //     break;
                    // }
                }
            }

            if(aux4!=0){
                continue;
            }

            valores = malloc(sizeof(char*)*tablaactual->numCampos); // reservamos memoria

            for(i=0;i<tablaactual->numCampos;i++){
                valores[i]=strdup(comtok[i+1]); // metemos en valores los parametros del comtok
            }

            lineaux = inicializar_linea(valores); // creamos la estructura de la linea

            if(lineaux != NULL){ // si todo sale bn nos metemos
                if(linea==NULL){ // si es el primer valor
                    
                    linea=lineaux;
                    
                    //Actualizar tabla
                    actualizar_lineas(linea, tablaactual, nombre_bd); // escribimos en el fichero los valores
                }
                else{

                    insertarUltima_linea(linea, lineaux);

                    //Actualizar tabla
                    actualizar_lineas(linea, tablaactual, nombre_bd); // escribimos en el fichero los valores
                }
            }
            else{
                printf("ERROR de punteros");
            }

        }
        else if (strcmp(comtok[0], "select") == 0)
        { 
            if (tabla == NULL)
            {
                printf("Todavia no ha cargado ninguna bd\n");
                continue;
            }  
            if (strcmp(prompt, nombre_bd) == 0)
            {
                printf("Todavia no ha cargado ninguna tabla\n");
                continue;
            }            
            
            //------------------------------------------------------------

            comillas = strchr(comaux, '"');
            
            if(comillas != NULL){
                        
                comillasaux = strchr(comillas+1, '"');
                
                if(comillasaux == NULL){
                    printf("ERROR de comillas");
                    break;
                }
                else{
                    // LLamamos a mistrotok con las comillas activas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                    num_arg_aux++;

                    while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,1)) != NULL){
                        
                        //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                        num_arg_aux++;  
                    }
                }
            }
            else{

                //Llamamos a mistrotok con las comillas desactivadas
                num_arg_aux = 0;
                comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                num_arg_aux++;

                while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,0)) != NULL)
                {
                    //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                    num_arg_aux++;
                    
                }
            }
            //-------------------------------------------------------

            if (num_arg_aux == 1) // si solo han puesto select
            {
                //IMPRIMIR TODA LA TABLA ACTIVA
                imprimir_encabezado(tablaactual);
                imprimir_todo(tablaactual->numCampos, linea);
            }
            else if(num_arg_aux == 2){ // si han puesto select ASC o select DES

                if(strcmp(comtok[1], "ASC") == 0 || strcmp(comtok[1], "DES") == 0){

                    imprimir_encabezado(tablaactual);

                    if(strcmp(comtok[1], "ASC") == 0){
                        
                        //ORDENAMOS ASCENDENTE
                        imprimir_todo_ASC(tablaactual->numCampos, linea, tablaactual);

                    }
                    if (strcmp(comtok[1], "DES") == 0){
                        
                        //ORDENAMOS DESCENDENTE
                        imprimir_todo_DES(tablaactual->numCampos, linea, tablaactual);
                    }
                }
                else{
                    printf("ERROR parametros ASC/DES mal introducidos\n");
                }
            }
            else if (num_arg_aux == 4) // select F = hola
            {
                //MOSTRAR LOS CAMPOS
                i=0;
                for(i;i<tablaactual->numCampos;i++){

                    if(strcmp(tablaactual->campos[i], comtok[1]) == 0){ // comprobamos que la columna existe
                        
                        if(tablaactual->tipos[i] == NUM && DecimalValidar(comtok[3])==1){

                            imprimir_encabezado(tablaactual);
                            imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                            break;
                        }
                        else if(tablaactual->tipos[i] == DATE && FechaValidaDMA(comtok[3])==1){

                            imprimir_encabezado(tablaactual);
                            imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                            break;
                        }
                        else if(tablaactual->tipos[i] == TEXT){

                            imprimir_encabezado(tablaactual);
                            imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                            break;
                        }
                        else{
                            printf("ERROR de tipos\n");
                            i++;
                            continue;
                        }
                    }

                    if(i+1 == tablaactual->numCampos){
                        printf("campo no encontrado\n");
                    }
                }
                
            }
            else if(num_arg_aux == 5){ // en el caso de que pongas "select T = hola ASC"
                
                if(strcmp(comtok[4],"ASC")==0){
                    for(i=0;i<tablaactual->numCampos;i++){

                        if(strcmp(tablaactual->campos[i], comtok[1]) == 0){
                            
                            if(tablaactual->tipos[i] == NUM && DecimalValidar(comtok[3])==1){

                                imprimir_encabezado(tablaactual);
                                ordenar_asc(linea,tablaactual,0); // en el caso de que pidan ordenar y no sea por el primer campo, solo deberemos modificar el 0
                                imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                                break;
                            }
                            else if(tablaactual->tipos[i] == DATE && FechaValidaDMA(comtok[3])==1){

                                imprimir_encabezado(tablaactual);
                                ordenar_asc(linea,tablaactual,0); // en el caso de que pidan ordenar y no sea por el primer campo, solo deberemos modificar el 0
                                imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                                break;
                            }
                            else if(tablaactual->tipos[i] == TEXT){

                                imprimir_encabezado(tablaactual);
                                ordenar_asc(linea,tablaactual,0); // en el caso de que pidan ordenar y no sea por el primer campo, solo deberemos modificar el 0
                                imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                                break;
                            }
                            else{
                                printf("ERROR de tipos\n");
                                i++;
                                continue;
                            }
                        }
                        if(i+1 == tablaactual->numCampos){
                            printf("campo no encontrado\n");
                        }
                    }
                    
                }

                if(strcmp(comtok[4],"DES")==0){
                    for(i=0;i<tablaactual->numCampos;i++){

                        if(strcmp(tablaactual->campos[i], comtok[1]) == 0){
                            
                            if(tablaactual->tipos[i] == NUM && DecimalValidar(comtok[3])==1){

                                imprimir_encabezado(tablaactual);
                                ordenar_des(linea,tablaactual,0); // en el caso de que pidan ordenar y no sea por el primer campo, solo deberemos modificar el 0
                                imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                                break;
                            }
                            else if(tablaactual->tipos[i] == DATE && FechaValidaDMA(comtok[3])==1){

                                imprimir_encabezado(tablaactual);
                                ordenar_des(linea,tablaactual,0); // en el caso de que pidan ordenar y no sea por el primer campo, solo deberemos modificar el 0
                                imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                                break;
                            }
                            else if(tablaactual->tipos[i] == TEXT){

                                imprimir_encabezado(tablaactual);
                                ordenar_des(linea,tablaactual,0); // en el caso de que pidan ordenar y no sea por el primer campo, solo deberemos modificar el 0
                                imprimir_campo_valor(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                                break;
                            }
                            else{
                                printf("ERROR de tipos\n");
                                i++;
                                continue;
                            }
                        }
                        if(i+1 == tablaactual->numCampos){
                            printf("campo no encontrado\n");
                        }
                    }
                }   
            }
            else{
                printf("ERROR numero de parametros incorrecto\n");
            }
        }
        else if (strcmp(comtok[0], "list") == 0){

             if (num_arg != 1)
            {
                printf("	## ERROR: comando 'list' numero de parametros incorrecto\n\n");
                continue;
            }

            mostrar_bd();

        }
        else if (strcmp(comtok[0], "delete") == 0){

            //---------- MODIFICAR VALOR LINEA ---------------------- renombra nombre = vicente Paco

            /*if (tabla == NULL)
            {
                printf("Todavia no ha cargado ninguna bd\n");
                continue;
            }  
            if (strcmp(prompt, nombre_bd) == 0)
            {
                printf("Todavia no ha cargado ninguna tabla\n");
                continue;
            }             

            j = validar_caracteres(comtok[4]);
            if(j != 0){
                printf("ERROR de caracteres");
                continue;
            }

            comillas = strchr(comaux, '"');
            
            if(comillas != NULL){
                        
                comillasaux = strchr(comillas+1, '"');
                
                if(comillasaux == NULL){
                    printf("ERROR de comillas");
                    break;
                }
                else{
                    // LLamamos a mistrotok con las comillas activas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                    num_arg_aux++;

                    while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,1)) != NULL){
                        
                        //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                        num_arg_aux++;  
                    }
                }
            }
            else{

                //Llamamos a mistrotok con las comillas desactivadas
                num_arg_aux = 0;
                comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                num_arg_aux++;

                while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,0)) != NULL)
                {
                    //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                    num_arg_aux++;
                    
                }
            }

            if (num_arg_aux == 5) 
            {
                //MOSTRAR LOS CAMPOS
                for(i=0;i<tablaactual->numCampos;i++){

                    if(strcmp(tablaactual->campos[i], comtok[1]) == 0){ // comprobamos que la columna existe
                        
                        if(tablaactual->tipos[i] == NUM && DecimalValidar(comtok[3])==1){

                            imprimir_encabezado(tablaactual);
                            linea = renombrar_linea_valor_deseado(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i, comtok[4]);
                            actualizar_lineas(linea, tablaactual, nombre_bd);
                            break;
                        }
                        else if(tablaactual->tipos[i] == DATE && FechaValidaDMA(comtok[3])==1){

                            imprimir_encabezado(tablaactual);
                            linea = renombrar_linea_valor_deseado(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i, comtok[4]);
                            actualizar_lineas(linea, tablaactual, nombre_bd);
                            break;
                        }
                        else if(tablaactual->tipos[i] == TEXT){

                            imprimir_encabezado(tablaactual);
                            linea = renombrar_linea_valor_deseado(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i, comtok[4]);
                            actualizar_lineas(linea, tablaactual, nombre_bd);
                            break;
                        }
                        else{
                            printf("ERROR de tipos\n");
                        }
                    }
                }
            }*/


            //---------------------------------------------------------

            //------------ RENOMBRAR BD ---------------------------- renombrar vicente Alberto

            // controlar numero de argumentos y que no hayan tablas activas

            /*j = validar_caracteres(comtok[2]);

            if(j != 0){
                printf("ERROR de caracteres");
                continue;
            }

            j = renombrar_BD(nombre_bd, comtok[2]);

            if(j<2){
                continue;
            }
            else{

                strcpy(nombre_bd, comtok[2]);
                strcpy(prompt, comtok[2]);

            }*/

            //-------------------------------------------------------

            // ----------- BORRAR UNA BD ----------------------------- borrar vicente
            /*tabla = borrar_bd(tabla, comtok[1]);
            editar_meta(comtok[1], tabla);
            char aux6[100] = "";
            strcpy(aux6, comtok[1]);
            strcat(aux6, "\\meta.db");
            remove(aux6);
            rmdir(comtok[1]);
            strcpy(nombre_bd, "*");
            strcpy(prompt, "*");*/
            //-------------------------------------------------------

            // ----------- RENOMBRAR TABLAS ----------------------------- 

            /*j = validar_caracteres(comtok[2]);

            if(j != 0){
                printf("ERROR de caracteres");
                continue;
            }

            //validar que no este en la tabla a cambiar y este en la base de datos activa

            tabla = renombrar_tabla(tabla, nombre_bd, comtok[1], comtok[2]);
        
            editar_meta(nombre_bd, tabla);
            mostrar_tablas(tabla, nombre_bd);*/
            
            //-----------------------------------------------------


            //--------------- MOSTRAR TABLAS -------------------
            //faltan hacer algunas comprobaciones
            /*
            if (num_arg != 1)
            {
                printf("	## ERROR: comando 'mostrar' numero de parametros incorrecto\n\n");
                continue;
            }

            mostrar_tablas(tabla, nombre_bd);*/

            //---------------------------------------------------


            //----------------------------- BORRAR_VALOR_DESEADO-------------------------
            if (tabla == NULL)
            {
                printf("Todavia no ha cargado ninguna bd\n");
                continue;
            }  
            if (strcmp(prompt, nombre_bd) == 0)
            {
                printf("Todavia no ha cargado ninguna tabla\n");
                continue;
            }             

            comillas = strchr(comaux, '"');
            
            if(comillas != NULL){
                        
                comillasaux = strchr(comillas+1, '"');
                
                if(comillasaux == NULL){
                    printf("ERROR de comillas");
                    break;
                }
                else{
                    // LLamamos a mistrotok con las comillas activas
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                    num_arg_aux++;

                    while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,1)) != NULL){
                        
                        //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                        num_arg_aux++;  
                    }
                }
            }
            else{

                //Llamamos a mistrotok con las comillas desactivadas
                num_arg_aux = 0;
                comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                num_arg_aux++;

                while ((comtok[num_arg_aux] = MiStrTok(NULL, s2,0)) != NULL)
                {
                    //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                    num_arg_aux++;
                    
                }
            }

            if (num_arg_aux == 4) 
            {
                //MOSTRAR LOS CAMPOS
                for(i=0;i<tablaactual->numCampos;i++){

                    if(strcmp(tablaactual->campos[i], comtok[1]) == 0){ // comprobamos que la columna existe
                        
                        if(tablaactual->tipos[i] == NUM && DecimalValidar(comtok[3])==1){

                            //imprimir_encabezado(tablaactual);
                            linea = borrar_linea_valor_deseado(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                            actualizar_lineas(linea, tablaactual, nombre_bd);
                            break;
                        }
                        else if(tablaactual->tipos[i] == DATE && FechaValidaDMA(comtok[3])==1){

                            //imprimir_encabezado(tablaactual);
                            linea = borrar_linea_valor_deseado(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                            actualizar_lineas(linea, tablaactual, nombre_bd);
                            break;
                        }
                        else if(tablaactual->tipos[i] == TEXT){

                            //imprimir_encabezado(tablaactual);
                            linea = borrar_linea_valor_deseado(tablaactual->numCampos, linea, tablaactual->tipos[i], comtok[3], comtok[2], i);
                            actualizar_lineas(linea, tablaactual, nombre_bd);
                            break;
                        }
                        else{
                            printf("ERROR de tipos\n");
                        }
                    }
                }
            }
            else{
                printf("Comando mal introducido\n");
            }

            
            //---------------- BORRAR TABLAS -------------------------------------------            

            /*if(num_arg != 2){
                printf("ERROR de argumentos en 'borrar' ");
                continue;
            }
            if(strcmp(nombre_bd, "*") == 0 ){
                printf("ERROR no hay base de datos activa ");
                continue;
            }
            if(strcmp(nombre_tabla, "*") != 0 ){
                printf("ERROR tabla activa, salga a la bd de datos");
                continue;
            }

            tabla = borrar_tabla(tabla, nombre_bd, comtok[1]);
            editar_meta(nombre_bd, tabla);
            tabla = cargarTablas(nombre_bd);*/
            
            //--------------------------------------------------------------------------
            
           //-------------------- BORRAR_LINEAS ---------------------------------------- 
            /*if (num_arg != 1)
            {
                printf("	## ERROR: comando 'exit' numero de parametros incorrecto\n\n");
                continue;
            }

            if (!strcmp(prompt, nombre_bd) == 0) //este caso se da cuando hay una db y tabla activa
            {
                borrar_lineas(linea,tablaactual->numCampos, prompt, nombre_bd);
                linea=NULL;
            }
            else{
                printf("Debe haber una tabla activa\n");
            } */    
            //---------------------------------------------------------------------------  
        }
        else{
            printf("Comando no reconocido\n");
        }
    }

    printf("FIN...");
    return 0;
}
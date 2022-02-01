#include "lib.h"
#define PATH_MAX 260


void hola(){ // escribe la cabecera

    printf("Vicente Candela Perez\n");
    printf("Vicente.candela01@goumh.umh.es\n\n\n");

    printf("Ejercicio 1: Si\nEjercicio 2: si\n Ejercicio 4: si\n");
    printf("El ejercicio 3 esta medio hecho, si haces un new table lo pone en el meta, y validar email esta parcialmente hecho, el resto esta sin hacer\n");

    printf("Usted esta autorizado a realizar los siguientes comandos\n");
    printf("exist\n");
    printf("new db <nombre_base_de_datos>\n");
    printf("use db <nombre_base_de_datos>\n");
    printf("new table <nombre_tabla> <campo> <tipo> [<campo> <tipo> ...]\n");
    printf("use table <nombre_base_de_datos>\n");
    printf("insert <valor> [<valor> ...]\n");
    printf("select [<campo> * <valor>] [<orden>]\n");

}

void crear_bd(char *nombre_bd){ // creamos la base de datos

    char aux1[11]="\\meta.db";
    char aux2[50]="";

    strcpy(aux2,nombre_bd);
    strcat(aux2,aux1);

    if(access(nombre_bd,F_OK) == 0 ){  // si ya hay una bd con ese nombre error
        printf("\tBD ya creada\n");
    }
    else{ // sino creamos la db
        mkdir(nombre_bd); // creamos el directorio
        crear_meta(nombre_bd, aux2);   // creamos el meta
    }
}

int entrar_dir(char *nombre_bd){ // nos cambia el prompt al estado actual
    
    DIR *dir;
    if(dir = opendir(nombre_bd)){ // entramos al directorio
        return 0;
    }
    else{ // esa bd no existe
        printf("La Base de Datos %s no fue encontrada\n", nombre_bd);
        return 1;
    }

}

TABLE* cargarTablas(char *nombre_bd){ //Carga las tablas en memoria

    TABLE *tabla=NULL, *tablanueva;
    FILE *meta;
    char aux1[11]="\\meta.db";
    char aux2[50]="";
    char aux3[50]="";
    char linea[1000],val[100];
    int num_tabla,num_campos, i, j;
    char *nombre_tabla;
    char **nombre_campo;
    TYPE *tipos; 
    

    strcpy(aux2,nombre_bd);
    strcat(aux2,aux1);

    meta = fopen(aux2, "r"); // abrimos el meta en modo lectura
    if(meta == NULL){
        printf("ERROR inesperado");
        return 0;
    }
    fgets(linea,1000,meta); // leemos los primeros mil caracteres de una linea
    ObtenerCampo(linea,":",1,val); // optenemos el valor del campo 1
    Trim(val);                  // le quitamos los espacios al valor
    num_tabla = atoi(val);      // lo convertimos a numero

    for(i=0; i<num_tabla;i++){ // vamos a hacer este proceso tantas veces como tablas tengamos en el meta

        fgets(linea,1000,meta); // IGNORAMOS LOS GUIONES DEL META
        fgets(linea,1000,meta); // cogemos la linea del nombre de la tabla y num de campos. "CANDELA: 2"
        ObtenerCampo(linea, ":", 0,val); // cogemos el primer valor de la linea hasta los dos puntos "CANDELA"
        Trim(val);                  // Quita los espacios
        nombre_tabla=strdup(val);   // apuntamos nombre_tabla al valor de val
        ObtenerCampo(linea,":",1,val); // cogemos el numero de campos "2"
        Trim(val);                  //quita los espacios
        num_campos=atoi(val);       // convertimos a numero el valor de numcampos "2" 

        nombre_campo = malloc(sizeof(char*)*num_campos); // reservamos memoria tipo puntero para los parametros
        tipos = malloc(sizeof(TYPE*)*num_campos); //Reservmos memoria para los tipos
        
        for(j=0;j<num_campos;j++){ // lo vamos a hacer por cada columna de la tabla
            fgets(linea,1000,meta); // cogemos la linea
            ObtenerCampo(linea,":",0,val);  // vemos el nombre del campo
            Trim(val);
            nombre_campo[j]=strdup(val); // lo copiamos en la caja de nombre_campo que es un puntero doble
            ObtenerCampo(linea,":",1,val); // cogemos el tipo
            Trim(val);
            tipos[j]=tipo(val); // lo metemos en la caja de tipos
        }

        tablanueva = inicializar_tabla(nombre_tabla,num_campos, nombre_campo, tipos); // creamos la tabla

        if(tabla==NULL){ // si es la primera tabla
            tabla=tablanueva;
                    
        }
        else{ // sino la insertamos la ultima
            insertarUltima(tabla, tablanueva);          
        }

    }
    fclose(meta); // cerramos el meta
    return tabla;
}

void Trim(char *cad){ //ESTA FUNCION QUITA LOS ESPACIOS EN BLANCO
	int c, i, j, n=strlen(cad);
	
	if (cad==NULL || n<1)
		return;
	n--;
	while(n>=0 && ((c=cad[n])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		n--;
	cad[n+1]='\0';
	i=0;
	while (i<=n && ((c=cad[i])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		i++;
	for (j=0 ; j<=n-i+1; j++)
		cad[j]=cad[j+i];
}

char* ObtenerCampo(char *lin, char *sep, int id, char *val) // ESTA FUNCION NOS DEVUELVE EL VALOR DEL CAMPO QUE HEMOS INDICADO
{                                                           
	int i, tamSep=1;
	char *p1, *p2;
	
	if (lin==NULL || lin[0]=='\0')
		return NULL;
	
	p1=lin;
	p2=strstr(lin, sep);
	
	for(i=0; i<id;i++)
	{
		p1=p2+tamSep;
		p2=strstr(p1, sep);
	}
	if(p2==NULL)
		strcpy(val,p1);
	else
	{
		strncpy(val, p1, p2-p1);
		val [p2-p1]='\0';
	}
	return val;
}

int entrar_tabla(char *nombre_db, char *nombre_tab){
    DIR *dir;
    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";

    strcpy(aux3, nombre_db);
    strcat(aux3,aux2);
    strcat(aux3,nombre_tab);
    strcat(aux3,aux1);

    if(access(aux3,F_OK) == 0 ){

        if(dir = opendir(nombre_db)){ // entramos al directorio
            return 1;
        }
    }
    else{
        printf("La tabla: %s no fue encontrada\n", nombre_tab);
        return 0;
    }
}

ROW* cargar_lineas(char *nombre_bd, char *nombre_tabla, int num_campos, TYPE* tipos){

    char aux1[5]=".tab";
    char aux2[5]="\\";
    char aux3[100]="";
    FILE *fichero;
    char linea[1000];
    int i,comillas;
    long final;
    char *token, *lineaaux;
    char **valores;
    ROW *fila = NULL, *filanueva;

    strcpy(aux3,nombre_bd);
    strcat(aux3,aux2);
    strcat(aux3,nombre_tabla);
    strcat(aux3,aux1);

    if(access(aux3,F_OK) == 0 ){ // si podemos acceder a la ruta entramos

        fichero = fopen(aux3, "r"); // abrimos el fichero en modo lectura

        fseek( fichero, 0, SEEK_END ); // situa el cursor en la ultima posicion

        if (ftell( fichero ) == 0 ) // dime donde esta el cursor en el fichero, y si esta en la posicion 0 entramos
        {
            fclose(fichero);
            return fila;
        }

        final=ftell(fichero); // dime donde esta la posicion del fichero
        fseek(fichero,0,SEEK_SET); // mueve el cursor al principio del fichero, es decir, a la posicion 0


        while(final != ftell(fichero)){ // mientras el cursor no llegue al final del fichero

            fgets(linea,1000,fichero); // cogemos la linea

            valores = malloc(sizeof(char*)*num_campos); //  reservamos memoria del tamanyo de los num_campos

            for(i=0;i<num_campos;i++){
             
                comillas = (tipos[i] == TEXT); // si hay un campo de tipo texto comillas valdra 1
            
                if(i == 0){
                    lineaaux = linea;
                }
                else{
                    lineaaux = NULL;
                }
                token = MiStrTok(lineaaux,",\n",comillas); //dividimos la linea en tokens con las comillas activadas
                valores[i]=strdup(token);
            }

            filanueva = inicializar_linea(valores); //creamos la estructura de la linea

            if(fila==NULL){ // si es la primera linea
                fila=filanueva;       
            }
            else{
                insertarUltima_linea(fila, filanueva); // insertamos la linea la ultima  
            }
        }                 
    }
    else{
        printf("ERROR inesperado");
        return 0;
    }

    fclose(fichero);
    return fila;

}

void insertarUltima_linea(ROW *fila, ROW *filanueva){

    ROW *filaaux;

    filaaux = fila; // hacemos uso de un auxiliar para no perder el puntero de la primera fila
    while(filaaux->sig != NULL){
        filaaux = filaaux->sig;
    }
    filaaux->sig = filanueva;
    filanueva->ant = filaaux;
    filanueva->sig = NULL;

}

void liberar_memoria_lineas(ROW *lineas, int num_campos){

    ROW *lineasiguiente;
    int i;

    if(lineas == NULL){ // por si hubiese algun error
        return;
    }

    while (lineas != NULL) // mientras hayan lineas
    {
        lineasiguiente = lineas->sig; // pasamos a la siguiente

        for(i=0; i<num_campos;i++){ // esto por cada campo de la linea

            free(lineas->valores[i]); // liberamos la memoria de cada campo
        }

        free(lineas->valores); // liberamos la caja de valores
        free(lineas); //liberamos la linea

        lineas = lineasiguiente;
    }
}

TABLE* buscar_tabla(char *nombre_tabla, TABLE *tabla){ // IMPORTANTE

    if(tabla != NULL ){ // si la tabla no es nula

        while(tabla != NULL){ // mientras la tabla no sea nula

            if(strcmp(tabla->nombre, nombre_tabla) == 0){ // comparamos que la se llame igual q lo que han escrito
                return tabla;
            }
            tabla=tabla->sig;
        }
    }
    else{
        return tabla;
    }
}

void crear_meta(char *nombre_bd, char *aux2){ // creamos el meta inicial

    DIR *dir;
    if(dir = opendir(nombre_bd)){// entramos al directorio

        FILE *fichero; // creamos el meta
        fichero = fopen(aux2,"w"); // abrimos el txt
        if(fichero != NULL){

            fprintf(fichero, "TABLAS: 0\n");
            fprintf(fichero,"-----------------\n");

            fclose(fichero);
        }
        else{
        printf("algo salio mal\n");
        }
    } 
}

void crear_table(char *nombre_bd, char *nombre_tabla, TABLE *tabla){ //Crea el fichero en el que insertaremos datos

    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";
    TABLE *tablanueva, *tablaultima;

    strcpy(aux3,nombre_bd);
    strcat(aux3,aux2);
    strcat(aux3,nombre_tabla);
    strcat(aux3,aux1);

    if(access(aux3,F_OK) == 0 ){

        printf("Tabla: %s ya creada\n",nombre_tabla);
    }
    else{
    
        DIR *dir;
        if(dir = opendir(nombre_bd)){// entramos al directorio

            FILE *fichero; // creamos el meta
            fichero = fopen(aux3,"w"); // abrimos el txt
            if(fichero != NULL){
                
                editar_meta(nombre_bd, tabla); //vamos a editar el meta

            }
            else{
                printf("algo salio mal\n");
            }
            fclose(fichero);
        }
    }
}

void insertarUltima(TABLE *tabla, TABLE *taux){ //inserta la tabla en la ultima posicion
    
    TABLE *tablanueva;

    tablanueva = tabla;
    while(tablanueva->sig != NULL){
        tablanueva = tablanueva->sig;
    }
    tablanueva->sig = taux;
    taux->ant = tablanueva;
}

TABLE* ultima_tabla(TABLE *tabla){ //Nos situa en la ultima tabla de la lista

    while(tabla->sig != NULL){
        tabla=tabla->sig;
    }

    return tabla;
}

ROW* inicializar_linea(char **valores){ // Nos crea la estructura de una linea y le asigna los valores

    ROW *linea = malloc(sizeof(ROW));
    if (linea == NULL)
    {
        return NULL;
    }
    linea->valores = valores;
    linea->ant = NULL;
    linea->sig = NULL;
    
    return linea;
}

TABLE* inicializar_tabla(char *nombre, int num_campos, char **campos, TYPE *tipos){
    //Tener en cuenta que si llamamos a esto, campos y tipos tienen que ser punteros y ya estar escritos
    int i;

    TABLE *tabla=malloc(sizeof(TABLE)); //Reservamos memoria para una tabla
    if(tabla == NULL){ // Por si algo saliese mal
        return NULL;
    }
    strcpy(tabla->nombre, nombre); // copiamos el nombre
    tabla->numCampos = num_campos; // copiamos el numero de campos   
    tabla->campos = campos;  // copiamos los campos
    tabla->tipos = tipos;   // copiamos los tipos
    tabla->sig = NULL;
    tabla->ant = NULL;

    // printf("%s, %d\n", tabla->nombre, tabla->numCampos);
    // for(i=0; i< tabla->numCampos; i++){
    //     printf("%s: %d\n", tabla->campos[i], tabla->tipos[i]);
        
    // }
    return tabla;
}

void editar_meta(char *nombre_bd, TABLE *tabla){ //Estamos escribiendo en el meta

    DIR *dir;
    char aux1[11]="\\meta.db";
    char aux2[50]="";
    char aux3[50]="";
    int numero_tablas = 0;
    int i;

    strcpy(aux2,nombre_bd);
    strcat(aux2,aux1);
    numero_tablas = contar_tablas(nombre_bd);

    if(dir = opendir(nombre_bd)){// entramos al directorio

        FILE *fichero; // creamos el meta
        fichero = fopen(aux2,"w"); // abrimos el txt
        if(fichero != NULL){

            fprintf(fichero, "TABLAS: %d\n", numero_tablas);
            fprintf(fichero,"-----------------\n");

            
            if(numero_tablas == 1){ // en el caso de que solo haya una tabla
                fprintf(fichero, "%s: %d\n", tabla->nombre, tabla->numCampos); // escribimos en el fichero su nombre y numero de campos

                for(i=0; i< tabla->numCampos; i++){ // hacemos un bucle para ir escribiendo el tipo de datos

                    if(tabla->tipos[i] == 0 ){
                        strcpy(aux3,"NUM");
                    }
                    if(tabla->tipos[i] == 1 ){
                        strcpy(aux3,"DATE");
                    }
                    if(tabla->tipos[i] == 4 ){
                        strcpy(aux3,"EMAIL");
                    }
                    if(tabla->tipos[i] == 2 ){
                        strcpy(aux3,"TEXT");
                    }
                    

                    fprintf(fichero, "%s: %s\n", tabla->campos[i], aux3); // escribimos en el fichero
                }
            }
            else{

                while(tabla != NULL){ // mientras sigan habiendo tablas
                    
                    fprintf(fichero, "%s: %d\n", tabla->nombre, tabla->numCampos); // escribimos el nombre y el numero de tablas
                    
                    for(i=0; i< tabla->numCampos; i++){ // hacemos un bucle para ir escribiendo el tipo de datos

                        if(tabla->tipos[i] == 0 ){
                            strcpy(aux3,"NUM");
                        }
                        if(tabla->tipos[i] == 1 ){
                            strcpy(aux3,"DATE");
                        }
                        if(tabla->tipos[i] == 4 ){
                        strcpy(aux3,"EMAIL");
                        }
                        if(tabla->tipos[i] == 2 ){
                            strcpy(aux3,"TEXT");
                        }

                        fprintf(fichero, "%s: %s\n", tabla->campos[i], aux3); // escribimos en el fichero
                    }

                    if(tabla->sig == NULL){ // si la siguiente tabla apunta a nulo nos salimos del bucle
                        break;
                    }
                    fprintf(fichero,"-----------------\n");
                    tabla=tabla->sig; 
                }
            }

            fclose(fichero); // cerramos el fichero
        }
        else{
        printf("algo salio mal\n");
        }
    } 

}

int contar_tablas(char *nombre_bd){ // esta funcion cuenta el numero de ficheros de la carpeta

    DIR *d;
    struct dirent *f;
    int num = 0;

    if(d = opendir(nombre_bd)){
        while(f = readdir(d)){
        num++;
        }
    }
    num = num - 3; // aqui restamos 3 porque una tabla es . otra es .. y otra es el meta
    
    return num;

}

TYPE tipo(char *valor){ //Cogemos texto y devolvemos un TYPE

    if(strcmp(valor, "DATE") == 0){
        return DATE;
    }
    else if(strcmp(valor, "NUM") == 0){
        return NUM;
    }
    else if(strcmp(valor, "TEXT") == 0){
        return TEXT;
    }
    else if(strcmp(valor, "EMAIL")==0){
        return EMAIL;
    }
    else{
       return ERR;
    }
}

void liberar_memoria_tabla(TABLE *tabla){ // esto sirve para quitar de la memoria las tablas cargadas

    TABLE *tablasiguiente;
    int i;

    if(tabla == NULL){ // por si algo saliese mal
        return;
    }

    while(tabla != NULL){ // mientras hayan tablas

        tablasiguiente = tabla->sig;

        for(i=0; i<tabla->numCampos;i++){
            free(tabla->campos[i]); // liberamos los campos de la tabla, que era un doble puntero
        }
        free(tabla->tipos); // liberamos la caja de tipos
        free(tabla->campos); // liberamos la caja de campos
        free(tabla); // liberamos la tabla

        tabla = tablasiguiente;
    }
}

int validar_caracteres(char *argumento){ //Nos aseguramos que esta todo bn escrito
    
    char cadena[100]="";
    int j,errores;

    strcpy(cadena,argumento);
    
    j=0;
    errores=0;
    while(cadena[j] != '\0'){

        if(cadena[j] == '_'){
            j++;
            continue;
        }
        if(cadena[j]>='0' && cadena[j]<='9'){
            j++;
            continue;
        }
        if(cadena[j]>='a' && cadena[j]<='z'){
            j++;
            continue;
        }
        if(cadena[j]>='A' && cadena[j]<='Z'){
            j++;
            continue;
        }
        printf("ERROR de caracteres\n");
        j++;
        errores++;
        break;
    }

    return errores;
}

int validar_caracteres2(char *argumento){ //Nos aseguramos que esta todo bn escrito
    
    char cadena[100]="";
    int j,errores;

    strcpy(cadena,argumento);
    
    j=0;
    errores=0;
    while(cadena[j] != '\0'){

        if(cadena[j] != '.'){
            j++;
            continue;
        }
        else{
            errores++;
            break;
        }
    }

    return errores;
}

int validar_caracteres3(char *argumento){ //Nos aseguramos que esta todo bn escrito
    
    char cadena[100]="";
    int j, h,a , errores;

    strcpy(cadena,argumento);
    
    j=0;
    h=0;
    a = 0;
    errores=0;
    while(cadena[j] != '\0'){

        if(cadena[0]=='.' || cadena[0]=='@'){
            errores++;
            break;
        }

        if(cadena[j]>='0' && cadena[j]<='9'){
            j++;
            continue;
        }
        if(cadena[j]>='a' && cadena[j]<='z'){
            j++;
            continue;
        }
        if(cadena[j]>='A' && cadena[j]<='Z'){
            j++;
            continue;
        }
        if(cadena[j] == '.'){
            if(strlen(cadena) == j)
            {
                errores++;
                breaK;
            }
            j++;
            h++;
            printf("%d",h);
            
            continue;
        }
        if(cadena[j] == '@'){
            
            if(strlen(cadena) == j)
            {
                errores++;
                breaK;
            }
            j++;
            a++;
            printf("%d",a);
            continue;
        }

        printf("ERROR de caracteres\n");
        errores++;
        break;
    }

    if(a != 1 || h != 1){
        printf("Un email no se escribe asi\n");
        return 1;
    }

    return errores;
}


int FechaValidaDMA (char *cad){ // comprobamos que sea una fecha valida
    
    int i, d, m, a;
	int error=0;

    
	for (i=0; i<10; i++)
	{
        if (i!=2 && i!=5)
        {
            if (cad[i]<'0' || cad [i]>'9')
            error=1;
        }
	}
	if ((cad[2]!='/' || cad[5]!='/')) // Si pide que la fecha acepte guiones añadimos esto && (cad[2]!='-' || cad[5]!='-')
    	error=1;
    	
    d=(cad[0]-'0')*10 +(cad[1]-'0');
    m=(cad[3]-'0')*10 +(cad[4]-'0');
    a=(cad[6]-'0')*1000 +(cad[7]-'0')*100+(cad[8]-'0')*10+ (cad[9]-'0');
   
    if (error==1)
        return 0;
    else if (strlen(cad)!=10)
        return 0;
    else if(d<1 || m<1 || a<1 || d>31 || m>12 || a>9999 )
        return 0;
    else if((m==4 || m==6 || m==9|| m==11) && d>30)
        return 0;
    else if (m==2 && a%4==0 && d>29)
        return 0;
    else if (m==2 && a%4!=0 && d>28)
        return 0;
    else 
		return 1;
}

int DecimalValidar (char *cad) //ESTO VALIDA TANTO DECIMALES, REALES y ENTEROS
{
	int i, ini, p=0, j;
	
	if(cad[0]=='+' || cad[0]=='-'){
        ini=1;
    }	
	else{
        ini=0;
    }
		
	for(i=ini; i<strlen(cad); i++)
    {
        if (cad[i]<'0' || cad[i]>'9')
        {
            if (cad[i]=='.'){
                p++;
            }
            else{
                return 0;
            }       
        }
        if (p==2){
            return 0;
        }     
    }
	if (ini==1 && strlen(cad)==1){
        return 0;
    }	
	if (strlen(cad)==0){
        return 0;
    }
		
	if (ini==1 && strlen(cad)==2 && cad[1]=='.'){
        return 0;
    }	
	if (ini==0 && strlen(cad)==1 && cad[0]=='.'){
        return 0;
    }
	return 1;	
}

void actualizar_lineas(ROW *linea, TABLE *tablaactual, char *nombre_bd){ // escribimos en el fichero todos los valores de las lineas

    DIR *dir;
    char aux1[11]=".tab";
    char aux2[50]="\\";
    char aux3[50]="";

    int i;

    strcpy(aux3,nombre_bd);
    strcat(aux3,aux2);
    strcat(aux3, tablaactual->nombre);
    strcat(aux3,aux1); // creamos la ruta de directorio

    if(dir = opendir(nombre_bd)){// entramos al directorio

        FILE *fichero; // creamos el meta
        fichero = fopen(aux3,"w"); // abrimos el txt
        if(fichero != NULL){

            while (linea != NULL){ // mientras hayan lineas
                
                for(i=0;i<tablaactual->numCampos-1;i++){ // escribimos cada valor para su campo menos en la ultima columna
                    if(tablaactual->tipos[i] == TEXT ){ // este caso lo hacemos para escribir las comillas en txt de los tipo texto
                        fprintf(fichero, "\"%s\", ",linea->valores[i]); //estamos escribiendo en el txt los valores de la linea
                    }
                    else{
                        fprintf(fichero, "%s, ", linea->valores[i]); // aqui es la situacion del DATE y NUM que se escriben normal
                    }
                }

                if(tablaactual->tipos[tablaactual->numCampos-1] == TEXT ){ // escribimos en la ultima columna para asi añadir el \n
                    fprintf(fichero, "\"%s\"\n",linea->valores[tablaactual->numCampos-1]);
                }
                else{
                    fprintf(fichero, "%s\n", linea->valores[tablaactual->numCampos-1]);
                }    

                if(linea->sig == NULL){
                    break;
                }

                linea = linea->sig;

            }
            fclose(fichero);
        }
        else{
        printf("algo salio mal\n");
        }
    } 
}

void imprimir_encabezado(TABLE *tablaactual){ // imprime el encabezado de la tabla

    int i;

    for(i=0;i<tablaactual->numCampos;i++){
        printf("%s\t",tablaactual->campos[i]);
    }

    printf("\n");
}

void imprimir_todo(int numcampos, ROW *linea){ // imprime todos los valores de la linea

    int i, j=1;
    ROW *lineaaux = linea;

    if (linea == NULL){
        printf("No se han encontrado datos\n");
    }
    else{
        while(lineaaux != NULL){

            printf("%d- ", j); // este es el contador de cada linea
            for(i=0;i<numcampos-1;i++){
                printf("%s,",lineaaux->valores[i]); // aqui escribimos todos los valores menos el ultimo de la linea
            }

            printf("%s\n", lineaaux->valores[numcampos-1]); // aqui escribimos el ultimo valor para asi añadir el \n
            j++;

            lineaaux = lineaaux->sig;

        }
    }
}

void imprimir_campo_valor(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion){ // select P = 20

    int i, j=1;
    ROW *lineaaux = linea;

    if (linea == NULL){ // por si el fichero estuviese vacio
        printf("No se han encontrado datos\n");
    }
    else{

        if(tipo == NUM){ // esto se lo pasamos por parametro
            if(strcmp(simbolo,"=")==0){ // en el caso de que quiera igualdad de numeros
                
                while(lineaaux != NULL){ // mientras hayan lineas

                    if(atof(valor) == atof(lineaaux->valores[posicion])){ // convertimos el texto a numeros reales y los comparamos
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]); // imprimimos los valores de esa linea menos el ultimo
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]); // escribimos el ultimo valor y añadimos  \n
                        j++; 
                    }

                    lineaaux = lineaaux->sig;
                }
            }
            else if(strcmp(simbolo,">")==0){
                while(lineaaux != NULL){

                    if(atof(lineaaux->valores[posicion]) > atof(valor)){
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]);
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]);
                        j++; 
                    }

                    lineaaux = lineaaux->sig;

                }
            }
            else if(strcmp(simbolo,"<")==0){
                while(lineaaux != NULL){

                    if(atof(lineaaux->valores[posicion]) < atof(valor)){
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]);
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]);
                        j++; 
                    }

                    lineaaux = lineaaux->sig;

                }
            }
            else{
                printf("solo acepto > ,< o =\n");
            }
        }
        else if(tipo == DATE){ // este es el caso de que sea tipo fecha
            if(strcmp(simbolo,"=")==0){
                while(lineaaux != NULL){ // mientras hayan lineas

                    if(FechatoNumeroDMA(valor) == FechatoNumeroDMA(lineaaux->valores[posicion])){ // convertimos las fechas a numeros para asi poder compararlas
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]); // escribimos todos los valores menos el ultimo
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]); // escribimos el ultimo valor mas \n
                        j++; 
                    }

                    lineaaux = lineaaux->sig;
                }
            }
            else if(strcmp(simbolo,">")==0){
                while(lineaaux != NULL){

                    if(FechatoNumeroDMA(lineaaux->valores[posicion]) > FechatoNumeroDMA(valor)){
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]);
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]);
                        j++; 
                    }

                    lineaaux = lineaaux->sig;
                }
            }
            else if(strcmp(simbolo,"<")==0){
                while(lineaaux != NULL){

                    if(FechatoNumeroDMA(lineaaux->valores[posicion]) < FechatoNumeroDMA(valor)){
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]);
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]);
                        j++; 
                    }

                    lineaaux = lineaaux->sig;
                }
            }
            else{
                printf("solo acepto > ,< o =\n");
            }
        }
        else if(tipo == TEXT){ // en el caso de que sea tipo texto
            if(strcmp(simbolo,"=")==0){
                while(lineaaux != NULL){ // mientras hayan lineas

                    if(strcmp(valor,lineaaux->valores[posicion])==0){ // comparamos la cadena de caracteres
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]); // escribimos todos los valores menos el ultimo
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]); // escribimos el ultimo mas \n
                        j++; 
                    }

                    lineaaux = lineaaux->sig;

                }
            }
            else if(strcmp(simbolo,">")==0){
                while(lineaaux != NULL){

                    if(strcmp(lineaaux->valores[posicion],valor)>0){
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]);
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]);
                        j++; 
                    }

                    lineaaux = lineaaux->sig;

                }
            }
            else if(strcmp(simbolo,"<")==0){
                while(lineaaux != NULL){

                    if(strcmp(lineaaux->valores[posicion],valor)<0){
                        printf("%d- ", j);

                        for(i=0;i<numcampos-1;i++){
                            printf("%s, ",lineaaux->valores[i]);
                        }

                        printf("%s\n", lineaaux->valores[numcampos-1]);
                        j++; 
                    }

                    lineaaux = lineaaux->sig;

                }
            }
            else{
                printf("solo acepto > ,< o =\n");
            }
        } 
    }
}

void imprimir_todo_ASC(int numcampos, ROW *linea, TABLE *tablaactual){

    int i, j=1;
    ROW *lineaaux = linea;

    if (linea == NULL){
        printf("No se han encontrado datos\n");
    }
    else{
        ordenar_asc(lineaaux, tablaactual, 0); // en el caso de que pidan ordenar por un campo que no sea el primero, deberemos modificar el 0
        
        while(lineaaux != NULL){ // mientras hayan lineas
            printf("%d- ",j);
            for(i=0;i<numcampos-1;i++){
                printf("%s,",lineaaux->valores[i]); // escribimos todos los valores menos el ultimo
            }
            printf("%s\n", lineaaux->valores[numcampos-1]); // escribimos el ultimo valor mas el  \n
            j++;
            lineaaux = lineaaux->sig; 
        }
             
    }

}

void imprimir_todo_DES(int numcampos, ROW *linea, TABLE *tablaactual){

    int i, j=1;
    ROW *lineaaux = linea;

    if (linea == NULL){
        printf("No se han encontrado datos\n");
    }
    else{
        ordenar_des(lineaaux, tablaactual, 0); // en el caso de que pidan ordenar y no sea por el primer campo, solo deberemos modificar el 0
        
        while(lineaaux != NULL){ // mientras hayan lineas
            printf("%d- ",j); 
            for(i=0;i<numcampos-1;i++){
                printf("%s,",lineaaux->valores[i]); // escribimos todos los valores menos el ultimo
            }
            printf("%s\n", lineaaux->valores[numcampos-1]); // escribimos el ultimo mas \n
            j++;
            lineaaux = lineaaux->sig; 
        }
             
    }

}

int FechatoNumeroDMA(char* cad) // de fecha pasamos a numero en el formato DMA
{
	int res, dia, mes, anyo;
	dia=(cad[0]-'0')*10 +(cad[1]-'0');
    mes=(cad[3]-'0')*10 +(cad[4]-'0');
    anyo=(cad[6]-'0')*1000 +(cad[7]-'0')*100+(cad[8]-'0')*10+ (cad[9]-'0');
	res=dia;

	if (mes==1)
		res=res;
	else if (mes==2)
		res=res+31;
	else if (mes==3 && (anyo%4)!=0)
		res=res+59;
	else if (mes==3 && (anyo%4)==0)
		res=res+60;
	else if (mes==4 && (anyo%4)!=0)
		res=res+90;
	else if (mes==4 && (anyo%4)==0)
		res=res+91;
	else if (mes==5 && (anyo%4)!=0)
		res=res+120;
	else if (mes==5 && (anyo%4)==0)
		res=res+121;
	else if (mes==6 && (anyo%4)!=0)
		res=res+151;
	else if (mes==6 && (anyo%4)==0)
		res=res+152;
	else if (mes==7 && (anyo%4)!=0)
		res=res+181;
	else if (mes==7 && (anyo%4)==0)
		res=res+182;
	else if (mes==8 && (anyo%4)!=0)
		res=res+212;
	else if (mes==8 && (anyo%4)==0)
		res=res+213;
	else if (mes==9 && (anyo%4)!=0)
		res=res+243;
	else if (mes==9 && (anyo%4)==0)
		res=res+244;
	else if (mes==10 && (anyo%4)!=0)
		res=res+273;
	else if (mes==10 && (anyo%4)==0)
		res=res+274;
	else if (mes==11 && (anyo%4)!=0)
		res=res+304;
	else if (mes==11 && (anyo%4)==0)
		res=res+305;
	else if (mes==12 && (anyo%4)!=0)
		res=res+334;
	else if (mes==12 && (anyo%4)==0)
		res=res+335;

	res=(res+(anyo-1)*365);
	res=(res+(anyo-1)/4);


	return res;
}

char* NumerotoFechaDMA(int res) // de numero pasamos a fecha en el formato DMA
{
	int anyo, mes=0, dia, bi,i;
	char *fecha;
	float a;

	fecha=malloc(sizeof(char)*10);

	a=0.00273785463;
	anyo=((res)*a);
	bi=((anyo)/4);
	anyo=anyo+(bi/365);
	if (anyo%4!=0)
		res=(res-bi-((anyo-1)*365));
	else if (anyo%4==0)
		res=(res-bi-((anyo-1)*365));


	if (res<=31)
		mes=1;
	else if (res<=59 && (anyo%4)!=0)
	{
		mes=2;
		res=res-31;
	}
	else if (res<=60 && (anyo%4)==0)
	{
		mes=2;
		res=res-31;
	}
	else if (res<=90 && (anyo%4)!=0)
	{
		mes=3;
		res=res-59;
	}
	else if (res<=91 && (anyo%4)==0)
	{
		mes=3;
		res=res-60;
	}
	else if (res<=120 && (anyo%4)!=0)
	{
		mes=4;
		res=res-90;
	}
	else if (res<=121 && (anyo%4)==0)
	{
		mes=4;
		res=res-91;
	}
	else if (res<=151 && (anyo%4)!=0)
	{
		mes=5;
		res=res-120;
	}
	else if (res<=152 && (anyo%4)==0)
	{
		mes=5;
		res=res-121;
	}
	else if (res<=181 && (anyo%4)!=0)
	{
		mes=6;
		res=res-151;
	}
	else if (res<=182 && (anyo%4)==0)
	{
		mes=6;
		res=res-152;
	}
	else if (res<=212 && (anyo%4)!=0)
	{
		mes=7;
		res=res-181;
	}
	else if (res<=213 && (anyo%4)==0)
	{
		mes=7;
		res=res-182;
	}
	else if (res<=243 && (anyo%4)!=0)
	{
		mes=8;
		res=res-212;
	}
	else if (res<=244 && (anyo%4)==0)
	{
		mes=8;
		res=res-213;
	}
	else if (res<=273 && (anyo%4)!=0)
	{
		mes=9;
		res=res-243;
	}
	else if (res<=274 && (anyo%4)==0)
	{
		mes=9;
		res=res-244;
	}
	else if (res<=304 && (anyo%4)!=0)
	{
		mes=10;
		res=res-273;
	}
	else if (res<=305 && (anyo%4)==0)
	{
		mes=10;
		res=res-274;
	}
	else if (res<=334 && (anyo%4)!=0)
	{
		mes=11;
		res=res-304;
	}
	else if (res<=335 && (anyo%4)==0)
	{
		mes=11;
		res=res-305;
	}
	else if (res<=365 && (anyo%4)!=0)
	{
		mes=12;
		res=res-334;
	}
	else if (res<=366 && (anyo%4)==0)
	{
		mes=12;
		res=res-335;
	}
	dia=res;
	if (dia==0)
	{
		dia=31;
		mes=12;
		anyo=anyo-1;
	}
	if (dia==-1)
	{
		dia=30;
		mes=12;
		anyo=anyo-1;
	}
    if(dia<10 && mes<10)
		sprintf(fecha,"0%i/0%i/%i",dia,mes,anyo);
	else if (dia<10 && mes>=10)
		sprintf(fecha,"0%i/%i/%i",dia,mes,anyo);
	else if (dia>=10 && mes<10)
		sprintf(fecha,"%i/0%i/%i",dia,mes,anyo);
	else
		sprintf(fecha,"%i/%i/%i",dia,mes,anyo);

	return fecha;
}

int contar_lineas(ROW *linea){ // contamos las lineas que hay

    int i=0;
    ROW *lineaaux;

    lineaaux = linea;

    while (lineaaux != NULL){
        
        i++;
        lineaaux= lineaaux ->sig;

    }

    return i;
}

void ordenar_asc(ROW *linea, TABLE *tablaactual, int campo){

    ROW *i, *j;

    if(tablaactual->tipos[campo] == NUM){

        for(i=linea; i != NULL; i = i->sig){
   
            for (j=i; j != NULL; j = j->sig){
               
                if(atof(i->valores[campo]) > atof(j->valores[campo])){
                   // printf("%s > %s\n", i->valores[campo], j->valores[campo]);
                   // printf("CAMBIANDO\n");
                    intercambio_valores(i, j);

                }   
            }
        }
    }
    if(tablaactual->tipos[campo] == DATE){
        for(i=linea; i != NULL; i = i->sig){
   
            for (j=i; j != NULL; j = j->sig){
               
                if(FechatoNumeroDMA(i->valores[campo]) > FechatoNumeroDMA(j->valores[campo])){
                    //printf("%s > %s\n", i->valores[campo], j->valores[campo]);
                    //printf("CAMBIANDO\n");
                    intercambio_valores(i, j);

                }   
            }
        }
    }
    if(tablaactual->tipos[campo] == TEXT){
        for(i=linea; i != NULL; i = i->sig){
   
            for (j=i; j != NULL; j = j->sig){
               
                if(strcmp(i->valores[campo],j->valores[campo])>0){
                    //printf("%s > %s\n", i->valores[campo], j->valores[campo]);
                    //printf("CAMBIANDO\n");
                    intercambio_valores(i, j);

                }   
            }
        }
    }
}

void ordenar_des(ROW *linea, TABLE *tablaactual, int campo){

    ROW *i, *j;

    if(tablaactual->tipos[campo] == NUM){

        for(i=linea; i != NULL; i = i->sig){
   
            for (j=i; j != NULL; j = j->sig){
               
                if(atof(i->valores[campo]) < atof(j->valores[campo])){
                    //printf("%s > %s\n", i->valores[campo], j->valores[campo]);
                    //printf("CAMBIANDO\n");
                    intercambio_valores(i, j);

                }   
            }
        }
    }
    if(tablaactual->tipos[campo] == DATE){
        for(i=linea; i != NULL; i = i->sig){
   
            for (j=i; j != NULL; j = j->sig){
               
                if(FechatoNumeroDMA(i->valores[campo]) < FechatoNumeroDMA(j->valores[campo])){
                    //printf("%s > %s\n", i->valores[campo], j->valores[campo]);
                    //printf("CAMBIANDO\n");
                    intercambio_valores(i, j);

                }   
            }
        }
    }
    if(tablaactual->tipos[campo] == TEXT){
        for(i=linea; i != NULL; i = i->sig){
   
            for (j=i; j != NULL; j = j->sig){
               
                if(strcmp(i->valores[campo],j->valores[campo])<0){
                    //printf("%s > %s\n", i->valores[campo], j->valores[campo]);
                    //printf("CAMBIANDO\n");
                    intercambio_valores(i, j);

                }   
            }
        }
    }
}

void intercambio_valores(ROW *linea1, ROW *linea2){

    char **lineaaux;

    lineaaux = linea1->valores;
    linea1->valores = linea2->valores;
    linea2->valores = lineaaux;
}

char* MiStrTok(char *cad1, char *sep, int comillas) // funciona como strtok pero en el tercer parametro si ponemos 0 quiere decir que no lleva comillas y si ponemos 1 que si
{                                                   // por lo que para dar preferencia a dividir por comillas usamos esta
	static char *pt1=NULL;
	static char *pt2=NULL;
	static char *ptFinal;

	if(cad1!=NULL)
	{
		pt1 = cad1;
		ptFinal = pt1+strlen(cad1);
	}
	else
	{
		pt1=pt2+1;
	}

	if(comillas == 1)
	{
		while( pt1[0]!='\0' && pt1<ptFinal && strchr(sep, pt1[0])!=NULL && pt1[0]!='"')
			pt1++;
		if(pt1[0]=='"')
		{
			pt1++;
			pt2=pt1+1;
			while( pt2[0]!='\0' && pt2[0]!='"' )
				pt2++;
			if(pt2[0]=='\0')
			{
				pt1=pt2=ptFinal=NULL;
				return NULL;
			}
			else
			{
				pt2[0]='\0';
				return pt1;
			}
		}
	}
	else
	{
		while( pt1[0]!='\0' && pt1<ptFinal && strchr(sep, pt1[0])!=NULL )
			pt1++;
	}

	if(pt1>=ptFinal)
	{
		pt1=pt2=ptFinal=NULL;
		return NULL;
	}

	pt2=pt1+1;
	while( pt2[0]!='\0' &&  strchr(sep, pt2[0])==NULL )
		pt2++;
	pt2[0]='\0';

	return pt1;
}

int EnteroValidar (char *cad)
{
	int i, ini;
	
	if(cad[0]=='+' || cad[0]=='-') // en el caso de que queramos q sea un numero entero positivo, quitaremos la parte del -
		ini=1;
	else 
		ini=0;
	for (i=ini; i<strlen(cad); i++)
		{
			if (cad[i]<'0' || cad[i]>'9')
				return 0;
		}
	if (ini==1 && strlen(cad)==1)
		return 0;
	if (ini==0 && strlen(cad)==0)
		return 0;
return 1;
}

TABLE* borrar_tabla(TABLE *tabla, char *nombre_bd, char *tablaborrar){ // BORRAR UN NODO CONCRETO DE UNA LISTA

    TABLE *tablasiguiente, *tableaux, *tablaanterior;
    DIR *dir;
    int i, aux;
    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";


    if(dir = opendir(nombre_bd)){
    
        if(tabla == NULL)
        {
            printf("Tabla vacia");
            return tabla;
        }

        aux = entrar_tabla(nombre_bd, tablaborrar);
        printf("3");

        if(aux == 1){

            strcpy(aux3,nombre_bd);
            strcat(aux3,aux2);
            strcat(aux3,tablaborrar);
            strcat(aux3,aux1);

            printf("4");
            tableaux = buscar_tabla(tablaborrar, tabla);

            tablaanterior = tableaux->ant;
            tablasiguiente = tableaux->sig;
            
            if(tableaux == tabla && tableaux->sig == NULL){
                printf("primer caso");
                tabla=NULL;
            }
            else if(tableaux == tabla && tableaux->ant == NULL){

                printf("segundo caso");
                tabla = tabla->sig;
                tablasiguiente->ant = NULL;
                tableaux ->sig = NULL;

            }
            else if(tableaux->sig == NULL){
                
                printf("tercer caso");
                tablaanterior->sig = NULL;
                tableaux->ant = NULL;
            }
            else{

                printf("ultimo caso");
                tablaanterior->sig = tableaux->sig;
                tablasiguiente->ant = tableaux->ant;
            }

            for(i=0;i<tableaux->numCampos;i++){

                free(tableaux->campos[i]);
            }

            free(tableaux->tipos);
            free(tableaux->campos);
            free(tableaux);

            remove(aux3);

        }

    }
    return tabla;
}

void borrar_lineas(ROW *lineas, int num_campos, char *prompt, char *nombre_bd){ // BORRAR TODAS LAS LINEAS DE UN FICHERO

    ROW *lineasiguiente;
    DIR *dir;
    char aux1[10] = ".tab";
    char aux2[110] = "";    
    int i;

    printf("%s\n",prompt);
    strcpy(aux2, prompt);
    strcat(aux2, aux1);

    printf("%s\n", aux2);

    if(dir = opendir(nombre_bd)){

        printf("Entramos en la carpeta\n");

        FILE *fichero;
        fichero = fopen(aux2, "w");
        if(fichero != NULL){
            printf("Entramos en el fichero\n");
            if(lineas != NULL){

                liberar_memoria_lineas(lineas, num_campos);

                fprintf(fichero, "");
                fclose(fichero);
            }
            printf("ya esta todo borrado\n");
        }
        else{
            printf("Ya esta todo borrado\n");
        }
    }
    else{
        printf("ERROR al abrir el directorio");
    }
}

ROW* borrar_linea_valor_deseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion){ // BORRAR EL VALOR DESEADO

    int i, j=1, k;
    ROW *lineaaux = linea, *lineasiguiente, *lineaanterior;

    if (linea == NULL){ // por si el fichero estuviese vacio
        printf("No se han encontrado datos\n");
    }
    else{

        if(tipo == NUM){ // esto se lo pasamos por parametro
          
            if(strcmp(simbolo,"=")==0){ // en el caso de que quiera igualdad de numeros
                
                while(lineaaux != NULL){ // mientras hayan lineas

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(atof(valor) == atof(lineaaux->valores[posicion])){ // convertimos el texto a numeros reales y los comparamos

                        if(lineaaux == linea && lineaaux->sig == NULL){
                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        
                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);

                    }

                    lineaaux = lineasiguiente;
                }
                return linea;
            }
            else if(strcmp(simbolo,">")==0){
                while(lineaaux != NULL){

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(atof(lineaaux->valores[posicion]) > atof(valor)){
                       
                        if(lineaaux == linea && lineaaux->sig == NULL){
 
                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);

                        j++; 
                    }

                    lineaaux = lineasiguiente;

                }
                return linea;
            }
            else if(strcmp(simbolo,"<")==0){
                while(lineaaux != NULL){

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(atof(lineaaux->valores[posicion]) < atof(valor)){
                        
                        if(lineaaux == linea && lineaaux->sig == NULL){
                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);

                        j++; 
                    }

                    lineaaux = lineasiguiente;

                }
                return linea;
            }
            else{
                printf("solo acepto > ,< o =\n");
            }
        }
        else if(tipo == DATE){ // este es el caso de que sea tipo fecha

            if(strcmp(simbolo,"=")==0){
  
                while(lineaaux != NULL){ // mientras hayan lineas

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(FechatoNumeroDMA(valor) == FechatoNumeroDMA(lineaaux->valores[posicion])){ // convertimos las fechas a numeros para asi poder compararlas

                        if(lineaaux == linea && lineaaux->sig == NULL){

                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        
                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);

                        j++; 
                    }

                    lineaaux = lineasiguiente;
                }

                return linea;
            }
            else if(strcmp(simbolo,">")==0){
                while(lineaaux != NULL){

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(FechatoNumeroDMA(lineaaux->valores[posicion]) > FechatoNumeroDMA(valor)){
                       
                        if(lineaaux == linea && lineaaux->sig == NULL){

                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        
                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);


                    }

                    lineaaux = lineasiguiente;
                }

                return linea;
            }
            else if(strcmp(simbolo,"<")==0){
                while(lineaaux != NULL){

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(FechatoNumeroDMA(lineaaux->valores[posicion]) < FechatoNumeroDMA(valor)){
                       
                        if(lineaaux == linea && lineaaux->sig == NULL){

                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        
                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);





                    }

                    lineaaux = lineasiguiente;
                }

                return linea;
            }
            else{
                printf("solo acepto > ,< o =\n");
            }
        }
        else if(tipo == TEXT){ // en el caso de que sea tipo texto
            if(strcmp(simbolo,"=")==0){
                while(lineaaux != NULL){ // mientras hayan lineas

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(strcmp(valor,lineaaux->valores[posicion])==0){ // comparamos la cadena de caracteres

                        if(lineaaux == linea && lineaaux->sig == NULL){
                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        
                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);

                        j++;
                    }

                    lineaaux = lineasiguiente;

                }

                return linea;
            }
            else if(strcmp(simbolo,">")==0){
                while(lineaaux != NULL){

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(strcmp(lineaaux->valores[posicion],valor)>0){
                        
                        if(lineaaux == linea && lineaaux->sig == NULL){

                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        
                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);

                    }

                    lineaaux = lineasiguiente;

                }

                return linea;
            }
            else if(strcmp(simbolo,"<")==0){
                while(lineaaux != NULL){

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(strcmp(lineaaux->valores[posicion],valor)<0){
                       
                        if(lineaaux == linea && lineaaux->sig == NULL){

                            linea=NULL;
                            
                        }
                        else if(lineaaux == linea && lineaaux->ant == NULL){

                            linea = linea->sig;
                            lineasiguiente->ant = NULL;
                            lineaaux ->sig = NULL;

                        }
                        else if(lineaaux->sig == NULL){
                            
                            lineaanterior->sig = NULL;
                            lineaaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaaux->sig;
                            lineasiguiente->ant = lineaaux->ant;
                        }

                        
                        for(i=0;i<numcampos;i++){
                            free(lineaaux->valores[i]);
                        }

                        free(lineaaux->valores);
                        free(lineaaux);

                    }

                    lineaaux = lineasiguiente;

                }
                return linea;

            }
            else{
                printf("solo acepto > ,< o =\n");
            }
        }    
    }
}

TABLE* borrar_bd (TABLE *tabla, char *nombre_bd){
    TABLE *tablasiguiente, *tableaux, *tablaanterior;
    DIR *dir;
    int i, aux;
    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";


    if(dir = opendir(nombre_bd)){
    
        if(tabla == NULL)
        {
            printf("Tabla vacia");
            return tabla;
        }

        while(tabla != NULL){

            tableaux = tabla;
            tablaanterior = tableaux->ant;
            tablasiguiente = tableaux->sig;

            strcpy(aux3,nombre_bd);
            strcat(aux3,aux2);
            strcat(aux3,tableaux->nombre);
            strcat(aux3,aux1);
            
            if(tableaux == tabla && tableaux->sig == NULL){
                tabla=NULL;
            }
            else if(tableaux == tabla && tableaux->ant == NULL){

                tabla = tabla->sig;
                tablasiguiente->ant = NULL;
                tableaux ->sig = NULL;

            }
            else if(tableaux->sig == NULL){

                tablaanterior->sig = NULL;
                tableaux->ant = NULL;
            }
            

            for(i=0;i<tableaux->numCampos;i++){

                free(tableaux->campos[i]);
            }

            free(tableaux->tipos);
            free(tableaux->campos);
            free(tableaux);

            remove(aux3);

        }

    }
    return tabla;
}

void mostrar_tablas(TABLE* tabla, char *nombre_bd){

    TABLE *tablaaux = tabla;
    int i, j;
    char aux[10]="";

    if(contar_tablas(nombre_bd) == 0){
        printf("No hay tablas\n");
        return;
    }

    for(i=0;i<contar_tablas(nombre_bd); i++){
        
        printf("%s\n", tablaaux->nombre);

        tablaaux = tablaaux->sig;
    }
}

void mostrar_bd(){

    DIR *d;
    struct dirent *f;
    int j =0;
    

    d = opendir(".");

    if(d == NULL){
        printf("NO hay bases de datos\n");
        return;
    }

    while((f = readdir(d)) != NULL){
        if ((strcmp(f->d_name, ".")!=0) && (strcmp(f->d_name, "..")!=0)){
            
            if(validar_caracteres2(f->d_name) == 0){
                printf("%s\n", f->d_name);
                j++;
            }
            
            //printf("%s\n", f->d_name);
        }
    }

    if(j == 0){
        printf("NO hay bases de datos\n");
        return;
    }

   closedir(d);


}

int FechaValidaAMD (char *cad)
{
    int i, d, m, a;
	int error=0;
	
    d=(cad[8]-'0')*10 +(cad[9]-'0');
    m=(cad[5]-'0')*10 +(cad[6]-'0');
    a=(cad[0]-'0')*1000 +(cad[1]-'0')*100+(cad[2]-'0')*10+ (cad[3]-'0');
    
   for (i=0; i<10; i++)
    {
        if (i!=4 && i!=7)
        {
            if (cad[i]<'0' || cad [i]>'9')
            error=1;
        }
    }
    if ((cad[4]!='/' || cad[7]!='/')) //&& (cad[4]!='-' || cad[7]!='-') añadimos esto por si quieren con guiones tambn
    	error=1;
    if (error==1)
        return 0;
    else if (strlen(cad)!=10)
        return 0;
    else if(d<1 || m<1 || a<1 || d>31 || m>12 || a>9999 )
        return 0;
    else if((m==4 || m==6 || m==9|| m==11) && d>30)
        return 0;
    else if (m==2 && a%4==0 && d>29)
        return 0;
    else if (m==2 && a%4!=0 && d>28)
        return 0;
    else 
		return 1;
}

int FechatoNumeroAMD(char* cad)
{
	int res, dia, mes, anyo;
    dia=(cad[8]-'0')*10 +(cad[9]-'0');
    mes=(cad[5]-'0')*10 +(cad[6]-'0');
    anyo=(cad[0]-'0')*1000 +(cad[1]-'0')*100+(cad[2]-'0')*10+ (cad[3]-'0');
	
	res=dia;

	if (mes==1)
		res=res;
	else if (mes==2)
		res=res+31;
	else if (mes==3 && (anyo%4)!=0)
		res=res+59;
	else if (mes==3 && (anyo%4)==0)
		res=res+60;
	else if (mes==4 && (anyo%4)!=0)
		res=res+90;
	else if (mes==4 && (anyo%4)==0)
		res=res+91;
	else if (mes==5 && (anyo%4)!=0)
		res=res+120;
	else if (mes==5 && (anyo%4)==0)
		res=res+121;
	else if (mes==6 && (anyo%4)!=0)
		res=res+151;
	else if (mes==6 && (anyo%4)==0)
		res=res+152;
	else if (mes==7 && (anyo%4)!=0)
		res=res+181;
	else if (mes==7 && (anyo%4)==0)
		res=res+182;
	else if (mes==8 && (anyo%4)!=0)
		res=res+212;
	else if (mes==8 && (anyo%4)==0)
		res=res+213;
	else if (mes==9 && (anyo%4)!=0)
		res=res+243;
	else if (mes==9 && (anyo%4)==0)
		res=res+244;
	else if (mes==10 && (anyo%4)!=0)
		res=res+273;
	else if (mes==10 && (anyo%4)==0)
		res=res+274;
	else if (mes==11 && (anyo%4)!=0)
		res=res+304;
	else if (mes==11 && (anyo%4)==0)
		res=res+305;
	else if (mes==12 && (anyo%4)!=0)
		res=res+334;
	else if (mes==12 && (anyo%4)==0)
		res=res+335;

	res=(res+(anyo-1)*365);
	res=(res+(anyo-1)/4);


	return res;
}

char* NumerotoFechaAMD(int res)
{
	int anyo, mes=0, dia, bi,i;
	char *fecha;
	float a;

	fecha=malloc(sizeof(char)*10);

	a=0.00273785463;
	anyo=((res)*a);
	bi=((anyo)/4);
	anyo=anyo+(bi/365);
	if (anyo%4!=0)
		res=(res-bi-((anyo-1)*365));
	else if (anyo%4==0)
		res=(res-bi-((anyo-1)*365));


	if (res<=31)
		mes=1;
	else if (res<=59 && (anyo%4)!=0)
	{
		mes=2;
		res=res-31;
	}
	else if (res<=60 && (anyo%4)==0)
	{
		mes=2;
		res=res-31;
	}
	else if (res<=90 && (anyo%4)!=0)
	{
		mes=3;
		res=res-59;
	}
	else if (res<=91 && (anyo%4)==0)
	{
		mes=3;
		res=res-60;
	}
	else if (res<=120 && (anyo%4)!=0)
	{
		mes=4;
		res=res-90;
	}
	else if (res<=121 && (anyo%4)==0)
	{
		mes=4;
		res=res-91;
	}
	else if (res<=151 && (anyo%4)!=0)
	{
		mes=5;
		res=res-120;
	}
	else if (res<=152 && (anyo%4)==0)
	{
		mes=5;
		res=res-121;
	}
	else if (res<=181 && (anyo%4)!=0)
	{
		mes=6;
		res=res-151;
	}
	else if (res<=182 && (anyo%4)==0)
	{
		mes=6;
		res=res-152;
	}
	else if (res<=212 && (anyo%4)!=0)
	{
		mes=7;
		res=res-181;
	}
	else if (res<=213 && (anyo%4)==0)
	{
		mes=7;
		res=res-182;
	}
	else if (res<=243 && (anyo%4)!=0)
	{
		mes=8;
		res=res-212;
	}
	else if (res<=244 && (anyo%4)==0)
	{
		mes=8;
		res=res-213;
	}
	else if (res<=273 && (anyo%4)!=0)
	{
		mes=9;
		res=res-243;
	}
	else if (res<=274 && (anyo%4)==0)
	{
		mes=9;
		res=res-244;
	}
	else if (res<=304 && (anyo%4)!=0)
	{
		mes=10;
		res=res-273;
	}
	else if (res<=305 && (anyo%4)==0)
	{
		mes=10;
		res=res-274;
	}
	else if (res<=334 && (anyo%4)!=0)
	{
		mes=11;
		res=res-304;
	}
	else if (res<=335 && (anyo%4)==0)
	{
		mes=11;
		res=res-305;
	}
	else if (res<=365 && (anyo%4)!=0)
	{
		mes=12;
		res=res-334;
	}
	else if (res<=366 && (anyo%4)==0)
	{
		mes=12;
		res=res-335;
	}
	dia=res;
	if (dia==0)
	{
		dia=31;
		mes=12;
		anyo=anyo-1;
	}
	if (dia==-1)
	{
		dia=30;
		mes=12;
		anyo=anyo-1;
	}

	if(dia<10 && mes<10)
		sprintf(fecha,"%i/0%i/0%i",anyo,mes,dia);
	else if (dia<10 && mes>=10)
		sprintf(fecha,"%i/%i/0%i",anyo,mes,dia);
	else if (dia>=10 && mes<10)
		sprintf(fecha,"%i/0%i/%i",anyo,mes,dia);
	else
		sprintf(fecha,"%i/%i/%i",anyo,mes,dia);

	return fecha;
}

TABLE* renombrar_tabla(TABLE* tabla, char *nombre_bd, char *nombre_tabla, char *nombre_nuevo_tabla){

    DIR *dir;
    TABLE *tablaaux = tabla;
    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";
    char aux4[100]="";
    char aux5[100]="";
    int i;
    

    strcpy(aux3, nombre_bd);
    strcat(aux3,aux2);
    strcat(aux3,nombre_tabla);
    strcat(aux3,aux1);

    if(dir = opendir(nombre_bd)){// entramos al directorio 

        strcpy(aux5,nombre_bd);
        strcat(aux5,aux2);
        strcat(aux5,nombre_nuevo_tabla);
        strcat(aux5,aux1);

        i = entrar_tabla(nombre_bd, nombre_nuevo_tabla);

        if(i == 1){
            
            printf("La tabla: %s no puede actualizarse porque %s ya existe en nuestra bd",nombre_tabla, nombre_nuevo_tabla);
            return tabla;
        }

        if(rename(aux3, aux5) != 0){
            printf("No se pudo renombrar");
            return tabla;
        }
        else{
            
            tablaaux = buscar_tabla(nombre_tabla, tablaaux);
            strcpy(tablaaux->nombre,nombre_nuevo_tabla);
            return tabla;
        }
    }
    else{
        printf("La tabla: %s no fue encontrada\n", nombre_tabla);
        return tabla;
    }
}

int renombrar_BD(char *nombre_bd, char *nuevo_nombre_bd){

    DIR *dir;

    if(dir = opendir(nombre_bd)){// entramos al directorio 

        if(rename(nombre_bd, nuevo_nombre_bd) != 0){
            
            printf("No se pudo renombrar");
            return 0;
        }
        else{
            
            return 2;
        }
    }
    else{
        printf("La db: %s no fue encontrada\n", nombre_bd);
        return 1;
    }

}

ROW* renombrar_linea_valor_deseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion, char *nuevovalor){ 

    int i, j=1, k;
    ROW *lineaaux = linea, *lineasiguiente, *lineaanterior;

    if (linea == NULL){ // por si el fichero estuviese vacio
        printf("No se han encontrado datos\n");
    }
    else{
        printf("hola");
        if(tipo == NUM){ // esto se lo pasamos por parametro
            printf("hola2");
            if(strcmp(simbolo,"=")==0){ // en el caso de que quiera igualdad de numeros
                printf("hola3");
                while(lineaaux != NULL){ // mientras hayan lineas
                    printf("hola4");

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(atof(valor) == atof(lineaaux->valores[posicion])){ // convertimos el texto a numeros reales y los comparamos

                        if(DecimalValidar(nuevovalor)){ // comprobamos q sea un numero el nuevo valor
                            lineaaux->valores[posicion] = strdup(nuevovalor);
                        }
                        else{
                           printf("El nuevo valor no es un numero\n");
                        } 

                    }

                    lineaaux = lineasiguiente;
                }
                return linea;
            }
        }
        else if(tipo == DATE){ // este es el caso de que sea tipo fecha
            printf("hola2");
            if(strcmp(simbolo,"=")==0){
                printf("hola3");
                while(lineaaux != NULL){ // mientras hayan lineas
                    printf("hola4");

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;


                    if(FechatoNumeroDMA(valor) == FechatoNumeroDMA(lineaaux->valores[posicion])){ // convertimos las fechas a numeros para asi poder compararlas
                       
                        if(FechaValidaDMA(nuevovalor)){ // comprobamos q sea un numero el nuevo valor
                            lineaaux->valores[posicion] = strdup(nuevovalor);
                        }
                        else{
                           printf("El nuevo valor no es una fecha\n");
                        } 

                    }

                    lineaaux = lineasiguiente;
                }

                return linea;
            }
        }
        else if(tipo == TEXT){ // en el caso de que sea tipo texto
            if(strcmp(simbolo,"=")==0){
                while(lineaaux != NULL){ // mientras hayan lineas

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(strcmp(valor,lineaaux->valores[posicion])==0){ // comparamos la cadena de caracteres
                        
                        lineaaux->valores[posicion] = strdup(nuevovalor);
                    }

                    lineaaux = lineasiguiente;

                }

                return linea;
            }
        }    
    }
}

int verDuplicados(char **cadena, int num_arg)
{
    int i, j;
    for (i = 0; i < num_arg; i++)
    {
        for (j = i + 1; j < num_arg; j++)
        {
            if (strcmp(cadena[i], cadena[j]) == 0)
            {
                printf("No pueden haber dos campos con el mismo nombre\n");
                return 1;
            }
        }
    }
    return 0;
}

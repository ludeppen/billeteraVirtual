#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"

#define contraseniaParaRegistrarseAdmin "utn2024"

///**/////////////////////////////
///STRUCTS////////////////////////
///**/////////////////////////////

typedef struct {
 char nombreYapellido[50];
 char contrasenia[11];
 char dni[8]; // sin puntos xxxxxxxx
 float dineroIngresado;
 float plata; // comienza en 0
 int edad; // verificar +18
 Pila gastos; // por dia
 int estado; // activo, inactivo
} stCliente;

typedef struct {
 char nombreYapellido[50];
 int edad; //mayor de 18
 char contrasenia[10];
 char dni[8]; // sin puntos xxxxxxxx
 int estado; // activo, inactivo
} stAdmin;

typedef struct {
    char dniOrigen[10];
    char dniDestino[10];
    float monto;
} stTransferencia;

///**/////////////////////////////
///PROTOTIPADO////////////////////
///**/////////////////////////////

///registroClientes
stCliente registroCliente ();
void registroClienteArchivo (char archivoClientes[]);

///registroAdmins
stAdmin registroAdmin ();
void registroAdminArchivo (char archivoAdmins[]);

///verificar
int verificarRepetidosDNI (char archivoClientes[], stCliente cliente);
int verificarRepetidosDNIB (char archivoAdmins[], char dni[]);
int BuscarDniCliente(char archivoCliente[], char dni[]);
int verificarContrasenia(char archivoClientes[], stCliente cliente);
int verificarContraseniaB(char archivoAdmins[], char dni[], char contrasenia[]);
int verificarContraseniaC(char archivoClientes[], char contrsenia[]);
int verificarContraParaRegistrarseAdmin(char *contrasenia);
int verificarEstadoCliente (char archivoClientes[], char dniEleccion[]);
int verificarEstadoAdmin (char archivoAdmins[], char dniEleccion[]);

///inicioSesion
int iniciarSesionCliente(char archivoClientes[], char dniEleccion[]);
int iniciarSesionAdmin (char archivoAdmins[], char dniAdmin[]);
int verificarContraseniaAdministrador(char *contrasenia);

///bajarDatos
void bajarCliente (char archivoClientes[], char dniEleccion[]);
int bajarClienteB(char archivoClientes[], char contrasenia[]);
void bajarAdmin (char archivoAdmins[], char dniEleccion[]);

///AltaDatos
void darDeAltaCliente(char archivoClientes[],char dniEleccion[]);

///mostrar
void mostrarCliente (stCliente cliente);
void mostrarClientePorDni(char archivoClientes[], char dniEleccion[]);
void mostrarClientesArchivo (char archivoClientes[]);
void mostrarClienteDadoDeBaja(char archivoClientes[]);
void mostrarAdmin (stAdmin admin);
void mostrarAdminPorDni(char archivoAdmin[], char dniEleccion[]);
void mostrarAdminsArchivo (char archivoAdmins[]);
void mostrarSaldo(char archivo[], char dniEleccion[]);
int mostrarTransferencias(char archivoTransferencias[], char dni[]);
void mostrarArreglo(char archivocliente[],stCliente arreglo[],int validos, char dni[]);
void mostrarArregloDeArchivo(stCliente arreglo[],int val);
void mostrarArregloDeArchivobaja(stCliente arreglo[],int val);

///modificarCliente
int modificarSaldo(char archivoClientes[], char dniEleccion[], stCliente **clientesDinamico, int *numIngresos);
void transferenciaCliente (char archivoTransferencia[],char archivoClientes[], char dniEleccion[], char dniPasar[], Pila *pilaTransferencia);
void modificarDatosAdmin(char archivoAdmin[],char dni[], char contrasenia[]);
void modificarDatosCliente(char archivo[], char contrasenia[]);

///transferencias
int registrarTransferencia(char archivoTransferencias[], stTransferencia transferencia);
void cancelarUltimaTransferencia(char archivoClientes[],char archivoTransferencias[], Pila *pilaTransferencias);
stTransferencia ultTransferencia(char archivoTransferencias[], int i);

///cantidad de registros
stCliente *leerClientesDesdeArchivo(char archivoClientes[], int *numClientes);

///ordenamientos y recursiva
void intercambiarTransferencias(stTransferencia *a, stTransferencia *b);
void ordenarTransferencias(stTransferencia *transferencias, int izq, int der);
void mostrarTransferenciasOrdenadasRecursiva(char archivoTransferencias[], char dniUsuario[]);
void ordenarNombresCliente(char nombres[][50], int cant);
void OrdenarArchivoClientesNombre(char *archivoCliente);
void pasarClientesOrdenadosAarchivo(char *archivoCliente, stCliente cliente[100], int cant);
int PasarClientesAarreglo(char archivoCliente[] ,stCliente arreglo[]);


///**/////////////////////////////
///MAIN///////////////////////////
///**/////////////////////////////
int main()
{
    int eleccion=0,continuarCliente=0, eleccionCliente=0, eleccionAdmin=0, continuarAdmin=0, resIniciarSesionCliente=0, resIniciarSesionAdmin=0, eleccionRegistro=0, eleccionIS=0, numTransferencias=0, opcionConsultaCliente= 0,numIngresos = 0, opcionVerClientes=0;
    int val;
    char continuar='s',contraniaParaRegistrarseAdmin[10],contraseniaAdmin[10],contraseniaCliente[10], archivoClientes[] = "clientes.dat", archivoAdmins[] = "admins.dat", archivoTransferencias[] = "transferencias.dat",dniEleccion[11],dniAdmin[11],dniPasar[11],opcionCancelarTrans = 's';
    char arreglo[20][100];
    Pila pilaTransferencias;inicpila(&pilaTransferencias);
    stCliente cliente;
    stAdmin admin;
    stCliente *clientesDinamico=NULL;


    do{
    printf("   =========MENU=========\n");
    printf("1. Registrarse como usuario.\n");
    printf("2. Registrarse como administrador.\n");
    printf("3. Iniciar sesion como usuario.\n");
    printf("4. Iniciar sesion como administrador.\n");
    printf("5. Salir.\n");
    scanf("%d", &eleccion);
    switch(eleccion)
    {

    ///REGISTRO//////////////////////////////////////////////////
    case 1:
        registroClienteArchivo(archivoClientes);
        break;
    case 2:
        printf("Ingrese la contrasenia:\n");
        fflush(stdin);
        gets(contraniaParaRegistrarseAdmin);
        int res = verificarContraParaRegistrarseAdmin(contraniaParaRegistrarseAdmin);
        if(res == 1)
        {
          registroAdminArchivo(archivoAdmins);
        }
        else
        {
            printf("Contrasenia incorrecta.\n");
        }
        break;
    ///INICIO SESION//////////////////////////////////////////////////
    case 3:
        ///CLIENTE//////////////////////////////////////////////////
                resIniciarSesionCliente=iniciarSesionCliente(archivoClientes,dniEleccion);

                if (resIniciarSesionCliente==0)
                {
                        printf("No se pudo iniciar sesion.\n");
                }
                else{
                        do{
                            system("cls");
                            printf("========MENU CLIENTE========\n");
                            mostrarSaldo(archivoClientes,dniEleccion);
                            printf("1. Ingresar dinero.\n");
                            printf("2. Transferir dinero.\n");
                            printf("3. Cancelar ultima Transferencia.\n");
                            printf("4. Mostrar informacion.\n");
                            printf("5. Modificar informacion.\n");
                            printf("6. Ver resumen hasta la fecha.\n");
                            printf("7. Darme de baja.\n");
                            printf("8. Salir.\n");
                            scanf("%d", &eleccionCliente);

                        switch(eleccionCliente){
                        case 1:
                             modificarSaldo(archivoClientes, dniEleccion,&clientesDinamico,&numIngresos);
                            break;
                        case 2:
                            printf("Ingrese el dni a realizar transferencia.\n");
                            fflush(stdin);
                            gets(dniPasar);
                            int flag =  BuscarDniCliente(archivoClientes,dniPasar);
                            if(flag == 1)
                            {
                              transferenciaCliente(archivoTransferencias,archivoClientes, dniEleccion, dniPasar,&pilaTransferencias);
                            }
                            else
                            {
                                printf("Dni no registrado.\n");
                            }
                            break;
                        case 3:
                            printf("Esta seguro que desea cancelar su ultima transferencia? ( S / N ):\n");
                            fflush(stdin);
                            scanf("%c",&opcionCancelarTrans);
                            if(opcionCancelarTrans == 's')
                            {
                              cancelarUltimaTransferencia(archivoClientes,archivoTransferencias,&pilaTransferencias);
                            }
                            else
                            {
                                break;
                            }
                            break;
                        case 4:
                            mostrarClientePorDni(archivoClientes, dniEleccion);
                            break;
                        case 5:
                             printf("Ingrese su contrasenia para modificar datos:\n");
                             fflush(stdin);
                             gets(contraseniaCliente);

                             modificarDatosCliente(archivoClientes,contraseniaCliente);
                             break;
                        case 6:
                             printf("1. ver transferencias.\n");
                             printf("2. ver depositos realizados en el dia.\n");
                             fflush(stdin);
                             scanf("%d",&opcionConsultaCliente);
                             switch(opcionConsultaCliente)
                             {
                             case 1:
                                  mostrarTransferenciasOrdenadasRecursiva(archivoTransferencias,dniEleccion);
                                  break;
                             case 2:
                                  mostrarArreglo(archivoClientes,clientesDinamico,numIngresos, dniEleccion);
                                  break;
                             default:
                                  printf("Opcion no valida.\n");
                                  break;
                             }
                             break;
                        case 7:
                             printf("Ingrese su contrasenia para darse de baja (si se da de baja debera llamar al 0800-428-7627 o registrarse nuevamente para iniciar sesion):\n");
                             fflush(stdin);
                             gets(contraseniaCliente);
                             int encontrado = bajarClienteB(archivoClientes,contraseniaCliente);
                             if(encontrado == 1)
                             {
                               return 0;
                             }
                             break;
                        case 8:
                             printf("Adios.\n");
                             return 0;
                             break;
                        }
                            printf("Desea continuar como cliente? S/N\n");
                            fflush(stdin);
                            scanf(" %c", &continuarCliente);
                        }while (continuarCliente == 's');
                }
        break;
    case 4:
        ///ADMIN//////////////////////////////////////////////////
                resIniciarSesionAdmin = iniciarSesionAdmin (archivoAdmins,dniAdmin);
                if (resIniciarSesionAdmin==1)
                {
                        do{
                            system("cls");
                            printf("=======MENU ADMIN=======\n");
                            printf("1. Dar cliente de baja.\n");
                            printf("2. Dar cliente de alta.\n");
                            printf("3. Mostrar clientes.\n");
                            printf("4. Mostrar admins.\n");
                            puts("\n======Sobre mi======\n");
                            printf("5. Mis datos.\n");
                            printf("6. Modificar mis datos.\n");
                            printf("7. Darme de baja.\n");
                            printf("8. Salir.\n");
                            scanf("%d", &eleccionAdmin);

                        switch(eleccionAdmin)
                        {
                        case 1:
                            printf("Ingrese el dni del cliente a dar de baja.\n");
                            fflush(stdin);
                            gets(dniEleccion);
                            bajarCliente(archivoClientes, dniEleccion);
                            printf("Cliente dado de baja con exito!.\n");
                            break;
                        case 2:
                            printf("Ingrese el dni del cliente a dar de alta.\n");
                            fflush(stdin);
                            gets(dniEleccion);
                            darDeAltaCliente(archivoClientes,dniEleccion);
                            printf("Cliente dado de alta con exito!.\n");
                            break;
                        case 3:
                            printf("Ver clientes habilitados(1) o ver clientes dados de baja(2)?:\n");
                            fflush(stdin);
                            scanf("%d",&opcionVerClientes);
                            switch(opcionVerClientes)
                            {
                            case 1:
                                val =PasarClientesAarreglo(archivoClientes,arreglo);
                                mostrarArregloDeArchivoAlta(arreglo,val);
                                 break;
                            case 2:
                                 val =PasarClientesAarreglo(archivoClientes,arreglo);
                                 mostrarArregloDeArchivobaja(arreglo,val);
                                 break;
                            default:
                                 printf("Opcion no valida.\n");
                                 break;
                            }
                            break;
                        case 4:
                            mostrarAdminsArchivo(archivoAdmins);
                            break;
                        case 5:
                            mostrarAdminPorDni(archivoAdmins,dniAdmin);
                            break;
                        case 6:
                             printf("Ingrese su contrasenia para modificar datos:\n");
                             fflush(stdin);
                             gets(contraseniaAdmin);

                             modificarDatosAdmin(archivoAdmins,dniAdmin,contraseniaAdmin);
                            break;
                        case 7:
                            bajarAdmin(archivoAdmins,dniAdmin);
                            printf("Usted ha sido de baja con exito!.\n");
                            break;
                        case 8:
                            printf("Adios.\n");
                            return 0;
                            break;
                        }
                            printf("Desea continuar como admin? S/N\n");
                            fflush(stdin);
                            scanf(" %c", &continuarAdmin);
                        }while (continuarAdmin == 's');
                   }
                   else
                   {
                      printf("No se pudo iniciar sesion.\n");
                   }
                   break;
    case 5:
         printf("Adios.\n");
         return 0;
         break;
    default:
        printf("Opcion no valida.\n");
        break;
    }
                printf("Desea continuar? S/N\n");
                fflush(stdin);
                scanf(" %c", &continuar);
                }while (continuar=='s');
return 0;
}

///**/////////////////////////////
///FUNCIONES//////////////////////
///**/////////////////////////////

///registroClientes
stCliente registroCliente ()
{
 stCliente cliente;

    ///NOMBRE Y APELLIDO///
    printf("Ingrese su nombre y apellido:\n");
    fflush(stdin);
    gets(cliente.nombreYapellido);

    ///CONTRASENIA///

    do
    {
    printf("Ingrese su contrasenia. Debe ser de maximo 10 caracteres.\n");
    fflush(stdin);
    gets(cliente.contrasenia);
    if (strlen(cliente.contrasenia)>10)
    {
         printf("La contraseña debe contener maximo 10 caracteres.\n");
    }
    } while(strlen(cliente.contrasenia)>10);

    ///EDAD///
    do
    {
    printf("Ingrese su edad. Debe ser mayor de 18 anios.\n");
    scanf("%d",&cliente.edad);
    if (cliente.edad<18)
    {
        printf("Debe ser mayor de 18 anios para registrarse.\n");
    }
    else if(cliente.edad > 100)
    {
        printf("Debe ser menor a 100 años para registrarse.\n");
    }
    }while(cliente.edad<18 || cliente.edad > 100);
     ///DNI///
    do
    {
    printf("Ingrese su dni. Sin puntos, solo numeros.\n");
    fflush(stdin);
    gets(cliente.dni);
    if ((strlen(cliente.dni)>8) || (strlen(cliente.dni) < 7))
    {
        printf("El dni debe contener 7 u 8 numeros.\n");
    }
    }while((strlen(cliente.dni) > 8) || (strlen(cliente.dni) < 7));

    ///PLATA///
    cliente.plata = 0;

    ///ESTADO///
    cliente.estado = 1;

    return cliente;
}

void registroClienteArchivo (char archivoClientes[])
{
    FILE * fp = fopen (archivoClientes, "ab");
    stCliente cliente;
    int resVerificarRepetidosDNI=0;
    if (fp) {
        cliente=registroCliente();
        resVerificarRepetidosDNI=verificarRepetidosDNI(archivoClientes, cliente);

        if (resVerificarRepetidosDNI!=0)
        {
            printf("No fue posible crear el cliente ya que ya tiene una cuenta.\n");
        }
        else
        {
            fwrite(&cliente, sizeof(stCliente), 1, fp);
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para registrar el cliente.\n");
    }
}

///registroAdmins
stAdmin registroAdmin()
{
  stAdmin admin;

    ///NOMBRE Y APELLIDO
    printf("Ingrese su nombre y apellido.\n");
    fflush(stdin);
    gets(admin.nombreYapellido);

    ///EDAD
    do
    {
    printf("Ingrese su edad. Debe ser mayor de 18 anios.\n");
    scanf("%d",&admin.edad);
    if (admin.edad<18)
    {
        printf("Debe ser mayor de 18 anios para registrarse.\n");
    }
    else if(admin.edad > 100)
    {
        printf("Debe ser menor a 100 años para registrarse.\n");
    }
    }while(admin.edad<18 || admin.edad > 100);

    ///CONTRASENIA///
    do{
    printf("Ingrese su contrasenia. Debe ser de maximo 10 caracteres.\n");
    fflush(stdin);
    gets(admin.contrasenia);
    if (strlen(admin.contrasenia)>10)
    {
         printf("La contraseña debe contener maximo 10 caracteres.\n");
    }
    } while(strlen(admin.contrasenia)>10);

     ///DNI///
    do
    {
    printf("Ingrese su dni. Sin puntos, solo numeros.\n");
    fflush(stdin);
    gets(admin.dni);
    if ((strlen(admin.dni)>8) || (strlen(admin.dni) < 7))
    {
        printf("El dni debe contener 7 u 8 numeros.\n");
    }
    }while((strlen(admin.dni) > 8) || (strlen(admin.dni) < 7));

    ///ESTADO///
    admin.estado = 1;
return admin;
}

void registroAdminArchivo (char archivoAdmins[])
{
    FILE * fp = fopen (archivoAdmins, "ab");
    stAdmin admin;
    int resVerificarRepetidosDNI=0;
    if (fp)
    {
        admin=registroAdmin();

        resVerificarRepetidosDNI=verificarRepetidosDNIB(archivoAdmins, admin.dni);
        if (resVerificarRepetidosDNI!=0)
        {
             printf("No fue posible crear el admin ya que ya tiene una cuenta.\n");
        }
        else
        {
            fwrite(&admin, sizeof(stAdmin),1,fp);
        }
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo para registrar el admin.\n");
    }
}

///verificar
int verificarRepetidosDNI (char archivoClientes[], stCliente cliente)
{
    FILE * fp = fopen (archivoClientes, "rb");
    stCliente aux;
    int flag=0;
    if (fp){
        while(fread(&aux, sizeof(stCliente), 1, fp) != 0)
        {
            if (strcmp (cliente.dni, aux.dni) == 0)
            {
                flag=1;
                break;
            }
        }
        fclose(fp);
    }
    else{
        printf("El archivo no se pudo abrir para comparar los dni.\n");
    }
    return flag;
}

int verificarRepetidosDNIB (char archivoAdmins[], char dni[])
{
    FILE * fp = fopen (archivoAdmins, "rb");
    stAdmin aux;
    int flag=0;
    if (fp)
    {
        while(fread(&aux, sizeof(stAdmin), 1, fp) > 0)
        {
            if (strcmp (dni, aux.dni) == 0)
            {
                flag=1;
                break;
            }
        }
        fclose(fp);
    }
    else{
        printf("El archivo no se pudo abrir para comparar los dni.\n");
    }
return flag;
}

int BuscarDniCliente(char archivoCliente[], char dni[])
{
    FILE * fp = fopen (archivoCliente, "rb");
    stCliente aux;
    int flag=0;
    if (fp)
    {
        while(fread(&aux, sizeof(stCliente), 1, fp) > 0)
        {
            if (strcmp (dni, aux.dni) == 0)
            {
                flag=1;
                break;
            }
        }
        fclose(fp);
    }
    else{
        printf("El archivo no se pudo abrir para comparar los dni.\n");
    }
return flag;
}
int verificarContrasenia(char archivoClientes[], stCliente cliente)
{
    FILE * fp = fopen (archivoClientes, "rb");
    stCliente aux;
    int flag=0;
    if (fp){
        while(fread(&aux, sizeof(stCliente), 1, fp) != 0) {
            if (strcmp(cliente.dni, aux.dni) == 0 && strcmp(cliente.contrasenia, aux.contrasenia) == 0)
            {
                flag=1;
                break;
            }
        }
        fclose(fp);
    }
    else{
        printf("El archivo no se pudo abrir para comparar las contrasenias.\n");
    }
    return flag;
}

int verificarContraseniaB(char archivoAdmins[], char dni[], char contrasenia[])
{
    FILE *fp = fopen(archivoAdmins, "rb");
    stAdmin aux;
    int flag = 0;

    if (fp)
    {
        while (fread(&aux, sizeof(stAdmin), 1, fp) > 0)
        {
            if (strcmp(dni, aux.dni) == 0 && strcmp(contrasenia, aux.contrasenia) == 0)
            {
                flag = 1;
                break;
            }
        }
        fclose(fp);
    } else {
        printf("El archivo no se pudo abrir para comparar las contraseñas.\n");
    }

return flag;
}

int verificarContraParaRegistrarseAdmin(char *contrasenia)
{
    if(strcmp(contrasenia, contraseniaParaRegistrarseAdmin) == 0)
    {
        return 1; // Contraseña correcta
    }
return 0; // Contraseña incorrecta
}

int verificarEstadoCliente (char archivoClientes[], char dniEleccion[])
{
    FILE * fp = fopen (archivoClientes, "rb");
    stCliente cliente;
    int flag=0;
    if (fp){
            while (fread(&cliente, sizeof(stCliente), 1, fp) != 0)
            {
                 if (strcmp(cliente.dni, dniEleccion) == 0)
                 {
                    if (cliente.estado==1)
                    {
                        flag=1;
                        break;
                    }
                }
            }
            fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para verificar el estado del cliente.\n");
    }
    return flag;
}

int verificarEstadoAdmin (char archivoAdmins[], char dniEleccion[])
{
    FILE * fp = fopen (archivoAdmins, "rb");
    stAdmin admin;
    int flag=0;
    if (fp){
            while (fread(&admin, sizeof(stAdmin), 1, fp) != 0)
            {
                 if (strcmp(admin.dni, dniEleccion) ==0 )
                 {
                    if (admin.estado==1)
                    {
                        flag=1;
                        break;
                    }
                }
            }
            fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para verificar el estado del admin.\n");
    }
    return flag;
}

///inicioSesion
int iniciarSesionCliente(char archivoClientes[],char dniEleccion[])
{
    FILE * fp = fopen(archivoClientes, "rb");

    int inicioSesion=0, resVerificarRepetidosDNI=0,resVerificarContrasenia=0, resVerificarEstadoCliente=0;
    stCliente cliente;

    printf("Ingrese su DNI.\n");
    fflush(stdin);
    gets(cliente.dni);

    resVerificarEstadoCliente=verificarEstadoCliente(archivoClientes, cliente.dni);
    if(resVerificarEstadoCliente==1)
    {
      if (fp)
      {
      resVerificarRepetidosDNI=verificarRepetidosDNI(archivoClientes, cliente);

       if (resVerificarRepetidosDNI==1)
       {
            printf("Ingrese su contrasenia.\n");
            fflush(stdin);
            gets(cliente.contrasenia);
            resVerificarContrasenia=verificarContrasenia(archivoClientes, cliente);
            if (resVerificarContrasenia == 1)
            {
                inicioSesion=1;
            }
        }
        strcpy(dniEleccion,cliente.dni);
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para iniciar sesion del cliente.\n");
    }
    }
    else{
        printf("No existe una cuenta cliente con ese dni.\n");
    }
    return inicioSesion;
}

int iniciarSesionAdmin (char archivoAdmins[], char dniAdmin[])
{
    FILE * fp = fopen(archivoAdmins, "rb");
    int inicioSesion=0, resVerificarRepetidosDNI=0,resVerificarContrasenia=0,resVerificarEstadoAdmin=0;
    stAdmin admin;

    printf("Ingrese su DNI.\n");
    fflush(stdin);
    gets(admin.dni);

    if (fp)
    {
      resVerificarRepetidosDNI=verificarRepetidosDNIB(archivoAdmins, admin.dni);

      if(resVerificarRepetidosDNI==1)
      {
         resVerificarEstadoAdmin = verificarEstadoAdmin(archivoAdmins, admin.dni);

         if (resVerificarEstadoAdmin==1)
         {
           printf("Ingrese su contrasenia.\n");
           fflush(stdin);
           gets(admin.contrasenia);

           resVerificarContrasenia=verificarContraseniaB(archivoAdmins, admin.dni,admin.contrasenia);

           if (resVerificarContrasenia == 1)
           {
            inicioSesion=1;
           }
           else
           {
               printf("Contrasenia incorrecta.\n");
           }
         }
         else
         {
            printf("Administrador dado de baja, no se puede iniciar sesion.\n");
         }
     }
     else
     {
       printf("No existe una cuenta admin con ese dni.\n");
     }

     strcpy(dniAdmin,admin.dni);
     fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo para iniciar sesion del admin.\n");
    }
return inicioSesion;
}

///bajarDato
void bajarCliente (char archivoClientes[], char dniEleccion[])
{
    FILE * fp = fopen (archivoClientes, "r+b");
    stCliente cliente;
    if (fp)
    {
            while (fread(&cliente, sizeof(stCliente), 1, fp) !=0)
            {
                if (strcmp(cliente.dni, dniEleccion) == 0)
                {
                        cliente.estado=0;
                        fseek(fp, -sizeof(stCliente), SEEK_CUR);
                        fwrite(&cliente, sizeof(stCliente), 1, fp);
                        break;

                }
            }
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo para dar de baja el cliente.\n");
    }
}

int bajarClienteB(char archivoClientes[], char contrasenia[])
{
   FILE *fp = fopen(archivoClientes, "r+b");
   stCliente cliente;
   int encontrado = 0;
    if (fp)
    {
        while (fread(&cliente, sizeof(stCliente), 1, fp) > 0)
        {
            if (strcmp(cliente.contrasenia, contrasenia) == 0)
            {
                cliente.estado=0;
                fseek(fp, -sizeof(stCliente), SEEK_CUR);
                fwrite(&cliente, sizeof(stCliente), 1, fp);
                encontrado = 1;
                printf("Su cuenta ha sido dada de baja con exito.\n");
                break;
            }
        }
        if (!encontrado)
        {
            printf("Contraseña incorrecta. No se pudo dar de baja al cliente.\n");
        }

        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo para dar de baja el cliente.\n");
    }
return encontrado;
}

void bajarAdmin (char archivoAdmins[], char dniEleccion[])
{
    FILE * fp = fopen (archivoAdmins, "r+b");
    stAdmin admin;
    if (fp)
    {
            while (fread(&admin, sizeof(stAdmin), 1, fp) !=0)
            {
                if (strcmp(admin.dni, dniEleccion) == 0)
                {
                        admin.estado=0;
                        strcpy(admin.dni," ");
                        fseek(fp, -sizeof(stAdmin), SEEK_CUR);
                        fwrite(&admin, sizeof(stAdmin), 1, fp);
                        break;
                }
            }
        fclose(fp);
        }
    else
    {
        printf("No se pudo abrir el archivo para dar de baja el admin.\n");
    }
}

void darDeAltaCliente(char archivoClientes[],char dniEleccion[])
{
    FILE * fp = fopen (archivoClientes, "r+b");
    stCliente cliente;
    if (fp)
    {
            while (fread(&cliente, sizeof(stCliente), 1, fp) !=0)
            {
                if (strcmp(cliente.dni, dniEleccion) == 0)
                {
                    if(cliente.estado == 0)
                    {
                        cliente.estado=1;
                        strcpy(cliente.dni,dniEleccion);
                        fseek(fp, -sizeof(stCliente), SEEK_CUR);
                        fwrite(&cliente, sizeof(stCliente), 1, fp);
                        break;
                    }
                }
            }
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo para dar de baja el cliente.\n");
    }
}

///mostrar
void mostrarCliente (stCliente cliente)
{
    printf("Nombre y apellido: %s\n",cliente.nombreYapellido);
    printf("DNI: %s.\n", cliente.dni);
    printf("Edad: %d.\n", cliente.edad);
    if(cliente.estado == 1)
    {
    printf("Habilitado.\n");
    }
    else
    {
        printf("Inhabilitado.\n");
    }
    printf("Dinero: %.2f.\n", cliente.plata);
    puts("-------------------------------------------\n");
}
void mostrarClientePorDni(char archivoClientes[], char dniEleccion[])
{
    FILE * fp = fopen (archivoClientes, "rb");
    stCliente cliente;
    if(fp)
    {
        while(fread(&cliente, sizeof(stCliente), 1, fp)!=0){
            if (strcmp(cliente.dni, dniEleccion) == 0){
                    mostrarCliente(cliente);
        }
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para mostrar un cliente.\n");
    }
}
void mostrarClientesArchivo (char archivoClientes[]){

    FILE * fp = fopen (archivoClientes, "rb");
    stCliente cliente;
    if (fp){
        while(fread(&cliente, sizeof(stCliente), 1, fp)!= 0){
         if(cliente.estado == 1)
         {
            mostrarCliente(cliente);
         }
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para mostrar el cliente.\n");
    }
}

void mostrarClienteDadoDeBaja(char archivoClientes[])
{
  FILE * fp = fopen (archivoClientes, "rb");
    stCliente cliente;
    if (fp)
    {
        while(fread(&cliente, sizeof(stCliente), 1, fp)!= 0)
        {
         if(cliente.estado == 0)
         {
            mostrarCliente(cliente);
         }
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para mostrar el cliente.\n");
    }
}
void mostrarAdmin (stAdmin admin)
{
    printf("Nombre: %s\n",admin.nombreYapellido);
    printf("Edad: %d\n",admin.edad);
    printf("DNI: %s\n", admin.dni);
    printf("Estado %d\n", admin.estado);
    printf("Contrasenia %s\n", admin.contrasenia);
    puts("-------------------------------");
}
void mostrarAdminPorDni(char archivoAdmin[], char dniEleccion[])
{
    FILE * fp = fopen (archivoAdmin, "rb");
    stAdmin admin;
    if(fp)
    {
        while(fread(&admin, sizeof(stAdmin), 1, fp)!=0)
        {
            if (strcmp(admin.dni, dniEleccion) == 0)
            {
                    mostrarAdmin(admin);
            }
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para mostrar un cliente.\n");
    }
}
void mostrarAdminsArchivo (char archivoAdmins[]){

    FILE * fp = fopen (archivoAdmins, "rb");
    stAdmin admin;
    if (fp){
        while(fread(&admin, sizeof(stAdmin), 1, fp)!= 0){
            mostrarAdmin(admin);
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para mostrar el admin.\n");
    }
}
void mostrarSaldo(char archivo[], char dniEleccion[])
{
    stCliente cliente;
    FILE*fp = fopen(archivo,"rb");
    if(fp)
    {
     while(fread(&cliente,sizeof(stCliente),1,fp) != 0)
     {
      if(strcmp(cliente.dni,dniEleccion) == 0)
      {
      puts("----------------------------");
      printf("Su saldo actual es: $%.2f\n", cliente.plata);
      puts("----------------------------");
      }
     }
    fclose(fp);
    }
}

void mostrarArreglo(char archivocliente[],stCliente arreglo[],int validos, char dni[])
{
    FILE * fp = fopen (archivocliente, "rb");
    stCliente cliente;
    if(fp)
    {

        while(fread(&cliente, sizeof(stCliente), 1, fp)!=0)
        {
            if (strcmp(cliente.dni, dni) == 0)
            {
                    for(int i = 0; i < validos;i++)
                    {
                     printf("Saldo: %.2f\n", arreglo[i].dineroIngresado);
                     printf("--------------------------------------------\n");
                    }
            }
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para mostrar un cliente.\n");
    }
}

///modificarCliente
int modificarSaldo(char archivoClientes[], char dniEleccion[], stCliente **clientesDinamico, int *numIngresos)
{
    FILE * fp = fopen (archivoClientes, "r+b");
    stCliente cliente;
    float cantDinero, suma;
    int cantIngresos;
    if (fp){
        while(fread(&cliente, sizeof(stCliente), 1, fp)!= 0)
        {
            if (strcmp(dniEleccion, cliente.dni)==0)
            {
                printf("Ingrese la cantidad de dinero a ingresar.\n");
                scanf("%f", &cantDinero);

                suma = cliente.plata + cantDinero;
                cliente.plata= suma;

                fseek(fp, -sizeof(stCliente), SEEK_CUR);
                fwrite(&cliente, sizeof(stCliente), 1, fp);

                if (clientesDinamico != NULL)
                {
                    *clientesDinamico = (stCliente *) realloc(*clientesDinamico, (*numIngresos + 1) * sizeof(stCliente));
                    cliente.dineroIngresado = cantDinero;
                    (*clientesDinamico)[*numIngresos] = cliente;
                    (*numIngresos)++;
                }
                break;
            }
        }
        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para modificar el cliente.\n");
    }
}

void modificarDatosCliente(char archivo[], char contrasenia[])
{
  stCliente cliente;
    FILE*fp = fopen(archivo, "r + b");
    if(fp)
    {
        int encontrado = 0;
        int pos = ftell(fp); // posición actual del archivo
        while(fread(&cliente, sizeof(stCliente), 1, fp) == 1)
        {
            if(strcmp(cliente.contrasenia, contrasenia) == 0)
            {
                encontrado = 1;
                char control = 's';
                while(control == 's')
                {
                    int opcion;
                    printf("Que desea modificar?:\n");
                    printf("1. Nombre y apellido.\n");
                    printf("2. Edad.\n");
                    printf("3. contrasenia.\n");
                    scanf("%d",&opcion);
                    switch(opcion)
                    {
                    case 1:
                        printf("Ingrese el nuevo nombre y apellido:\n");
                        fflush(stdin);
                        gets(cliente.nombreYapellido);
                        break;
                    case 2:
                        printf("Ingrese la nueva edad:\n");
                        scanf("%d",&cliente.edad);
                        break;
                    case 3:
                        printf("Ingrese la nueva contrasenia:\n");
                        fflush(stdin);
                        gets(cliente.contrasenia);
                        break;
                    default:
                        printf("error.\n");
                        break;
                    }
                    printf("\nDesea modificar otro dato? S/N \n");
                    fflush(stdin);
                    scanf(" %c",&control);
                }
                fseek(fp, pos, SEEK_SET); // regresar a la posición del registro encontrado
                fwrite(&cliente, sizeof(stCliente), 1, fp);
                break;
            }
            pos = ftell(fp); // actualizar la posición actual del archivo
        }
        if(!encontrado)
        {
            printf("Contrasenia incorrecta.\n");
        }
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo para modificar los datos del cliente.\n");
    }
}

void modificarDatosAdmin(char archivoAdmin[],char dni[], char contrasenia[])
{
  stAdmin admin;
    FILE*fp = fopen(archivoAdmin, "r + b");
    if(fp)
    {
        int encontrado = 0;
        int pos = ftell(fp); // posición actual del archivo
        while(fread(&admin, sizeof(stAdmin), 1, fp) == 1)
        {
            if(strcmp(admin.contrasenia, contrasenia) == 0 && strcmp(admin.dni,dni) == 0)
            {
                encontrado = 1;
                char control = 's';
                while(control == 's')
                {
                    int opcion;
                    printf("Que desea modificar?:\n");
                    printf("1. Nombre y apellido.\n");
                    printf("2. Edad.\n");
                    printf("3. contrasenia.\n");
                    scanf("%d",&opcion);
                    switch(opcion)
                    {
                    case 1:
                        printf("Ingrese el nuevo nombre y apellido:\n");
                        fflush(stdin);
                        gets(admin.nombreYapellido);
                        break;
                    case 2:
                        printf("Ingrese la nueva edad:\n");
                        scanf("%d",&admin.edad);
                        break;
                    case 3:
                        printf("Ingrese la nueva contrasenia:\n");
                        fflush(stdin);
                        gets(admin.contrasenia);
                        break;
                    default:
                        printf("error.\n");
                        break;
                    }
                    printf("\nDesea modificar otro dato? S/N \n");
                    fflush(stdin);
                    scanf(" %c",&control);
                }
                fseek(fp, pos, SEEK_SET); // regresa a la posición del registro encontrado
                fwrite(&admin, sizeof(stAdmin), 1, fp);
                break;
            }
            pos = ftell(fp); // actualizar la posición actual del archivo
        }
        if(encontrado == 0)
        {
            printf("Contrasenia incorrecta.\n");
        }
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo para modificar los datos del cliente.\n");
    }
}

///TRANSFERENCIA
void transferenciaCliente (char archivoTransferencia[],char archivoClientes[], char dniEleccion[], char dniPasar[], Pila *pilaTransferencia)
{

    FILE * fp = fopen (archivoClientes, "r+b");
    stCliente cliente;
    stTransferencia transferencia;
    float cantDinero=0;
    int res=0, i = 0;
    if (fp){

         while(fread(&cliente, sizeof(stCliente), 1, fp)!= 0)
         {
            if (strcmp(dniEleccion, cliente.dni)==0)
            {
                while(cantDinero<=0){
                printf("Ingrese la cantidad de dinero a transferir.\n");
                scanf("%f", &cantDinero);
                }
                if (cantDinero<cliente.plata) //Comprueba si hay plata suficiente para pasar
                {
                cliente.plata=cliente.plata-cantDinero;
                fseek(fp, -sizeof(stCliente), SEEK_CUR);
                fwrite(&cliente, sizeof(stCliente), 1, fp);

                strcpy(transferencia.dniOrigen, dniEleccion);
                strcpy(transferencia.dniDestino, dniPasar);
                transferencia.monto = cantDinero;
                i =  registrarTransferencia(archivoTransferencia,transferencia);

                apilar(pilaTransferencia,i);
                printf("Transferencia realizada con exito!.\n");
                break;

                }
                else
                {
                    printf("No dispone de fondos suficientes!\n");
                    return; // sale en caso de no tener
                }
                }
          }
                rewind(fp);

                while(fread(&cliente, sizeof(stCliente), 1, fp)!= 0){
                  if (strcmp(dniPasar, cliente.dni)==0){
                    cliente.plata = cliente.plata + cantDinero;
                    fseek(fp, -sizeof(stCliente), SEEK_CUR);
                    fwrite(&cliente, sizeof(stCliente), 1, fp);
                    break;
                }
             }

        fclose(fp);
    }
    else{
        printf("No se pudo abrir el archivo para transferir.\n");
    }
}
stTransferencia ultTransferencia(char archivoTransferencias[], int i)
{
    FILE *fp = fopen(archivoTransferencias, "rb");
    stTransferencia transferencia;

    if (fp)
    {
        fseek(fp, i * sizeof(stTransferencia), SEEK_SET);
        fread(&transferencia, sizeof(stTransferencia), 1, fp);
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo de transferencias para obtener la transferencia.\n");
    }

return transferencia;
}
void cancelarUltimaTransferencia(char archivoClientes[],char archivoTransferencias[], Pila *pilaTransferencias)
{
    stCliente cliente;
    FILE *fp = fopen(archivoClientes, "r+b");
    int i = desapilar(pilaTransferencias);
    stTransferencia ultimaTransferencia = ultTransferencia(archivoTransferencias, i);

    if (fp)
    {
        while (fread(&cliente, sizeof(stCliente), 1, fp) != 0)
        {
            if (strcmp(ultimaTransferencia.dniOrigen, cliente.dni) == 0)
            {
                cliente.plata += ultimaTransferencia.monto;
                fseek(fp, -sizeof(stCliente), SEEK_CUR);
                fwrite(&cliente, sizeof(stCliente), 1, fp);
                break;
            }
        }
         rewind(fp);

        while (fread(&cliente, sizeof(stCliente), 1, fp) != 0)
        {
            if (strcmp(ultimaTransferencia.dniDestino, cliente.dni) == 0)
            {
                cliente.plata -= ultimaTransferencia.monto;
                fseek(fp, -sizeof(stCliente), SEEK_CUR);
                fwrite(&cliente, sizeof(stCliente), 1, fp);
                break;
            }
        }
      fclose(fp);
      printf("Su transferencia ha sido cancelada con exito.\n");
    }
    else
    {
       printf("No se pudo abrir el archivo.\n");
    }

}
int registrarTransferencia(char archivoTransferencias[], stTransferencia transferencia)
{
    FILE *fp = fopen(archivoTransferencias, "ab");
    int i = 0;
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        i = ftell(fp) / sizeof(stTransferencia);
        fwrite(&transferencia,sizeof(stTransferencia),1,fp);
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo de transferencias para registrar la transferencia.\n");
    }
return i;
}

int mostrarTransferencias(char archivoTransferencias[], char dni[])
{
    FILE *fp = fopen(archivoTransferencias, "rb");
    stTransferencia transferencia;
    int cantidad = 0;

    if (fp)
    {

        printf("Transferencias realizadas por el cliente con DNI %s:\n", dni);
        while (fread(&transferencia, sizeof(stTransferencia), 1, fp) != 0)
        {
               if (strcmp(transferencia.dniOrigen,dni) == 0)
               {
                printf("Origen: %s\n", transferencia.dniOrigen);
                printf("Destino: %s\n", transferencia.dniDestino);
                printf("Monto: %.2f\n", transferencia.monto);
                puts("------------------------------------");
                cantidad++;
               }

        }
        if (cantidad == 0)
        {
            printf("No se encontraron transferencias realizadas por el cliente con DNI %s.\n", dni);
        }
        fclose(fp);
    }
    else
    {
        printf("No se pudo abrir el archivo de transferencias.\n");
    }
return cantidad;
}

///ORDENAMIENTOS
///recursiva
void intercambiarTransferencias(stTransferencia *a, stTransferencia *b)
{
    stTransferencia aux = *a;
    *a = *b;
    *b = aux;
}

void ordenarTransferencias(stTransferencia *transferencias, int izq, int der)
{
    if (izq < der)
    {
        float ultPos = transferencias[der].monto;
        int i = izq - 1;

        for (int j = izq; j <= der - 1; j++)
        {
            if (transferencias[j].monto >= ultPos)
            {
                i++;
                intercambiarTransferencias(&transferencias[i], &transferencias[j]);
            }
        }
        intercambiarTransferencias(&transferencias[i + 1], &transferencias[der]);

        ordenarTransferencias(transferencias, izq, i);
        ordenarTransferencias(transferencias, i + 2, der);
    }
}

void mostrarTransferenciasOrdenadasRecursiva(char archivoTransferencias[], char dniUsuario[])
{
    FILE *fp = fopen(archivoTransferencias, "rb");
    if (fp)
    {
        stTransferencia transferencias[100];
        int numTransferencias = 0;

        while (fread(&transferencias[numTransferencias], sizeof(stTransferencia), 1, fp) == 1)
        {
            numTransferencias++;
        }

        fclose(fp);

        ordenarTransferencias(transferencias, 0, numTransferencias - 1);

        printf("Transferencias ordenadas de mayor a menor para la cuenta %s:\n", dniUsuario);
        for (int i = 0; i < numTransferencias; i++)
        {
            if (strcmp(transferencias[i].dniOrigen, dniUsuario) == 0 || strcmp(transferencias[i].dniDestino, dniUsuario) == 0)
            {
                printf("DNI Origen: %s | DNI Destino: %s | Monto: %.2f\n", transferencias[i].dniOrigen, transferencias[i].dniDestino, transferencias[i].monto);
            }
        }
    }
    else
    {
        printf("No se pudo abrir el archivo de transferencias para mostrar las transferencias ordenadas.\n");
    }
}

void intercambiarClientes(stCliente *a, stCliente *b)
{
    stCliente temp[20];
    strcpy(temp,a);
    strcpy(a,b);
    strcpy(b,temp);
}
void ordenarClientesPorNombre(stCliente clientes[], int cant)
{
    for (int i = 0; i < cant - 1; i++)
    {
        for (int j = i + 1; j < cant; j++)
        {
            if (strcmp(clientes[i].nombreYapellido, clientes[j].nombreYapellido) > 0)
            {
                intercambiarClientes(&clientes[i], &clientes[j]);
            }
        }
    }
}
int PasarClientesAarreglo(char archivoCliente[] ,stCliente arreglo[])
{
    FILE * fp=fopen(archivoCliente,"rb");
    stCliente cliente;
    int i=0;
    if (fp)
    {
        while(fread(&cliente,sizeof(stCliente),1,fp)!=0)
        {
            arreglo[i]=cliente;
            i++;
        }
        fclose(fp);
        return i;
    }
    else
    {
        printf("El archivo no se pudo abrir");
    }

}
void mostrarArregloDeArchivoAlta(stCliente arreglo[],int val)
{
    int i=0;
    ordenarClientesPorNombre(arreglo,val);
    for(i;i<val;i++)
    {
      if(arreglo[i].estado == 1)
      {
        printf("Nombre y apellido: %s\n",arreglo[i].nombreYapellido);
        printf("DNI: %s.\n", arreglo[i].dni);
        printf("Edad: %d.\n", arreglo[i].edad);
        if(arreglo[i].estado == 1)
       {
         printf("Habilitado.\n");
       }
       else
       {
         printf("Inhabilitado.\n");
       }
       printf("Dinero: %.2f.\n", arreglo[i].plata);
       puts("-------------------------------------------\n");
     }
   }
}
void mostrarArregloDeArchivobaja(stCliente arreglo[],int val)
{
    int i=0;
    ordenarClientesPorNombre(arreglo,val);
    for(i;i<val;i++)
    {
      if(arreglo[i].estado == 0)
      {
        printf("Nombre y apellido: %s\n",arreglo[i].nombreYapellido);
        printf("DNI: %s.\n", arreglo[i].dni);
        printf("Edad: %d.\n", arreglo[i].edad);
        if(arreglo[i].estado == 1)
       {
         printf("Habilitado.\n");
       }
       else
       {
         printf("Inhabilitado.\n");
       }
       printf("Dinero: %.2f.\n", arreglo[i].plata);
       puts("-------------------------------------------\n");
    }
  }
}


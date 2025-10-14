#include <stdio.h>

// Estructura para los datos del pasajero
typedef struct {
    char tipoDocumento[3]; // CC, TI, PA
    char primerApellido[50];
} Pasajero;

// Nodo para simular lista enlazada (no se usa realmente en arreglo)
typedef struct Nodo {
    Pasajero pasajero;
    struct Nodo* siguiente;
} Nodo;

#define MAX_PASAJEROS 200

Nodo listaPasajeros[MAX_PASAJEROS];
int totalPasajeros = 0;
int capacidadAvion = 0;
int maxTiquetes = 0;

// Función para validar tipo de documento
int esTipoDocumentoValido(char* tipo) {
    return ( (tipo[0]=='C' && tipo[1]=='C') ||
             (tipo[0]=='T' && tipo[1]=='I') ||
             (tipo[0]=='P' && tipo[1]=='A') );
}

// Registrar pasajero
void registrarPasajero() {
    if (totalPasajeros < maxTiquetes && totalPasajeros < MAX_PASAJEROS) {
        char tipoDoc[3];
        printf("Tipo de documento (CC/TI/PA): ");
        scanf("%2s", tipoDoc);

        if (!esTipoDocumentoValido(tipoDoc)) {
            printf("Tipo de documento invalido. Solo se permite CC, TI o PA.\n");
            return;
        }

        // Copiar tipo de documento validado
        listaPasajeros[totalPasajeros].pasajero.tipoDocumento[0] = tipoDoc[0];
        listaPasajeros[totalPasajeros].pasajero.tipoDocumento[1] = tipoDoc[1];
        listaPasajeros[totalPasajeros].pasajero.tipoDocumento[2] = '\0';

        printf("Primer apellido: ");
        scanf("%49s", listaPasajeros[totalPasajeros].pasajero.primerApellido);

        listaPasajeros[totalPasajeros].siguiente = NULL;
        totalPasajeros++;
        printf("Pasajero registrado exitosamente.\n");
    } else {
        printf("No se pueden registrar mas pasajeros.\n");
    }
}

// Mostrar pasajeros
void mostrarPasajeros() {
    if (totalPasajeros == 0) {
        printf("\nNo hay pasajeros registrados.\n");
        return;
    }
    printf("\nLista de pasajeros registrados:\n");
    for (int i = 0; i < totalPasajeros; i++) {
        printf("%d. %s - %s\n", i + 1,
               listaPasajeros[i].pasajero.tipoDocumento,
               listaPasajeros[i].pasajero.primerApellido);
    }
}

int main() {
    printf("Ingrese la capacidad maxima de asientos del avion: ");
    scanf("%d", &capacidadAvion);

    if (capacidadAvion <= 0) {
        printf("La capacidad debe ser mayor a cero.\n");
        return 1;
    }

    maxTiquetes = capacidadAvion + (capacidadAvion / 10);

    printf("Se pueden vender hasta %d tiquetes (incluyendo overbooking).\n", maxTiquetes);

    int opcion;
    do {
        printf("\n1. Registrar pasajero\n2. Mostrar pasajeros\n3. Salir\nSeleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrarPasajero();
                break;
            case 2:
                mostrarPasajeros();
                break;
            case 3:
                printf("Programa finalizado.\n");
                break;
            default:
                printf("Opción invalida.\n");
        }
    } while (opcion != 3);

    return 0;
}
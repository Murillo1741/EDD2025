/*
  Taller: Árbol Binario de Búsqueda (ABB)
  Sistema de Gestión de Tiquetes para el Muelle Turístico

  Este archivo contiene el código paso a paso (comentado en español)
  que implementa un ABB para gestionar pasajeros usando el número
  de documento como clave.

  Requisitos implementados:
  - Estructura Pasajero (documento, destino, tipo_pasaje)
  - Inserción (ignora duplicados)
  - Recorridos: Inorden, Preorden, Postorden
  - Conteo de nodos
  - Eliminación por documento
  - Menú interactivo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------
   Paso 1: Definiciones
   ---------------------- */
#define DEST_COUNT 4
#define DEST_MAX_LEN 32

/* Lista de destinos permitidos */
const char *destinos[DEST_COUNT] = {"Timbiquí", "Juanchaco", "Tumaco", "Guapi"};

/* Estructura Pasajero: representa la información almacenada en cada nodo */
typedef struct Pasajero {
	int documento;               /* clave de búsqueda */
	char destino[DEST_MAX_LEN];  /* nombre del destino */
	int tipo_pasaje;             /* 1 = Ida, 2 = Ida y Regreso */
} Pasajero;

/* Nodo del ABB */
typedef struct Node {
	Pasajero info;
	struct Node *left;
	struct Node *right;
} Node;


/* ----------------------
   Paso 2: Utilidades básicas
   ---------------------- */

/* Crear un nuevo nodo a partir de un Pasajero */
Node *create_node(Pasajero p) {
	Node *n = (Node *)malloc(sizeof(Node));
	if (!n) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	n->info = p;
	n->left = n->right = NULL;
	return n;
}


/* ----------------------
   Paso 3: Inserción en ABB
   - Inserta por documento
   - Si documento ya existe, se ignora (no se inserta)
   ---------------------- */

int insert_node(Node **root, Pasajero p) {
	if (*root == NULL) {
		*root = create_node(p);
		return 1; /* insertado */
	}
	if (p.documento < (*root)->info.documento) {
		return insert_node(&(*root)->left, p);
	} else if (p.documento > (*root)->info.documento) {
		return insert_node(&(*root)->right, p);
	} else {
		/* documento repetido */
		return 0; /* no insertado */
	}
}


/* ----------------------
   Paso 4: Mostrar pasajero (formato legible)
   ---------------------- */

void print_pasajero(const Pasajero *p) {
	printf("Documento: %d | Destino: %s | Tipo: %s\n",
		   p->documento,
		   p->destino,
		   (p->tipo_pasaje == 1) ? "Ida" : "Ida y Regreso");
}


/* ----------------------
   Paso 5: Recorridos (inorden, preorden, postorden)
   - Inorden: orden ascendente por documento
   ---------------------- */

void inorder(Node *root) {
	if (!root) return;
	inorder(root->left);
	print_pasajero(&root->info);
	inorder(root->right);
}

void preorder(Node *root) {
	if (!root) return;
	print_pasajero(&root->info);
	preorder(root->left);
	preorder(root->right);
}

void postorder(Node *root) {
	if (!root) return;
	postorder(root->left);
	postorder(root->right);
	print_pasajero(&root->info);
}


/* ----------------------
   Paso 6: Conteo de nodos
   ---------------------- */

int count_nodes(Node *root) {
	if (!root) return 0;
	return 1 + count_nodes(root->left) + count_nodes(root->right);
}


/* ----------------------
   Paso 7: Eliminación de un nodo por documento
   - Casos: hoja, un hijo, dos hijos
   - En el caso de dos hijos, se usa el sucesor mínimo del subárbol derecho
   ---------------------- */

Node *find_min(Node *root) {
	if (!root) return NULL;
	while (root->left) root = root->left;
	return root;
}

Node *delete_node(Node *root, int documento, int *deleted) {
	if (!root) return NULL;
	if (documento < root->info.documento) {
		root->left = delete_node(root->left, documento, deleted);
	} else if (documento > root->info.documento) {
		root->right = delete_node(root->right, documento, deleted);
	} else {
		/* encontrado */
		*deleted = 1;
		if (!root->left && !root->right) {
			free(root);
			return NULL;
		} else if (!root->left) {
			Node *tmp = root->right;
			free(root);
			return tmp;
		} else if (!root->right) {
			Node *tmp = root->left;
			free(root);
			return tmp;
		} else {
			Node *minRight = find_min(root->right);
			root->info = minRight->info; /* copiar datos del sucesor */
			root->right = delete_node(root->right, minRight->info.documento, deleted);
		}
	}
	return root;
}


/* ----------------------
   Paso 8: Liberar memoria del árbol
   ---------------------- */

void free_tree(Node *root) {
	if (!root) return;
	free_tree(root->left);
	free_tree(root->right);
	free(root);
}


/* ----------------------
   Paso 9: Helpers de entrada (seleccionar destino y tipo)
   - Para simplificar se muestra un menú con destinos predefinidos
   ---------------------- */

int seleccionar_destino(char *out_dest) {
	int opt = 0;
	printf("Seleccione destino:\n");
	for (int i = 0; i < DEST_COUNT; ++i) {
		printf("  %d) %s\n", i + 1, destinos[i]);
	}
	printf("Opción: ");
	if (scanf("%d", &opt) != 1) {
		while (getchar() != '\n');
		return 0;
	}
	if (opt < 1 || opt > DEST_COUNT) return 0;
	strncpy(out_dest, destinos[opt - 1], DEST_MAX_LEN - 1);
	out_dest[DEST_MAX_LEN - 1] = '\0';
	return 1;
}

int seleccionar_tipo() {
	int t = 0;
	printf("Tipo de pasaje:\n  1) Ida\n  2) Ida y Regreso\nOpción: ");
	if (scanf("%d", &t) != 1) {
		while (getchar() != '\n');
		return 0;
	}
	if (t != 1 && t != 2) return 0;
	return t;
}


/* ----------------------
   Paso 10: Menú interactivo
   - Ofrece las operaciones solicitadas
   ---------------------- */

int main(void) {
	Node *root = NULL;
	int running = 1;

	while (running) {
		printf("\n--- Sistema de Tiquetes - ABB ---\n");
		printf("1) Registrar pasajero\n");
		printf("2) Eliminar pasajero por documento\n");
		printf("3) Listar Inorden\n");
		printf("4) Listar Preorden\n");
		printf("5) Listar Postorden\n");
		printf("6) Contar pasajeros\n");
		printf("7) Salir\n");
		printf("Seleccione opción: ");

		int opt = 0;
		if (scanf("%d", &opt) != 1) {
			printf("Entrada inválida. Intente de nuevo.\n");
			while (getchar() != '\n');
			continue;
		}

		switch (opt) {
			case 1: {
				/* Registrar pasajero: leer documento, destino y tipo */
				Pasajero p;
				printf("Documento (entero): ");
				if (scanf("%d", &p.documento) != 1) {
					printf("Documento inválido.\n");
					while (getchar() != '\n');
					break;
				}
				while (getchar() != '\n'); /* limpiar buffer */

				if (!seleccionar_destino(p.destino)) {
					printf("Destino inválido. Registro cancelado.\n");
					break;
				}

				p.tipo_pasaje = seleccionar_tipo();
				if (p.tipo_pasaje == 0) {
					printf("Tipo inválido. Registro cancelado.\n");
					break;
				}

				if (insert_node(&root, p)) {
					printf("Pasajero registrado correctamente.\n");
				} else {
					printf("Documento ya existe. Registro ignorado.\n");
				}
				break;
			}
			case 2: {
				int doc;
				printf("Documento a eliminar: ");
				if (scanf("%d", &doc) != 1) {
					printf("Entrada inválida.\n");
					while (getchar() != '\n');
					break;
				}
				int deleted = 0;
				root = delete_node(root, doc, &deleted);
				if (deleted) printf("Pasajero con documento %d eliminado.\n", doc);
				else printf("Documento %d no encontrado.\n", doc);
				break;
			}
			case 3:
				if (!root) printf("Árbol vacío.\n"); else inorder(root);
				break;
			case 4:
				if (!root) printf("Árbol vacío.\n"); else preorder(root);
				break;
			case 5:
				if (!root) printf("Árbol vacío.\n"); else postorder(root);
				break;
			case 6: {
				int c = count_nodes(root);
				printf("Total de pasajeros registrados: %d\n", c);
				break;
			}
			case 7:
				running = 0;
				break;
			default:
				printf("Opción inválida. Intente otra vez.\n");
		}
	}

	free_tree(root);
	printf("Saliendo. Memoria liberada.\n");
	return 0;
}


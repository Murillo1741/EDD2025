/*
 * Proyecto Final - Sistema Logístico
 * Implementa un árbol ABB con balanceo AVL y colas FIFO por lote (fecha de caducidad).
 * Funcionalidades:
 *  - Recepción de mercancía (insertar en AVL)
 *  - Registrar pedido (encolar en FIFO del lote con fecha más cercana a vencer)
 *  - Baja de producto (eliminar nodo y su cola)
 *  - Cancelar pedido (eliminar un pedido específico y restaurar stock)
 *  - Reporte de estado (in-order)
 *
 * Compilar: gcc final.c -o final.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64

typedef struct Order {
    char *destination;
    int quantity;
    struct Order *next;
} Order;

typedef struct AVLNode {
    int date; // AAAAMMDD
    char *product;
    int stock;
    Order *head;
    Order *tail;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Utility: allocate string copy
char *strdup_alloc(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}

int max(int a, int b) { return (a > b) ? a : b; }

int height(AVLNode *n) { return n ? n->height : 0; }

AVLNode *new_node(int date, const char *product, int stock) {
    AVLNode *node = malloc(sizeof(AVLNode));
    if (!node) return NULL;
    node->date = date;
    node->product = strdup_alloc(product);
    node->stock = stock;
    node->head = node->tail = NULL;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

Order *new_order(const char *dest, int qty) {
    Order *o = malloc(sizeof(Order));
    if (!o) return NULL;
    o->destination = strdup_alloc(dest);
    o->quantity = qty;
    o->next = NULL;
    return o;
}

void free_order_list(Order *head) {
    while (head) {
        Order *t = head;
        head = head->next;
        free(t->destination);
        free(t);
    }
}

AVLNode *rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode *rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int get_balance(AVLNode *n) {
    if (!n) return 0;
    return height(n->left) - height(n->right);
}

AVLNode *insert_avl(AVLNode *node, int date, const char *product, int stock, int *inserted) {
    if (!node) {
        *inserted = 1;
        return new_node(date, product, stock);
    }
    if (date < node->date)
        node->left = insert_avl(node->left, date, product, stock, inserted);
    else if (date > node->date)
        node->right = insert_avl(node->right, date, product, stock, inserted);
    else {
        *inserted = 0; // duplicate date
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = get_balance(node);

    // LL
    if (balance > 1 && date < node->left->date)
        return rotate_right(node);
    // RR
    if (balance < -1 && date > node->right->date)
        return rotate_left(node);
    // LR
    if (balance > 1 && date > node->left->date) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    // RL
    if (balance < -1 && date < node->right->date) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

AVLNode *min_value_node(AVLNode *node) {
    AVLNode *current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

AVLNode *delete_avl(AVLNode *root, int date, int *deleted) {
    if (!root) return root;
    if (date < root->date)
        root->left = delete_avl(root->left, date, deleted);
    else if (date > root->date)
        root->right = delete_avl(root->right, date, deleted);
    else {
        // found
        *deleted = 1;
        // free queue
        free_order_list(root->head);
        free(root->product);
        // node with one or no child
        if (!root->left || !root->right) {
            AVLNode *temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp; // copy content
            }
            free(temp);
        } else {
            AVLNode *temp = min_value_node(root->right);
            // copy data
            free(root->product);
            root->date = temp->date;
            root->product = strdup_alloc(temp->product);
            root->stock = temp->stock;
            // free old queue and duplicate successor's queue
            free_order_list(root->head);
            // Duplicate queue
            root->head = root->tail = NULL;
            for (Order *it = temp->head; it; it = it->next) {
                Order *n = new_order(it->destination, it->quantity);
                if (!root->head) root->head = root->tail = n;
                else { root->tail->next = n; root->tail = n; }
            }
            root->right = delete_avl(root->right, temp->date, deleted); // will delete successor
        }
    }

    if (!root) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = get_balance(root);

    // LL
    if (balance > 1 && get_balance(root->left) >= 0)
        return rotate_right(root);
    // LR
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    // RR
    if (balance < -1 && get_balance(root->right) <= 0)
        return rotate_left(root);
    // RL
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

AVLNode *find_node(AVLNode *root, int date) {
    if (!root) return NULL;
    if (date == root->date) return root;
    if (date < root->date) return find_node(root->left, date);
    return find_node(root->right, date);
}

// Enqueue order at specific node
int enqueue_order(AVLNode *node, const char *dest, int qty) {
    if (!node) return 0;
    if (qty <= 0) return 0;
    if (node->stock < qty) return -1; // insufficient stock
    Order *o = new_order(dest, qty);
    if (!o) return 0;
    if (!node->head) node->head = node->tail = o;
    else { node->tail->next = o; node->tail = o; }
    node->stock -= qty;
    return 1;
}

// Cancel specific order from a node's queue by matching destination and quantity (first match)
int cancel_order_in_node(AVLNode *node, const char *dest, int qty) {
    if (!node || !node->head) return 0;
    Order *prev = NULL;
    Order *it = node->head;
    while (it) {
        if (strcmp(it->destination, dest) == 0 && it->quantity == qty) {
            if (prev) prev->next = it->next;
            else node->head = it->next;
            if (it == node->tail) node->tail = prev;
            node->stock += it->quantity;
            free(it->destination);
            free(it);
            return 1;
        }
        prev = it;
        it = it->next;
    }
    return 0;
}

// Count orders
int count_orders(Order *head) {
    int c = 0;
    while (head) { c++; head = head->next; }
    return c;
}

// Report: in-order from smallest date (closest to expire)
void report_inorder(AVLNode *root) {
    if (!root) return;
    report_inorder(root->left);
    int y = root->date / 10000;
    int m = (root->date / 100) % 100;
    int d = root->date % 100;
    printf("Fecha: %04d-%02d-%02d | Producto: %s | Stock: %d | Pedidos en espera: %d\n",
           y, m, d, root->product, root->stock, count_orders(root->head));
    report_inorder(root->right);
}

// Free whole tree
void free_tree(AVLNode *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free_order_list(root->head);
    free(root->product);
    free(root);
}

// Helper to read string with spaces
void read_line(char *buf, int size) {
    if (!fgets(buf, size, stdin)) { buf[0] = '\0'; return; }
    size_t len = strlen(buf);
    if (len && buf[len-1] == '\n') buf[len-1] = '\0';
}

void pause() { printf("Presiona ENTER para continuar...\n"); getchar(); }

int main() {
    AVLNode *root = NULL;
    int choice = 0;
    char buf[256];

    while (1) {
        printf("\n--- Sistema Logístico - Puerto Buenaventura ---\n");
        printf("1) Recepción de Mercancía (Insertar en AVL)\n");
        printf("2) Registrar Pedido de Despacho (Encolar FIFO, lote más cercano a vencer)\n");
        printf("3) Baja de Producto (Eliminar lote por fecha)\n");
        printf("4) Cancelar Pedido (Eliminar pedido específico)\n");
        printf("5) Reporte de Estado (In-Order)\n");
        printf("6) Salir\n");
        printf("Elija una opción: ");
        if (!fgets(buf, sizeof(buf), stdin)) break;
        choice = atoi(buf);
        if (choice == 1) {
            printf("Ingrese fecha de vencimiento (AAAAMMDD): ");
            int date; if (scanf("%d", &date) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); continue; }
            while(getchar()!='\n');
            printf("Ingrese cantidad (stock): "); int qty; if (scanf("%d", &qty) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); continue; }
            while(getchar()!='\n');
            printf("Ingrese nombre del producto: "); read_line(buf, sizeof(buf));
            int inserted = 0;
            root = insert_avl(root, date, buf, qty, &inserted);
            if (!inserted) printf("Error: ya existe un lote con esa fecha, no se puede procesar.\n");
            else printf("Lote agregado correctamente.\n");
        } else if (choice == 2) {
            AVLNode *node = min_value_node(root);
            if (!node) { printf("No hay lotes disponibles.\n"); continue; }
            printf("Lote seleccionado: %s (Fecha %08d) Stock: %d\n", node->product, node->date, node->stock);
            printf("Ingrese destino: "); read_line(buf, sizeof(buf));
            printf("Ingrese cantidad solicitada: "); int qty; if (scanf("%d", &qty) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); continue; }
            while(getchar()!='\n');
            int res = enqueue_order(node, buf, qty);
            if (res == -1) printf("No hay stock suficiente para ese pedido.\n");
            else if (res == 1) printf("Pedido encolado correctamente.\n");
            else printf("Error al encolar pedido.\n");
        } else if (choice == 3) {
            printf("Ingrese fecha del lote a eliminar (AAAAMMDD): "); int date; if (scanf("%d", &date) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); continue; }
            while(getchar()!='\n');
            int deleted = 0;
            root = delete_avl(root, date, &deleted);
            if (deleted) printf("Lote eliminado correctamente (se liberó su cola).\n");
            else printf("No se encontró lote con esa fecha.\n");
        } else if (choice == 4) {
            printf("Ingrese fecha del lote donde está el pedido (AAAAMMDD): "); int date; if (scanf("%d", &date) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); continue; }
            while(getchar()!='\n');
            AVLNode *node = find_node(root, date);
            if (!node) { printf("No se encontró lote con esa fecha.\n"); continue; }
            printf("Pedidos actuales en ese lote:\n");
            int idx = 1;
            for (Order *it = node->head; it; it = it->next) {
                printf("%d) Destino: %s | Cantidad: %d\n", idx++, it->destination, it->quantity);
            }
            if (!node->head) { printf("No hay pedidos en ese lote.\n"); continue; }
            printf("Ingrese destino del pedido a cancelar: "); read_line(buf, sizeof(buf));
            printf("Ingrese cantidad del pedido a cancelar: "); int qty; if (scanf("%d", &qty) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); continue; }
            while(getchar()!='\n');
            int canceled = cancel_order_in_node(node, buf, qty);
            if (canceled) printf("Pedido cancelado y stock restaurado.\n");
            else printf("Pedido no encontrado en la cola.\n");
        } else if (choice == 5) {
            printf("\n--- Reporte de Estado (ordenado por fecha: más próxima a vencer primero) ---\n");
            report_inorder(root);
        } else if (choice == 6) {
            break;
        } else {
            printf("Opción inválida.\n");
        }
    }

    free_tree(root);
    printf("Saliendo y liberando memoria.\n");
    return 0;
}

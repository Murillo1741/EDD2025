Proyecto Final - Sistema Logístico (Puerto Buenaventura)

Descripción
- Implementación en C de un sistema para gestionar inventarios perecederos por fecha de caducidad.
- Estructura híbrida: un Árbol ABB balanceado (AVL) donde cada nodo representa un lote (una fecha de vencimiento) y cada nodo contiene una cola FIFO de pedidos (envíos) para ese lote.

Estructuras de datos principales
- Nodo del árbol (AVL): clave `date` (entero AAAAMMDD), `product` (nombre), `stock` (cantidad disponible), y una cola FIFO (`head`/`tail`) con los pedidos.
- Pedido (Order): `destination` (string), `quantity` (int), `next` (puntero).

Comandos para compilar y ejecutar
- Desde la carpeta del proyecto:
```bash
gcc "Taller 11 diciembre/final.c" -o "Taller 11 diciembre/final.exe"
# Ejecutar:
./"Taller 11 diciembre/final.exe"   (en terminal compatible)
# O, si está en el mismo directorio que el ejecutable:
Taller\ 11\ diciembre\final.exe    (Windows PowerShell o CMD)
```

Menú y funciones del programa
1) Recepción de Mercancía (Insertar en AVL)
   - Pedir `fecha` (AAAAMMDD), `cantidad` y `nombre producto`.
   - Si ya existe un nodo con la misma fecha, la inserción se rechaza (no se procesan duplicados).
   - Si no existe, se crea un nodo y el árbol se balancea automáticamente (AVL).

2) Registrar Pedido de Despacho (Encolar FIFO)
   - El sistema elige el lote con la fecha más próxima a vencer (mínima fecha en el árbol).
   - Se pide `destino` y `cantidad`; el pedido se añade al final de la cola del nodo y se descuenta del `stock` del lote.
   - Si no hay stock suficiente, el pedido es rechazado.

3) Baja de Producto (Eliminar lote)
   - Elimina un lote por fecha (libera primero toda su cola de pedidos, luego el nodo) y balancea el árbol.

4) Cancelar Pedido (Eliminar pedido específico)
   - Busca en la cola del lote por `destino` y `cantidad` (primer match) y elimina ese pedido, restaurando la cantidad al `stock` del lote.

5) Reporte de Estado
   - Recorrido In-Order del AVL: muestra cada lote en orden creciente de fecha (de más próximo a vencer a más lejano), con `fecha`, `producto`, `stock` y número de pedidos en espera.

Notas sobre memoria y evaluación
- Al eliminar un nodo, la cola de pedidos se libera antes de liberar el nodo (evita fugas).
- El árbol mantiene balance AVL tras inserciones y eliminaciones.

Autor y materia
- Nombre: Diver Andres Murillo Valencia
- Materia: Estructuras de Datos

Observaciones
- El programa es interactivo por consola. Para pruebas automatizadas o mejoras, se pueden añadir funciones para importar/exportar datos, o una interfaz de prueba que simule inserciones y pedidos.

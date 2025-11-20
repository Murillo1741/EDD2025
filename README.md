# Taller: Sistema de Tiquetes (ABB)

Este repositorio contiene un programa en C que implementa un Árbol Binario de Búsqueda (ABB)
para gestionar la venta y reserva de tiquetes en un muelle turístico. El árbol usa el número de
documento del viajero como clave de indexación.

Resumen de la implementación
- Estructura `Pasajero`: contiene `documento` (clave), `destino` (cadena) y `tipo_pasaje` (1 = Ida, 2 = Ida y Regreso).
- Árbol Binario de Búsqueda (`Node`) para almacenar los pasajeros.
- Operaciones implementadas:
  - Insertar pasajero (ignora duplicados por `documento`).
  - Recorridos: Inorden, Preorden y Postorden.
  - Contar pasajeros.
  - Eliminar pasajero por `documento` (maneja 0/1/2 hijos).
  - Menú interactivo para ejecutar las acciones.

Pruebas rápidas sugeridas (si desea)
- Insertar documentos: 100, 50, 150. Hacer `Listar Inorden` debe mostrar 50, 100, 150.
- Eliminar el documento 100 y listar de nuevo para verificar la eliminación.


Estudiante: Diver Andrés Murillo Valencia
Estructura de datos
Universidad del Pacífico
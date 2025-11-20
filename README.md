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

Archivo principal
- `Taller 20 noviembre/taller.c`: implementación paso a paso con comentarios en español.

Cómo compilar (Windows PowerShell)
```powershell
gcc "Taller 20 noviembre\taller.c" -o taller.exe
```

Cómo ejecutar
```powershell
.\taller.exe
```

Pruebas rápidas sugeridas
- Insertar documentos: 100, 50, 150. Hacer `Listar Inorden` debe mostrar 50, 100, 150.
- Eliminar el documento 100 y listar de nuevo para verificar la eliminación.

Mejoras posibles
- Añadir búsqueda por documento (`search`) para consultar un pasajero concreto.
- Guardar y cargar datos desde un archivo (CSV) para persistencia.
- Usar `fgets` + `sscanf` para lecturas más robustas en lugar de `scanf`.
- Implementar un árbol balanceado (AVL o Red-Black) si se esperan muchos registros.

Contacto
- Si quieres que añada la función de búsqueda, persistencia o que reemplace `taller.c` por otra versión, dime cuál y lo implemento.

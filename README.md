# 13/10 :

## Decisiones de diseño

Prioridad del taller: eléctricas = 1. 
Mecánicas averiadas: más de 3000 km → 1
más de 1500 km → 2, 
el resto → 3. 
Hash: h = h*31 + caracter, módulo del tamaño de la tabla (31). Colisiones por encadenamiento.
Colisiones: se cuentan al insertar y no se descuentan al eliminar.
A5 con enteros: bicis * 100 < capacidad * 20, sin double, sin errores de redondeo.
Fechas: AAAAMMDD se compara directamente como int.

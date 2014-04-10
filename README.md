sqlite
======

Por cuestiones financieras mi webhosting ya no tiene PostgreSQL, así
que ahora programo en SQLite. He aquí algunas de mis creaciones.

* uid.c : Devuelve una cadena alfanumérica que sirve como
  identificador único. La ventaja que tiene esto sobre una simple
  secuencia numérica es que se quita la tentación de usar los ids
  cuando se quiere un _merge_ de dos orígenes de datos distintos.

* es_mx.c : Un _collation_ para idioma castellano. En específico
  quiero que los acentos se pongan en el lugar correcto.

* normalize.c : Una función para "normalizar" cadenas --- lo que hace
  es quitar acentos. Esto es útil para búsqueda por título en la base
  de datos de canciones.

La calidad de este código es actualmente muy pobre porque acabo de
empezar, pero irá mejorando poco a poco.




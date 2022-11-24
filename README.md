[![✗](https://img.shields.io/badge/Release-v0.2.0-ffb600.svg?style=for-the-badge)](https://github.com/agustinmorantes/ScriptScript/releases)

# ScriptScript
Nuestra idea consiste en un lenguaje que permita escribir y definir flujos de texto enriquecido (como puede ser diálogo en un videojuego o tutoriales para una aplicación móvil o web), con una sintaxis sencilla de usar similar a _Markdown_, de forma que pueda ser utilizado no solo por programadores sino que también por perfiles menos técnicos como podrían ser guionistas o traductores.

A diferencia de un simple archivo de _Word_ o _Markdown_, nuestro compilador toma guiones escritos en _ScriptScript_ y los convierte a un formato _JSON_ intermedio, el cual podría ser interpretado por un _plugin_ en un _framework_ o motor gráfico y ser utilizado para mostrar diálogos complejos e interactivos en videojuegos, presentaciones o aplicaciones de forma más sencilla, sin necesidad de un intermediario técnico que organice y estructure el guión para su uso en la aplicación final.


## Requerimientos

Para construir el compilador, se requieren las siguientes dependencias:

* [Bison v3.8.2](https://www.gnu.org/software/bison/)
* [CMake v3.24.1](https://cmake.org/)
* [Flex v2.6.4](https://github.com/westes/flex)
* [GCC v11.1.0](https://gcc.gnu.org/)
* [Make v4.3](https://www.gnu.org/software/make/)

Si en lugar de trabajar con un entorno _Linux_, se está construyendo el proyecto sobre un entorno _Microsoft Windows_, se debe instalar _Microsoft Visual Studio 2022_ con las extensiones para desarrollar aplicaciones en _C/C++_, así como también las herramientas requeridas, con excepción del compilador _GCC_ y la herramienta _Make_.

## Construcción

Para construir el proyecto por completo, ejecute en la raíz del repositorio los siguientes comandos (en _Linux_):

```bash
user@machine:path/ $ chmod u+x --recursive script
user@machine:path/ $ script/build.sh
```

En un entorno _Microsoft Windows_, en cambio, se debe ejecutar:

```bash
user@machine:path/ $ script\build.bat
```

Luego se deberá abrir la solución generada `bin\Compiler.sln` con el IDE _Microsoft Visual Studio 2022_. Los ejecutables que este sistema construye se depositan dentro del directorio `bin\Debug` y `bin\Release`, según corresponda.


## Cómo empezar

Para compilar un programa, primero cree un archivo vacío denominado `program` (o el nombre que desee), como por ejemplo:

```markdown
{
id: start
---
Bienvenidos todos a la coronación de mi hijo...
}
```

Luego, ejecute el compilador desde el directorio raíz del proyecto, o desde cualquier otro lugar indicando el path hacia el script `start.sh` y pasando por parámetro el path hacia el programa a compilar:

```bash
user@machine:path/ $ script/start.sh program
```

En Windows:

```bash
user@machine:path/ $ script\start.bat program
```

Debería obtener el resultado correcto de evaluar el programa anterior:

```json
{
  "header": {
    "id": "start"
  },
  "body": [
    {
      "type": "text",
      "text": "Bienvenidos todos a la coronación de mi hijo..."
    }
  ]
}
```

## Algunas nociones de ScriptScript
En esta sección procedemos a detallar algunas de las funcionalidades del lenguaje.

### Estructura de un archivo .scsc
Un archivo .scsc consiste de mútliples bloques declarados entre llaves:
```markdown
{
/* Header */
id: start
next: bloque2
---
/* Body */
Hola!
}

{
/* Segundo bloque */
id: bloque2
---
Hola!!
}
/* ... */
```
Estos bloques se separan en Header y Body.
En la sección del Header se agregan todos los metadatos que identifican al bloque.
El tag `id` es obligatorio, en donde se le da un nombre identificatorio al bloque, o se le asigna la palabra reservada `start` para indicar que es el primer bloque a ejecutar.  El tag `next` indica el id del siguiente bloque a mostrar.

En la sección del Body se inserta texto con formato Markdown, que será el que representa al diálogo.
Aquí se pueden utilizar variables con el símbolo `$` de la forma: `$thisIsAVariable`, y también se puede formatear el texto con sintáxis Markdown.

### Condicionales
Se pueden utilizar los condicionales `if` y `match` para dar valores condicionales, por ejemplo para elegir el siguiente diálogo a mostrar dependiendo del valor de una variable:
```markdown
{
id: start
next: bloque2 if var is true else bloque3
---
}
/* etc. */
```
```markdown
{
id: start
next: match var 
      when 2 then bloque2
      when 3 then bloque3
      default bloque4
---
}
/* etc. */
```

### Bloques de decisión
Otra funcionalidad es la de insertar decisiones para el usuario en el diálogo.
Esto se hace indicando una variable en el header de la forma `output: var`,
y luego en el texto insertando una serie de decisiones de la siguiente forma:

```markdown
{
id: start
output: var
---
@ 1 : Esta es la decisión 1
@ 2 : Esta es la decisión 2
@ 3 : Esta es la decisión 3
/* etc. */
}
/* etc. */
```
El valor que figura a la derecha del símbolo `:` Es el prompt que el usuario elige, mientras que el valor que figura a la izquierda es el que se inserta en la variable si el usuario toma esa rama de la decisión.

### Tags
Por último está la funcionalidad de tags, que permiten añadir metadatos dentro del texto, similar a los operadores para formato de Markdown
```markdown
{
id: start
---
Buenas, [wiggle](¿Cómo va?)
}

```

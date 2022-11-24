[![✗](https://img.shields.io/badge/Release-v0.2.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

# ScriptScript
Nuestra idea consiste en un lenguaje que permita escribir y definir flujos de texto enriquecido (como puede ser diálogo en un videojuego o tutoriales para una aplicación móvil o web), con una sintaxis sencilla de usar similar a _Markdown_, de forma que pueda ser utilizado no solo por programadores sino que también por perfiles menos técnicos como podrían ser guionistas o traductores.

A diferencia de un simple archivo de _Word_ o _Markdown_, nuestro compilador toma guiones escritos en _ScriptScript_ y los convierte a un formato _JSON_ intermedio, el cual podría ser interpretado por un _plugin_ en un _framework_ o motor gráfico y ser utilizado para mostrar diálogos complejos e interactivos en videojuegos, presentaciones o aplicaciones de forma más sencilla, sin necesidad de un intermediario técnico que organice y estructure el guión para su uso en la aplicación final.

## Cómo empezar

Para compilar un programa, primero cree un archivo vacío denominado `program` (o el nombre que desee), como por ejemplo:

```
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

```
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

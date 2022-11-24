[![✗](https://img.shields.io/badge/Release-v0.2.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

# ScriptScript
Markdown dialogue scripting language

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
  "body": {
    "type": "text",
    "text": "Bienvenidos todos a la coronación de mi hijo..."
  }
}
```

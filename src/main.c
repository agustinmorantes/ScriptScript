#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "frontend/syntactic-analysis/bison-parser.h"
#include "backend/support/symtable.h"
#include "backend/semantic-analysis/analyzer.h"
#include <stdio.h> 

// Estado de la aplicación.
CompilerState state;

// Punto de entrada principal del compilador.
const int main(const int argumentCount, const char ** arguments) {
	// Inicializar estado de la aplicación.
	state.program = NULL;
	state.result = 0;
	state.succeed = false;
	state.errorCount = 0;

	// Inicializar tabla de símbolos.
	symtable_init();

	// Mostrar parámetros recibidos por consola.
	for (int i = 0; i < argumentCount; ++i) {
		LogInfo("Argumento %d: '%s'", i, arguments[i]);
	}

	// Compilar el programa de entrada.
	LogInfo("Compilando...\n");
	const int result = yyparse();
	switch (result) {
		case 0:
			// La variable "succeed" es la que setea Bison al identificar el símbolo
			// inicial de la gramática satisfactoriamente.
			if (state.succeed) {
				LogInfo("Realizando análisis semántico...");
				if(!symtable_check()) {
					LogError("Error al chequear tabla de símbolos.");
					return 4;
				}
				AnalyzeAST(state.program);
				if(state.errorCount > 0) {
					LogError("La compilación no pudo finalizar debido a errores en el análisis semántico.");
					LogError("Se encontraron %d errores.", state.errorCount);
					return 5;
				}
				LogInfo("El análisis semántico fue exitoso.");

				LogInfo("Generando código...");
				Generator(state.program);
				LogInfo("La compilación fue exitosa.");
			}
			else {
				LogError("Se produjeron %d errores en la compilación.", state.errorCount);
				symtable_free();
				return 3;
			}
			break;
		case 1:
			LogError("Bison finalizo debido a un error de sintaxis.");
			break;
		case 2:
			LogError("Bison finalizo abruptamente debido a que ya no hay memoria disponible.");
			break;
		default:
			LogError("Error desconocido mientras se ejecutaba el analizador Bison (codigo %d).", result);
	}
	LogInfo("Fin.");
	symtable_free();
	return result;
}

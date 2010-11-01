GEOC Ejercicio 5 Laboratorio

    Miguel Angel Vico Moya


**Aclaraciones del código:**

    ¡¡IMPORTANTE: VER PUNTO (3) DE OTRAS CONSIDERACIONES!!

    __Cálculo del triángulo contenedor:__

        Para calcular el triángulo contenedor se ha optado por, una vez tenemos
        toda la nuve de puntos, conseguir un rectángulo contenedor y
        posteriormente construir un triángulo contenedor de dicho rectángulo.
        
        Para conseguir el rectángulo contenedor se analizan todos los puntos
        (O(n)) encontrando las máximas y mínimas coordenadas en X y en Y de
        forma que conseguimos encontrar un rectángulo que contiene todos los
        puntos (se amplia ligéramente el perímetro del mismo para que no haya
        ningún punto en la frontera).
        
        Posteriormente se contruye un triángulo de la forma más sencilla
        posible. Se ha considerado que un triángulo isósceles cuyos ángulos
        internos son de 45º, 45º y 90º de forma que se hace muy sencillo el
        encontrar 3 puntos que definan un triángulo contenedor del rectángulo.
        
        
    __Estructura de datos para la triangulación:__
    
        La estructura de datos empleada para realizar la triangulación a sido
	una implementación de un DCEL basado en Half Edges, de forma que
	cada arista posee un puntero a su siguiente y a su gemela, así como un
	puntero al vertice origen y un puntero a la cara a la que pertenece.
	Además se ha añadido la posibilidad de marcar una arista como no
	dibujable, para facilitar la tarea de eliminar las líneas y triangulos de mar.
	
	Los vertices del DCEL constan de un índice a punto y de un puntero a
	una de las aristas que salen de él.
	
	Las caras del DCEL constan de un puntero a una arista de las que esta
	formada (para iniciar la navegación) y se ha añadido la posibilidad de
	marcar la cara como infinito para identificar la o las caras exteriores a
	la triangulación.
	
        
    __Otras consideraciones:__
    
        (1) Para dibujar los triángulos resultantes se ha empleado la estructura
        de Triangle, de forma que pulsando la tecla "f" podemos cambiar el
	modo de ver los triangulos (rellenos o líneas).
        
        (2) También se ha exagerado la coordenada en Z de los puntos para que
        pueda verse con más detalle el relieve que estos puedan tener.
        
        (3) Se han añadido algunas opciones al programa, pasadas como
	parámetros. Estas opciones son:
		-D : Indica que se desea realizar una triangulación de Delaunay.
		-ND : Indica que se desea realizar una triangulación no de
			Delaunay.
		-NUT : Indica que no se desea ver los triángulos y líneas auxiliares
			utilizados para la triangulación (líneas de mar).
		-UT : Indica que se desea ver los triángulos y líneas auxiliares
			utilizados para la triangulación (líneas de mar).
	
	Por defecto, sin especificar ninguna opción, el programa es ejecutado
	con las opciones -D y -NUT.
        



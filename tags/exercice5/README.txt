GEOC Ejercicio 5 Laboratorio

    Miguel Angel Vico Moya


**Aclaraciones del c�digo:**

    ��IMPORTANTE: VER PUNTO (3) DE OTRAS CONSIDERACIONES!!

    __C�lculo del tri�ngulo contenedor:__

        Para calcular el tri�ngulo contenedor se ha optado por, una vez tenemos
        toda la nuve de puntos, conseguir un rect�ngulo contenedor y
        posteriormente construir un tri�ngulo contenedor de dicho rect�ngulo.
        
        Para conseguir el rect�ngulo contenedor se analizan todos los puntos
        (O(n)) encontrando las m�ximas y m�nimas coordenadas en X y en Y de
        forma que conseguimos encontrar un rect�ngulo que contiene todos los
        puntos (se amplia lig�ramente el per�metro del mismo para que no haya
        ning�n punto en la frontera).
        
        Posteriormente se contruye un tri�ngulo de la forma m�s sencilla
        posible. Se ha considerado que un tri�ngulo is�sceles cuyos �ngulos
        internos son de 45�, 45� y 90� de forma que se hace muy sencillo el
        encontrar 3 puntos que definan un tri�ngulo contenedor del rect�ngulo.
        
        
    __Estructura de datos para la triangulaci�n:__
    
        La estructura de datos empleada para realizar la triangulaci�n a sido
	una implementaci�n de un DCEL basado en Half Edges, de forma que
	cada arista posee un puntero a su siguiente y a su gemela, as� como un
	puntero al vertice origen y un puntero a la cara a la que pertenece.
	Adem�s se ha a�adido la posibilidad de marcar una arista como no
	dibujable, para facilitar la tarea de eliminar las l�neas y triangulos de mar.
	
	Los vertices del DCEL constan de un �ndice a punto y de un puntero a
	una de las aristas que salen de �l.
	
	Las caras del DCEL constan de un puntero a una arista de las que esta
	formada (para iniciar la navegaci�n) y se ha a�adido la posibilidad de
	marcar la cara como infinito para identificar la o las caras exteriores a
	la triangulaci�n.
	
        
    __Otras consideraciones:__
    
        (1) Para dibujar los tri�ngulos resultantes se ha empleado la estructura
        de Triangle, de forma que pulsando la tecla "f" podemos cambiar el
	modo de ver los triangulos (rellenos o l�neas).
        
        (2) Tambi�n se ha exagerado la coordenada en Z de los puntos para que
        pueda verse con m�s detalle el relieve que estos puedan tener.
        
        (3) Se han a�adido algunas opciones al programa, pasadas como
	par�metros. Estas opciones son:
		-D : Indica que se desea realizar una triangulaci�n de Delaunay.
		-ND : Indica que se desea realizar una triangulaci�n no de
			Delaunay.
		-NUT : Indica que no se desea ver los tri�ngulos y l�neas auxiliares
			utilizados para la triangulaci�n (l�neas de mar).
		-UT : Indica que se desea ver los tri�ngulos y l�neas auxiliares
			utilizados para la triangulaci�n (l�neas de mar).
	
	Por defecto, sin especificar ninguna opci�n, el programa es ejecutado
	con las opciones -D y -NUT.
        



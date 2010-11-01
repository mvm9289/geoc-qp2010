GEOC Ejercicio 4 Laboratorio

    Miguel Angel Vico Moya


**Aclaraciones del c�digo:**

    ��IMPORTANTE: VER PUNTO (4) DE ACLARACIONES ADICIONALES!!

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
    
        Para encontrar dentro de qu� tri�ngulo cae cada nuevo punto que se va
        analizando, y para obtener de forma sencilla el conjunto de tri�ngulos
        resultante de la triangulaci�n, se ha optado por una estructura en forma
        de �rbol d�nde cada nodo representa un tri�ngulo que contiene a sus 3
        tri�ngulos internos en todo momento.
        
        Por lo tanto disponemos de una estructura que, partiendo del tri�ngulo
        contenedor, a medida que nos adentramos en el �rbol vamos recorriendo
        todos los tri�ngulos que han ido resultando y que contienen tr�angulos
        m�s peque�os.
        
        Al llegar a las ra�ces del �rbol (s�n los �nicos nodos que no tienen
        nodos m�s internos), encontramos la representaci�n de los tri�ngulos m�s
        internos (los que realmente nos interesan en la triangulaci�n).
        
        Dicha estructura nos permite realizar una b�squeda de sobre qu�
        tri�ngulo cae el nuevo punto a analizar de forma r�pida y sencilla, ya
        que �nicamente debemos seguir una de las tres direcciones que nos da a
        elegir cada nodo del �rbol hasta llegar a una raiz.
        
        Puesto que las ra�ces del �rbol representan los tri�ngulos m�s internos
        de la triangulaci�n, la misma estructura mantiene actualizada una lista
        con dichos tri�ngulos de forma que cuando se pida obtener la
        triangulaci�n obtenida no haya que realizar ning�n c�lculo.
        
        
    __Otras consideraciones:__
    
        (1) Para dibujar los tri�ngulos resultantes se ha empleado la estructura
        de Polygon de forma que se vea de forma m�s clara las l�neas que conforman
        la triangulaci�n.
        
        (2) Tambi�n se ha exagerado la coordenada en Z de los puntos para que
        pueda verse con m�s detalle el relieve que estos puedan tener.
        
        (3) Se ha a�adido un fichero de entrada sencillo para probar la pr�ctica.
        
        (4) Se ha a�aido la l�nea #define WITHOUT_EXTERN_LINES en
        include/triangulation.h �til ver o no las l�neas generadas por el
        tri�ngulo contenedor con el resto de puntos. Para verlas (o no) comentar
        o descomentar dicha l�nea y recompilar la pr�ctica.
        



GEOC Ejercicio 4 Laboratorio

    Miguel Angel Vico Moya


**Aclaraciones del código:**

    ¡¡IMPORTANTE: VER PUNTO (4) DE ACLARACIONES ADICIONALES!!

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
    
        Para encontrar dentro de qué triángulo cae cada nuevo punto que se va
        analizando, y para obtener de forma sencilla el conjunto de triángulos
        resultante de la triangulación, se ha optado por una estructura en forma
        de árbol dónde cada nodo representa un triángulo que contiene a sus 3
        triángulos internos en todo momento.
        
        Por lo tanto disponemos de una estructura que, partiendo del triángulo
        contenedor, a medida que nos adentramos en el árbol vamos recorriendo
        todos los triángulos que han ido resultando y que contienen tríangulos
        más pequeños.
        
        Al llegar a las raíces del árbol (són los únicos nodos que no tienen
        nodos más internos), encontramos la representación de los triángulos más
        internos (los que realmente nos interesan en la triangulación).
        
        Dicha estructura nos permite realizar una búsqueda de sobre qué
        triángulo cae el nuevo punto a analizar de forma rápida y sencilla, ya
        que únicamente debemos seguir una de las tres direcciones que nos da a
        elegir cada nodo del árbol hasta llegar a una raiz.
        
        Puesto que las raíces del árbol representan los triángulos más internos
        de la triangulación, la misma estructura mantiene actualizada una lista
        con dichos triángulos de forma que cuando se pida obtener la
        triangulación obtenida no haya que realizar ningún cálculo.
        
        
    __Otras consideraciones:__
    
        (1) Para dibujar los triángulos resultantes se ha empleado la estructura
        de Polygon de forma que se vea de forma más clara las líneas que conforman
        la triangulación.
        
        (2) También se ha exagerado la coordenada en Z de los puntos para que
        pueda verse con más detalle el relieve que estos puedan tener.
        
        (3) Se ha añadido un fichero de entrada sencillo para probar la práctica.
        
        (4) Se ha añaido la línea #define WITHOUT_EXTERN_LINES en
        include/triangulation.h útil ver o no las líneas generadas por el
        triángulo contenedor con el resto de puntos. Para verlas (o no) comentar
        o descomentar dicha línea y recompilar la práctica.
        



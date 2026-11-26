# Análisis de tablas de dispersión

* *Alberto Martínez Martín, amm00530@red.ujaen.es*
* *Maitena María Rosa Fiedler, mmrf0008@red.ujaen.es*

## Tamaño de tabla: 206879
| función | máximo de colisiones |        supera 10 colisiones         | factor de carga | promedio de colisiones |
|---------|:--------------------:|:-----------------------------------:|:---------------:|:----------------------:|
| hash3   |          120694       |                 113                 |        0.649839        |     0.897767    |
| hash2   |       78790          |           64                        |     0.649839           |          0.586069 |
| hash1 |   247533 |554| 0.649839|1.84124|

## Tamaño de tabla: 197741
 unción | máximo de colisiones | supera 10 colisiones | factor de carga | promedio de colisiones |
|--------|:--------------------:|:--------------------:|:---------------:|:----------------------:|
| hash1  |        643683         |       600          |     0.679869       |      4.78795                  |
| hash2  |        97112         |       102          |     0.679869       |    0.722355                    |
| hash3  |        142825         |       187          |     0.679869       |            1.06239            |

## Justificación de la configuración elegida
Hemos optado por el factor de carga 0.65 y la fucnión hash2 dado que da los valores mas bajos en todas las areas estudiadas. Es aquella que nos 
da un promedio de colisiones menor y  un menor numero de veces que supera las 10 coalisiones


## Comparación de tiempos dados en milisegundos
Tiempo en realizar las búsquedas en la Tabla Hash:
Tiempo en realizar las búsquedas en la Lista:
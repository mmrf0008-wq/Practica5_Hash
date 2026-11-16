//
// Created by almar on 16/11/2026.
//

#ifndef PRACTICA5_HASH_UTM_H
#define PRACTICA5_HASH_UTM_H


class UTM {
private:
    double x;
    double y;
public:
    UTM():x(0),y(0){}
    UTM(double x, double y): x(x),y(y){}
    ~UTM();
    double distancia2(UTM otra);
};


#endif //PRACTICA5_HASH_UTM_H
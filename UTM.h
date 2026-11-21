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
    UTM();
    UTM(double x, double y);
    UTM(const UTM &orig);
    ~UTM();
    void setx(double x);
    void sety(double y);
    double getx() const;
    double gety() const;
    double distancia2(const UTM &otra);
};


#endif //PRACTICA5_HASH_UTM_H
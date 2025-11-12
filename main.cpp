#include <iostream>
#include <fstream>
#include "listas.hpp"
#include "rwstring.hpp"

using namesapce std;

struct Registro{
    string accion;
    int plazo;
    string bolsa;
    float preciouni;
    int cantidad;
};

const int laccion = 12;
const int lbolsa = 14;

struct Operacion {
    string tipo;   // "COMPRA" o "VENTA"
    string accion;
    int cantidad;
};

struct Bolsa {
    string bolsa;
    float totalDinero = 0;
    float resultado = 0;
    Nodo<Operacion> *lisop = nullptr;
};

struct Plazo {
    int codigo;
    string nombre;
    int cantVentas = 0;
    int cantCompras = 0;
    Nodo<Bolsa> *lisbolsas = nullptr;
};

fstream& operator >> (fstream &fs, Registro &reg) {
    reg.accion = readstring(fs, laccion);
    fs.read(reinterpret_cast<char*>(&reg.plazo), sizeof(reg.plazo));
    reg.bolsa = readstring(fs, lbolsa);
    fs.read(reinterpret_cast<char*>(&reg.preciouni), sizeof(reg.preciouni));
    fs.read(reinterpret_cast<char*>(&reg.cantidad), sizeof(reg.cantidad));
    return fs;
}

ostream& operator << (ostream &os, const Operacion &op) {
    os << op.tipo << "\t" << op.accion << "\t" << op.cantidad;
    return os;
}

ostream& operator << (ostream &os, const Bolsa &b) {
    os << b.bolsa << "\t" << b.totalDinero << "\t" << b.resultado;
    return os;
}

ostream& operator << (ostream &os, const Plazo &p) {
    os << p.codigo << "\t" << p.nombre << "\t" << p.cantVentas << "\t" << p.cantCompras;
    return os;
}

int critPlazo(Plazo a, Plazo b) {
    return a.codigo - b.codigo;
}

int critBolsa(Bolsa a, Bolsa b) {
    return a.bolsa.compare(b.bolsa);
}

string nombrePlazo(int cod) {
    switch (cod) {
        case 0: return "CI (contado inmediato)";
        case 1: return "24hs";
        case 2: return "48hs";
        case 3: return "72hs";
        default: return "Desconocido";
    }
}

void listarPlazos(Nodo<Plazo> *lis) {
    while (lis) {
        cout << *lis << endl;

        Nodo<Bolsa> *lb = lis->dato.lisbolsas;
        while (lb) {
            cout << *lb << endl;
            cout << "      Tipo      | Accion       | Cantidad" << endl;
            mostrar(lb->dato.lisop);
            lb = lb->sig;
        }

        cout << "----------------------------------------" << endl;
        lis = lis->sig;
    }
}

int main()
{
    fstream arch;
    archi.open("Operaciones.bin", ios::in | ios::binary);
    if (!archi) {
        cout << "No se pudo abrir el archivo Operaciones.bin" << endl;
        return EXIT_FAILURE;
    }
    Registro reg;
    Plazo plz;
    Nodo<Plazo> *lisPlazos = nullptr;
    Nodo<Plazo> *pPlz;
    Bolsa b;
    Nodo<Bolsa> *pBolsa;
    Operacion op;

    while(archi>>reg){
        plz.codigo = reg.plazo;
        plz.nombre = nombrePlazo(reg.plazo);
        pPlz = insertar_unico(plz, lisPlazos, critPlazo);
        if(reg.cantidad>0){
            pPlz->dato.cantVentas++;
        }
        else{
            pPlz->dato.cantCompras++;
        }

        b.bolsa = reg.bolsa;
        pBolsa = insertar_unico(b, pPlz->dato.lisbolsas, critBolsa);
        pBolsa->dato.totalDinero += reg.preciouni * reg.cantidad;
        pBolsa->dato.resultado += reg.preciouni * reg.cantidad;

        op.tipo = (reg.cantidad < 0) ? "COMPRA" : "VENTA";
        op.accion = reg.accion;
        op.cantidad = reg.cantidad;
        agregar(pBolsa->dato.lisop, op);
    }

    arch.close();

    cout << "=== LISTADO POR PLAZO / BOLSA / OPERACION ===" << endl << endl;
    listarPlazos(lisPlazos);

    return 0;
}

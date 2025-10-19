#include <iostream> 

template <typename T>
class MatrizBase {
protected:
    // Dimensiones de la matriz
    int _filas;
    int _columnas;

public:
    // Constructor base
    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}

    // Destructor virtual (necesario para clases base)
    virtual ~MatrizBase() {}

    // Metodo para rellenar la matriz
    virtual void cargarValores() = 0;

    // Metodo para mostrar la matriz
    virtual void imprimir() const = 0;

    // Metodo para sumar matrices (devuelve una nueva matriz)
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;

    // Metodos de Acceso
    int obtenerFilas() const { return _filas; }
    int obtenerColumnas() const { return _columnas; }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

public:
    // 1. Constructor
    MatrizDinamica(int filas, int columnas) 
        : MatrizBase<T>(filas, columnas) {
        
        std::cout << "-> Constructor: Creando Matriz Dinamica (" << this->_filas << "x" << this->_columnas << ")..." << std::endl;
        
        // Asigna memoria para las filas (arreglo de punteros)
        _datos = new T*[this->_filas];
        
        // Asigna memoria para las columnas de cada fila
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            // Inicializa valores a 0 (o el default de T)
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = T();
            }
        }
    }

    // 2. Destructor
    virtual ~MatrizDinamica() {
        std::cout << "-> Destructor: Liberando memoria de Matriz Dinamica (" << this->_filas << "x" << this->_columnas << ")..." << std::endl;
        
        if (_datos != nullptr) {
            // Libera cada fila
            for (int i = 0; i < this->_filas; ++i) {
                delete[] _datos[i];
            }
            // Libera el arreglo de filas
            delete[] _datos;
            _datos = nullptr;
        }
    }

    // 3. Constructor de Copia (Copia profunda)
    MatrizDinamica(const MatrizDinamica<T>& otra) 
        : MatrizBase<T>(otra._filas, otra._columnas) {
        
        std::cout << "-> Constructor de Copia (Deep Copy) de Matriz Dinamica..." << std::endl;

        // Asigna memoria nueva
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            // Copia los valores
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }

    // 4. Operador de Asignacion por Copia (Copia profunda)
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        std::cout << "-> Operador de Asignacion por Copia (Deep Copy)..." << std::endl;
        
        // Evita auto-asignacion (a = a)
        if (this == &otra) {
            return *this;
        }

        // Libera memoria antigua de 'this'
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; ++i) {
                delete[] _datos[i];
            }
            delete[] _datos;
        }

        // Actualiza dimensiones
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;

        // Asigna memoria nueva y copia los datos
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
        return *this;
    }

    // 5. Constructor de Movimiento (Regla de 5)
    // Transfiere los recursos (roba el puntero) de una matriz temporal
    MatrizDinamica(MatrizDinamica<T>&& otra) noexcept
        : MatrizBase<T>(otra._filas, otra._columnas), _datos(otra._datos) {
        
        std::cout << "-> Constructor de Movimiento (Robando recursos)..." << std::endl;
        // Deja la matriz 'otra' en estado nulo
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }

    // 6. Operador de Asignacion por Movimiento (Regla de 5)
    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& otra) noexcept {
        std::cout << "-> Operador de Asignacion por Movimiento (Robando recursos)..." << std::endl;
        
        if (this == &otra) {
            return *this;
        }

        // Libera memoria propia
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; ++i) {
                delete[] _datos[i];
            }
            delete[] _datos;
        }

        // Roba los recursos de 'otra'
        _datos = otra._datos;
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;

        // Deja 'otra' en estado nulo
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;

        return *this;
    }

    virtual void cargarValores() override {
        std::cout << "Cargando valores para Matriz Dinamica (" << this->_filas << "x" << this->_columnas << "):" << std::endl;
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << "  Ingrese valor para [" << i << "][" << j << "]: ";
                std::cin >> _datos[i][j];
            }
        }
    }

    virtual void imprimir() const override {
        std::cout << "Imprimiendo Matriz Dinamica (" << this->_filas << "x" << this->_columnas << "):" << std::endl;
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "  | ";
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        std::cout << "\n*** FUNCION SUMAR AUN NO IMPLEMENTADA EN PARTE 1 ***\n" << std::endl;
        
        // Para que el codigo compile, devolvemos nullptr.
        return nullptr;
    }
};

int main() {
    std::cout << "--- Sistema generico de Algebra Lineal (PARTE 1) ---" << std::endl;
    std::cout << ">> Demostracion de MatrizDinamica (Tipo FLOAT) <<" << std::endl;

    // 1. Creacion
    // Puntero base a un objeto derivado
    MatrizBase<float>* ptrMatrizA = new MatrizDinamica<float>(2, 2);

    // 2. Carga de valores
    ptrMatrizA->cargarValores();

    // 3. Impresion
    std::cout << "\nMatriz A:" << std::endl;
    ptrMatrizA->imprimir();

    // 4. Prueba de copia
    std::cout << "\n--- Demostracion de Copia ---" << std::endl;
    
    // Creamos un objeto concreto para probar el operador de asignacion
    MatrizDinamica<float> matrizB(1, 1); 
    matrizB = *(static_cast<MatrizDinamica<float>*>(ptrMatrizA)); // Asignacion por copia
    
    std::cout << "\nMatriz B (por asignacion):" << std::endl;
    matrizB.imprimir();

    // Probamos el constructor de copia
    MatrizDinamica<float> matrizC = matrizB; 
    std::cout << "\nMatriz C (por constructor de copia):" << std::endl;
    matrizC.imprimir();


    std::cout << "\n>> Saliendo de main... se llamaran a los destructores. <<" << std::endl;
    
    // 5. Limpieza de memoria
    delete ptrMatrizA;
    
    return 0;
}

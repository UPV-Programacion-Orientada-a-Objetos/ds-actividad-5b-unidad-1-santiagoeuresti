#include <iostream> 
#include <stdexcept> 

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}
    virtual ~MatrizBase() {} 

    virtual void cargarValores() = 0;
    virtual void imprimir() const = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual T obtenerValor(int f, int c) const = 0;
    virtual void asignarValor(int f, int c, T valor) = 0;

    int obtenerFilas() const { return _filas; }
    int obtenerColumnas() const { return _columnas; }
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

    // Utilitarios de memoria
    void asignarMemoria(int filas, int columnas) {
        _datos = new T*[filas];
        for (int i = 0; i < filas; ++i) {
            _datos[i] = new T[columnas];
            for (int j = 0; j < columnas; ++j) {
                _datos[i][j] = T(); 
            }
        }
    }

    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; ++i) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }

public:
    // 1. Constructor
    MatrizDinamica(int filas, int columnas) 
        : MatrizBase<T>(filas, columnas), _datos(nullptr) {
        std::cout << "-> Constructor: Creando Matriz Dinamica (" << this->_filas << "x" << this->_columnas << ")..." << std::endl;
        asignarMemoria(this->_filas, this->_columnas);
    }

    // 2. Destructor
    virtual ~MatrizDinamica() {
        std::cout << "-> Destructor: Liberando memoria de Matriz Dinamica (" << this->_filas << "x" << this->_columnas << ")..." << std::endl;
        liberarMemoria();
    }

    // 3. Constructor de Copia 
    MatrizDinamica(const MatrizDinamica<T>& otra) 
        : MatrizBase<T>(otra._filas, otra._columnas) {
        std::cout << "-> Constructor de Copia (Deep Copy) de Matriz Dinamica..." << std::endl;
        asignarMemoria(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }

    // 4. Operador de Asignacion por Copia
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        std::cout << "-> Operador de Asignacion por Copia (Deep Copy)..." << std::endl;
        if (this == &otra) {
            return *this;
        }
        liberarMemoria();
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        asignarMemoria(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
        return *this;
    }

    // 5.a. Constructor de Movimiento
    MatrizDinamica(MatrizDinamica<T>&& otra) noexcept
        : MatrizBase<T>(otra._filas, otra._columnas), _datos(otra._datos) {
        std::cout << "-> Constructor de Movimiento (Robando recursos)..." << std::endl;
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
    }

    // 5.b. Operador de Asignacion por Movimiento
    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& otra) noexcept {
        std::cout << "-> Operador de Asignacion por Movimiento (Robando recursos)..." << std::endl;
        if (this == &otra) {
            return *this;
        }
        liberarMemoria();
        _datos = otra._datos;
        this->_filas = otra._filas;
        this->_columnas = otra._columnas;
        otra._datos = nullptr;
        otra._filas = 0;
        otra._columnas = 0;
        return *this;
    }

    // Implementacion de Metodos Virtuales

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
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    virtual T obtenerValor(int f, int c) const override {
        return _datos[f][c];
    }

    virtual void asignarValor(int f, int c, T valor) override {
        if (f >= 0 && f < this->_filas && c >= 0 && c < this->_columnas) {
            _datos[f][c] = valor;
        }
    }

    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        std::cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)" << std::endl;
        
        if (this->_filas != otra.obtenerFilas() || this->_columnas != otra.obtenerColumnas()) {
            std::cout << "ERROR: Dimensiones no coinciden para la suma." << std::endl;
            return nullptr; 
        }

        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);

        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra.obtenerValor(i, j);
            }
        }
        return resultado;
    }
};


template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N]; 

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        std::cout << "-> Constructor: Creando Matriz Estatica (" << M << "x" << N << ")..." << std::endl;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                _datos[i][j] = T();
            }
        }
    }

    virtual ~MatrizEstatica() {
        std::cout << "-> Destructor: Destruyendo Matriz Estatica (" << M << "x" << N << ")... (memoria automatica)" << std::endl;
    }

    // Implementacion de Metodos Virtuales 

    virtual void cargarValores() override {
        std::cout << "Cargando valores para Matriz Estatica (" << M << "x" << N << "):" << std::endl;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << "  Ingrese valor para [" << i << "][" << j << "]: ";
                std::cin >> _datos[i][j];
            }
        }
    }

    virtual void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "| ";
            for (int j = 0; j < N; ++j) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }

    virtual T obtenerValor(int f, int c) const override {
        return _datos[f][c];
    }
    
    virtual void asignarValor(int f, int c, T valor) override {
         if (f >= 0 && f < M && c >= 0 && c < N) {
            _datos[f][c] = valor;
        }
    }

    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        std::cout << "(La suma es manejada por el metodo virtual de MatrizEstatica)" << std::endl;

        if (this->_filas != otra.obtenerFilas() || this->_columnas != otra.obtenerColumnas()) {
            std::cout << "ERROR: Dimensiones no coinciden para la suma." << std::endl;
            return nullptr;
        }

        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra.obtenerValor(i, j);
            }
        }
        return resultado;
    }
};


template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& a, const MatrizBase<T>& b) {
    return a.sumar(b);
}



int main() {
    std::cout << "--- Sistema generico de Algebra Lineal ---" << std::endl;
    
    std::cout << "\n>> Demostracion de Genericidad (Tipo FLOAT) <<" << std::endl;

    MatrizBase<float>* ptrMatrizA = nullptr;
    MatrizBase<float>* ptrMatrizB = nullptr;
    MatrizBase<float>* ptrMatrizC = nullptr;

    // 1. Creacion de Matriz Dinamica (3x2)
    ptrMatrizA = new MatrizDinamica<float>(3, 2);
    ptrMatrizA->asignarValor(0, 0, 1.5f);
    ptrMatrizA->asignarValor(0, 1, 2.0f);
    ptrMatrizA->asignarValor(1, 0, 0.0f);
    ptrMatrizA->asignarValor(1, 1, 1.0f);
    ptrMatrizA->asignarValor(2, 0, 4.5f);
    ptrMatrizA->asignarValor(2, 1, 3.0f);
    
    std::cout << "\nA =" << std::endl;
    ptrMatrizA->imprimir();

    // 2. Creacion de Matriz Estatica (3x2)
    ptrMatrizB = new MatrizEstatica<float, 3, 2>();
    ptrMatrizB->asignarValor(0, 0, 0.5f);
    ptrMatrizB->asignarValor(0, 1, 1.0f);
    ptrMatrizB->asignarValor(1, 0, 2.0f);
    ptrMatrizB->asignarValor(1, 1, 3.0f);
    ptrMatrizB->asignarValor(2, 0, 1.0f);
    ptrMatrizB->asignarValor(2, 1, 1.0f);

    std::cout << "\nB =" << std::endl;
    ptrMatrizB->imprimir();

    // 3. Operacion Polimorfica (Suma)
    std::cout << "\nSUMANDO: Matriz C = A + B ..." << std::endl;
    
    ptrMatrizC = *ptrMatrizA + *ptrMatrizB;
    
    if (ptrMatrizC != nullptr) {
        std::cout << "\nMatriz Resultado C (3x2, Tipo FLOAT):" << std::endl;
        ptrMatrizC->imprimir();
    }

    std::cout << "\n>> Demostracion de Limpieza de Memoria <<" << std::endl;
    
    std::cout << "Llamando al destructor de C..." << std::endl;
    delete ptrMatrizC; 
    
    std::cout << "Llamando al destructor de A..." << std::endl;
    delete ptrMatrizA; 
    
    std::cout << "Llamando al destructor de B..." << std::endl;
    delete ptrMatrizB; 

    std::cout << "\nSistema cerrado." << std::endl;
    return 0;
}

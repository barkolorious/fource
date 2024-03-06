#include <iostream>
#include <complex>
#include <valarray>
#include <random>
#include <iomanip>
#include <fstream>
using namespace std;
#define number complex<double>
#define vector valarray<number>
#define matrix valarray<vector>

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(-(1e9+7), +(1e9+7));

class fource{
  friend ostream &operator<<(ostream&, const fource&);
  friend istream &operator>>(istream&, fource&);

  private:    
    const double pi = acos(-1.0);
    const number zero = acos(1.0);
    const number I = number(0, 1);

    matrix A, invA;
    int N;
    bool loadedMatrix = false;

    int msb(int);
    vector fft(vector);
    vector ifft(vector);
    matrix inverse(matrix);
  
  public:
    string data;
    vector code;

    fource(string);
    fource(int);
    void newData(string);
    void generateMatrix();
    void encrypt(); 
    string decrypt();
    void write(const char*);
    void read(const char*);
    void saveMatrix(const char*);
    void loadMatrix(const char*);
    ~fource();
};

fource::fource(string message){
  data = message;
  N = msb(data.length());
}

fource::fource(int x = 255){
  N = msb(x);
}

/**
 * @brief Add a new data to encrypt to the ```fource``` object.
 * @param message Type: ```string``` - Data to be put in the ```fource``` object.
 * @return ```void```
 */
void fource::newData(string message){
  data = message;
  if(N <= data.length()){
    N = msb(data.length());
    loadedMatrix = false;
  }
}

/**
 * @brief Generate the matrix for future encryption usage.
 * @param void
 * @return ```void```
 */
void fource::generateMatrix(){
  matrix f(vector(zero, N), N);
  dis.reset();
  for(int c = 0; c < N; c++){
    for(int r = 0; r < N; r++){
      f[r][c] = number(dis(gen), dis(gen));
    }
  }
    
  A = matrix(vector(zero, N), N);
  for(int c = 0; c < N; c++){
    vector f_c(zero, N);
    for(int r = 0; r < N; r++){
      f_c[r] = f[r][c];
    }
    vector A_c = fft(f_c);
    for(int r = 0; r < N; r++){
      A[r][c] = A_c[r];
    }
  }
  invA = inverse(A);
  loadedMatrix = true;
}

/**
 * @brief Calculate the inverse of the matrix ```M```
 * @param M Type: ```valarray<valarray<complex<double>>>``` - Matrix to calculate the inverse of.
 * @return ```valarray<valarray<complex<double>>>```  - Inverse of matrix ```M```.
 */
matrix fource::inverse(matrix M){
  const int N = M.size();
  matrix id(vector(zero, N), N);
  for(int i = 0; i < N; i++) id[i][i] = number(1, 0);
  for(int r = N - 1; r > 0; r--) {
    if(abs(M[r - 1][0]) < abs(M[r][0])){
      swap(M[r - 1], M[r]);
      swap(id[r - 1], id[r]);
    }
  }
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      if(i != j){
        number c = M[j][i] / M[i][i];
        M[j] -= M[i] * c;
        id[j] -= id[i] * c;
      }
    }
  }
  for(int r = 0; r < N; r++){
    number c = M[r][r];
    M[r] /= c;
    id[r] /= c;
  }
  return id;
}

/**
 * @brief Calculate two times the most significant bit of ```x```
 * @param x Type: ```int``` - Number to use
 * @return Two times the most significant bit of ```x```
 */
int fource::msb(int x){
  int n = 0;
  while(x >> n) n++;
  return 1 << n;
}

/**
 * @brief Take the discrete Fourier transform of ```f``` using FFT.
 * @param f Type: ```valarray<complex<double>>``` - Funtion to take the discrete Fourier transform of.
 * @return ```valarray<complex<double>>``` - Discrete Fourier transform of function ```f```
 */
vector fource::fft(vector f){
  if(f.size() == 1) return f;

  const int n = f.size();
  double phi = -2 * pi / n;
  number w = exp(phi * I);

  vector f_even = f[slice(0, n / 2, 2)];
  vector f_odd  = f[slice(1, n / 2, 2)];

  vector F_even = fft(f_even);
  vector F_odd  = fft(f_odd);

  vector F = vector(zero, N);

  for(int i = 0; i < n / 2; i++){
    F[i]         = F_even[i] + F_odd[i] * pow(w, i);
    F[i + n / 2] = F_even[i] - F_odd[i] * pow(w, i);
  }

  return F;
}

/**
 * @brief Encrypt the data from ```data```. 
 * @param void
 * @return ```void```
 */
void fource::encrypt(){
  if(!loadedMatrix) 
    this->generateMatrix();

  vector s(zero, N);
  s[0] = data.length();
  for(int i = 1; i < N; i++) 
    s[i] = data[(i - 1) % data.length()];

  vector B = fft(s);

  code = vector(zero, N);
  for(int r = 0; r < N; r++) 
    code[r] = (invA[r] * B).sum();
}

/**
 * @brief Take the inverse discrete Fourier transform of ```F``` using IFFT. IFFT[F] = ((FFT[F*])*) / N
 * @param F Type: ```valarray<complex<double>>``` - Function to take the inverse discrete Fourier transform of.
 * @return ```valarray<complex<double>>``` - Inverse discrete Fourier transform of function ```F``` 
 */
vector fource::ifft(vector F){
  const int N = F.size();
  for(number& z : F) z = conj(z);
  vector f = fft(F);
  for(number& z : f) z = conj(z);
  f /= N;
  return f;
}

/**
 * @brief Decrypt the data from ```code```.
 * @param void
 * @return ```string``` - Decrypted data
 */
string fource::decrypt(){
  vector B(zero, N);
  for(int r = 0; r < N; r++) 
    B[r] = (A[r] * code).sum();

  vector s = ifft(B);
  int len = round(round(s[0].real()));

  string m = "";
  for(int i = 0; i < len; i++)
    m += char(round(round(s[i + 1].real())));

  return m;
}

/**
 * @brief Write ```code``` to file ```file```
 * @param file Type: ```const char*``` - Path of the file where ```code``` will be saved. ```code``` will be saved in binary encoding.
 * @return ```void```
 */
void fource::write(const char* file){
  ofstream fout(file, ios::out | ios::binary);
  int size = code.size();
  fout.write((char*)(&size), sizeof(int));
  for(number z : code){
    double re = z.real(), im = z.imag();
    fout.write((char*)(&re), sizeof(double));
    fout.write((char*)(&im), sizeof(double));
  }
  fout.close();
}

/**
 * @brief Read ```code``` from file ```file```
 * @param file Type: ```const char*``` - Path of the file where ```code``` will be read from. The file needs to be in binary encoding.
 * @return ```void```
 */
void fource::read(const char* file){
  ifstream fin(file, ios::in | ios::binary);
  int size;
  fin.read((char*)(&size), sizeof(int));
  vector x(zero, size);
  for(number& z : x){
    double re, im;
    fin.read((char*)(&re), sizeof(double));
    fin.read((char*)(&im), sizeof(double));
    z += number(re, im);
  }
  code = x;
  fin.close();
}

/**
 * @brief Save key matrices ```A``` and ```invA``` to file ```file```
 * @param file Type: ```const char*``` - Path of the file where ```A``` and ```invA``` will be saved. ```A``` and ```invA``` will be saved in binary encoding.
 * @return ```void```
 */
void fource::saveMatrix(const char* file){
  ofstream fout(file, ios::out | ios::binary);
  int size = A.size();
  fout.write((char*)(&size), sizeof(int));
  for(vector row : A){
    for(number z : row){
      double re = z.real(), im = z.imag();
      fout.write((char*)(&re), sizeof(double));
      fout.write((char*)(&im), sizeof(double));
    }
  }
  for(vector row : invA){
    for(number z : row){
      double re = z.real(), im = z.imag();
      fout.write((char*)(&re), sizeof(double));
      fout.write((char*)(&im), sizeof(double));
    }
  }
  fout.close();
}

/**
 * @brief Read key matrices ```A``` and ```invA``` from file ```file```
 * @param file Type: ```const char*``` - Path of the file where ```A``` and ```invA``` will be read from. The file needs to be in binary encoding.
 * @return ```void```
 */
void fource::loadMatrix(const char* file){
  ifstream fin(file, ios::in | ios::binary);
  int size;
  fin.read((char*)(&size), sizeof(int));
  A = matrix(vector(zero, size), size);
  for(vector& row : A){
    for(number& z : row){
      double re, im;
      fin.read((char*)(&re), sizeof(double));
      fin.read((char*)(&im), sizeof(double));
      z = number(re, im);
    }
  }
  invA = matrix(vector(zero, size), size);
  for(vector& row : invA){
    for(number& z : row){
      double re, im;
      fin.read((char*)(&re), sizeof(double));
      fin.read((char*)(&im), sizeof(double));
      z = number(re, im);
    }
  }
  loadedMatrix = true;
  fin.close();
}

/** 
 * @brief Output ```cr```'s encrypted data to ```ostr```
 * @param ostr Type: ```ostream&``` - Output stream that will print ```cr```
 * @param cr Type: ```fource``` - FOURCE object to be outputed
 * @return ```ostr```
 **/
ostream& operator<<(ostream& ostr, const fource& cr){
  ostr << "\nData: " << cr.data << '\n';
  ostr << showpos << fixed << setprecision(8);
  for(int i = 0; i < cr.code.size(); i++){
    ostr << "| " << setw(15) << left  << cr.code[i].real() 
         << ' '  << setw(15) << right << cr.code[i].imag() << "i |\n";
  }
  return ostr;
}

/**
 * @brief Read ```istr```'s buffer as a new data for ```cr```
 * @param istr Type: ```istream&``` - Input stream that will provide the new data
 * @param cr Type: ```fource&``` - FOURCE object to be updated
 * @return ```istr``` 
 */
istream& operator>>(istream& istr, fource& cr){
  string message;
  istr >> message;
  cr.newData(message);
  return istr;
}

fource::~fource(){}

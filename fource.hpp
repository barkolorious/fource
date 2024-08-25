#include <iostream>
#include <complex>
#include <valarray>
#include <random>
#include <iomanip>
#include <fstream>

class fource{
  friend std::ostream &operator<<(std::ostream&, const fource&);
  friend std::istream &operator>>(std::istream&, fource&);

  private:    
    const double pi = acos(-1.0);
    const std::complex<double> zero = acos(1.0);
    const std::complex<double> I = std::complex<double>(0, 1);

    std::valarray<std::valarray<std::complex<double>>> A, invA;
    int N;
    bool loadedMatrix = false;

    int msb(int);
    std::valarray<std::complex<double>> fft(std::valarray<std::complex<double>>);
    std::valarray<std::complex<double>> ifft(std::valarray<std::complex<double>>);
    std::valarray<std::valarray<std::complex<double>>> inverse(std::valarray<std::valarray<std::complex<double>>>);
  
  public:
    std::string data;
    std::valarray<std::complex<double>> code;

    fource(std::string);
    fource(int);
    void newData(std::string);
    void generateMatrix();
    void encrypt(); 
    std::string decrypt();
    void write(const char*);
    void read(const char*);
    void saveMatrix(const char*);
    void loadMatrix(const char*);
    ~fource();
};

fource::fource(std::string message){
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
void fource::newData(std::string message){
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
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(-(1e9+7), +(1e9+7));

  std::valarray<std::valarray<std::complex<double>>> f(std::valarray<std::complex<double>>(zero, N), N);
  dis.reset();
  for(int c = 0; c < N; c++){
    for(int r = 0; r < N; r++){
      f[r][c] = std::complex<double>(dis(gen), dis(gen));
    }
  }
    
  A = std::valarray<std::valarray<std::complex<double>>>(std::valarray<std::complex<double>>(zero, N), N);
  for(int c = 0; c < N; c++){
    std::valarray<std::complex<double>> f_c(zero, N);
    for(int r = 0; r < N; r++){
      f_c[r] = f[r][c];
    }
    std::valarray<std::complex<double>> A_c = fft(f_c);
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
std::valarray<std::valarray<std::complex<double>>> fource::inverse(std::valarray<std::valarray<std::complex<double>>> M){
  const int N = M.size();
  std::valarray<std::valarray<std::complex<double>>> id(std::valarray<std::complex<double>>(zero, N), N);
  for(int i = 0; i < N; i++) id[i][i] = std::complex<double>(1, 0);
  for(int r = N - 1; r > 0; r--) {
    if(abs(M[r - 1][0]) < abs(M[r][0])){
      swap(M[r - 1], M[r]);
      swap(id[r - 1], id[r]);
    }
  }
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      if(i != j){
        std::complex<double> c = M[j][i] / M[i][i];
        M[j] -= M[i] * c;
        id[j] -= id[i] * c;
      }
    }
  }
  for(int r = 0; r < N; r++){
    std::complex<double> c = M[r][r];
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
std::valarray<std::complex<double>> fource::fft(std::valarray<std::complex<double>> f){
  if(f.size() == 1) return f;

  const int n = f.size();
  double phi = -2 * pi / n;
  std::complex<double> w = exp(phi * I);

  std::valarray<std::complex<double>> f_even = f[std::slice(0, n / 2, 2)];
  std::valarray<std::complex<double>> f_odd  = f[std::slice(1, n / 2, 2)];

  std::valarray<std::complex<double>> F_even = fft(f_even);
  std::valarray<std::complex<double>> F_odd  = fft(f_odd);

  std::valarray<std::complex<double>> F = std::valarray<std::complex<double>>(zero, N);

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

  std::valarray<std::complex<double>> s(zero, N);
  s[0] = data.length();
  for(int i = 1; i < N; i++) 
    s[i] = data[(i - 1) % data.length()];

  std::valarray<std::complex<double>> B = fft(s);

  code = std::valarray<std::complex<double>>(zero, N);
  for(int r = 0; r < N; r++) 
    code[r] = (invA[r] * B).sum();
}

/**
 * @brief Take the inverse discrete Fourier transform of ```F``` using IFFT. IFFT[F] = ((FFT[F*])*) / N
 * @param F Type: ```valarray<complex<double>>``` - Function to take the inverse discrete Fourier transform of.
 * @return ```valarray<complex<double>>``` - Inverse discrete Fourier transform of function ```F``` 
 */
std::valarray<std::complex<double>> fource::ifft(std::valarray<std::complex<double>> F){
  const int N = F.size();
  for(std::complex<double>& z : F) z = conj(z);
  std::valarray<std::complex<double>> f = fft(F);
  for(std::complex<double>& z : f) z = conj(z);
  f /= N;
  return f;
}

/**
 * @brief Decrypt the data from ```code```.
 * @param void
 * @return ```string``` - Decrypted data
 */
std::string fource::decrypt(){
  std::valarray<std::complex<double>> B(zero, N);
  for(int r = 0; r < N; r++) 
    B[r] = (A[r] * code).sum();

  std::valarray<std::complex<double>> s = ifft(B);
  int len = round(round(s[0].real()));

  std::string m = "";
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
  std::ofstream fout(file, std::ios::out | std::ios::binary);
  int size = code.size();
  fout.write((char*)(&size), sizeof(int));
  for(std::complex<double> z : code){
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
  std::ifstream fin(file, std::ios::in | std::ios::binary);
  int size;
  fin.read((char*)(&size), sizeof(int));
  std::valarray<std::complex<double>> x(zero, size);
  for(std::complex<double>& z : x){
    double re, im;
    fin.read((char*)(&re), sizeof(double));
    fin.read((char*)(&im), sizeof(double));
    z += std::complex<double>(re, im);
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
  std::ofstream fout(file, std::ios::out | std::ios::binary);
  int size = A.size();
  fout.write((char*)(&size), sizeof(int));
  for(std::valarray<std::complex<double>> row : A){
    for(std::complex<double> z : row){
      double re = z.real(), im = z.imag();
      fout.write((char*)(&re), sizeof(double));
      fout.write((char*)(&im), sizeof(double));
    }
  }
  for(std::valarray<std::complex<double>> row : invA){
    for(std::complex<double> z : row){
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
  std::ifstream fin(file, std::ios::in | std::ios::binary);
  int size;
  fin.read((char*)(&size), sizeof(int));
  A = std::valarray<std::valarray<std::complex<double>>>(std::valarray<std::complex<double>>(zero, size), size);
  for(std::valarray<std::complex<double>>& row : A){
    for(std::complex<double>& z : row){
      double re, im;
      fin.read((char*)(&re), sizeof(double));
      fin.read((char*)(&im), sizeof(double));
      z = std::complex<double>(re, im);
    }
  }
  invA = std::valarray<std::valarray<std::complex<double>>>(std::valarray<std::complex<double>>(zero, size), size);
  for(std::valarray<std::complex<double>>& row : invA){
    for(std::complex<double>& z : row){
      double re, im;
      fin.read((char*)(&re), sizeof(double));
      fin.read((char*)(&im), sizeof(double));
      z = std::complex<double>(re, im);
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
std::ostream& operator<<(std::ostream& ostr, const fource& cr){
  ostr << "\nData: " << cr.data << '\n';
  ostr << std::showpos << std::fixed << std::setprecision(8);
  for(int i = 0; i < cr.code.size(); i++){
    ostr << "| " << std::setw(15) << std::left  << cr.code[i].real() 
         << ' '  << std::setw(15) << std::right << cr.code[i].imag() << "i |\n";
  }
  return ostr;
}

/**
 * @brief Read ```istr```'s buffer as a new data for ```cr```
 * @param istr Type: ```istream&``` - Input stream that will provide the new data
 * @param cr Type: ```fource&``` - FOURCE object to be updated
 * @return ```istr``` 
 */
std::istream& operator>>(std::istream& istr, fource& cr){
  std::string message;
  istr >> message;
  cr.newData(message);
  return istr;
}

fource::~fource(){}

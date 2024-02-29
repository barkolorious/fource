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
    void newMessage(string);
    void generateMatrix();
    void encrypt(); 
    string solve();
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

void fource::newMessage(string message){
  data = message;
  if(N <= data.length()){
    N = msb(data.length());
    loadedMatrix = false;
  }
}

void fource::generateMatrix(){
  //Anahtar oluşturma kodu

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

matrix fource::inverse(matrix M){
  //Matris'in tersini alma kodu
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

int fource::msb(int x){
  int n = 0;
  while(x >> n) n++;
  return 1 << n;
}

vector fource::fft(vector f){
  //Fast Fourier Transform - Hızlı Fourier Dönüşümü
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

void fource::encrypt(){
  //Şifreleme kodu
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

vector fource::ifft(vector F){
  const int N = F.size();
  for(number& z : F) z = conj(z);
  vector f = fft(F);
  for(number& z : f) z = conj(z);
  f /= N;
  return f;
}

string fource::solve(){
  //Deşifreleme kodu
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
 * @brief $\sin(5t)$
 * 
 * @param file File path to load the ```A``` from.
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
 * @brief Will output ```cr```'s encrypted data to ```ostr```
 * @param ostr Type: ```ostream&``` - Output stream that will print ```cr```
 * @param cr Type: ```fource``` - FOURCE object to be outputed
 * @return ```ostr```
 **/
ostream& operator<<(ostream& ostr, const fource& cr){
  ostr << "\nMessage: " << cr.data << '\n';
  ostr << showpos << fixed << setprecision(8);
  for(int i = 0; i < cr.code.size(); i++){
    ostr << "| " << setw(15) << left  << cr.code[i].real() 
         << ' '  << setw(15) << right << cr.code[i].imag() << "i |\n";
  }
  return ostr;
}

/**
 * @brief Will read ```istr```'s buffer as a new message for ```cr```
 * 
 * @param istr Input stream that will provide the new message
 * @param cr FOURCE object to be updated
 * @return ```istr``` 
 */
istream& operator>>(istream& istr, fource& cr){
  string message;
  istr >> message;
  cr.newMessage(message);
  return istr;
}

fource::~fource(){}

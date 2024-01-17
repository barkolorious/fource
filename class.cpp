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

    matrix A, invA;
    int N;
    bool loadedMatrix = false;

    int msb(int);
    vector dft(vector);
    vector idft(vector);
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
    for(int r = 0; r < N; r++) f_c[r] = f[r][c];
    vector A_c = dft(f_c);
    for(int r = 0; r < N; r++) A[r][c] = A_c[r];
  }
  invA = inverse(A);
  loadedMatrix = true;
}

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

int fource::msb(int x){
  int n = 0;
  while(x >> n) n++;
  return 1 << n;
}

vector fource::dft(vector f){
  const int N = f.size();
  vector F(zero, N);
  for(int k = 0; k < N; k++){
    for(int n = 0; n < N; n++){
      double phi = -2 * pi / N * k * n;
      number z = number(cos(phi), sin(phi));
      F[k] += f[n] * z;
    }
  }
  return F; 
}

void fource::encrypt(){
  if(!loadedMatrix) this->generateMatrix();
  vector s(zero, N);
  s[0] = data.length();
  for(int i = 1; i < N; i++) s[i] = data[(i - 1) % data.length()];
  vector B = dft(s);
  code = vector(zero, N);
  for(int r = 0; r < N; r++) code[r] = (invA[r] * B).sum();
}


vector fource::idft(vector F){
  const int N = F.size();
  vector f(zero, N);
  for(int n = 0; n < N; n++){
    for(int k = 0; k < N; k++){
      double phi = 2 * pi / N * k * n;
      number z = number(cos(phi), sin(phi));
      f[k] += F[n] * z;
    }
  }
  f /= N;
  return f; 
}

string fource::solve(){
  vector B(zero, N);
  for(int r = 0; r < N; r++) B[r] = (A[r] * code).sum();
  vector s = idft(B);
  int len = round(round(s[0].real()));
  string m = "";
  for(int i = 0; i < len; i++){
    m += char(round(round(s[i + 1].real())));
  }
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

ostream& operator<<(ostream& ostr, const fource& cr){
  ostr << "\nMessage: " << cr.data << '\n';
  ostr << showpos << fixed << setprecision(12);
  for(int i = 0; i < cr.code.size(); i++){
    cout << "| " << setw(15) << left  << cr.code[i].real() 
         << ' ' << setw(15) << right << cr.code[i].imag() << "i |\n";
  }
  return ostr;
}

istream& operator>>(istream& istr, fource& cr){
  string message;
  istr >> message;
  cr.newMessage(message);
  return istr;
}

fource::~fource(){}
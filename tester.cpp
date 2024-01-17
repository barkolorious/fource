#include "class.cpp"
using namespace std;
#include <chrono>
using namespace std::chrono;

int main(){
  int n, totalGenerateMatrixRunTime = 0;
  cin >> n; n--;
  fource deneme(n);

  auto start = high_resolution_clock::now();
    deneme.generateMatrix();
  auto stop = high_resolution_clock::now();
  totalGenerateMatrixRunTime += duration_cast<milliseconds>(stop - start).count();
  n++;
  string fileName = "matrix"; fileName += to_string(n); fileName += ".dat";
  deneme.saveMatrix(fileName.c_str());

  int testCount = 10000, totalEncryptRunTime = 0, totalSolveRunTime = 0, totalRightAnswers = 0;
  for(int test = 0; test < testCount; test++){
    int len = rand() % n;
    if(len == 0) len++;
    string mes = "";
    for(int i = 0; i < len; i++){
      char letter = (rand() % 95) + 32;
      mes += letter;
    }
   auto start = high_resolution_clock::now();
      deneme.newMessage(mes);
      deneme.encrypt();
  auto  stop = high_resolution_clock::now();
    totalEncryptRunTime += duration_cast<milliseconds>(stop - start).count();

    start = high_resolution_clock::now();
      string ret = deneme.solve();
    stop = high_resolution_clock::now();
    totalSolveRunTime += duration_cast<milliseconds>(stop - start).count();

    if(mes == ret) totalRightAnswers++;
    cout << "Test No " << test + 1 << "/" << testCount << ":\t" << boolalpha << (mes == ret) << endl;
  }
  cout << "N                           :\t" << n << endl;
  cout << "Doğru Cevap Oranı           :\t" << totalRightAnswers << "/" << testCount << endl;
  cout << "Ortalama Şifreleme Süresi   :\t" << totalEncryptRunTime / testCount << "ms" << endl;
  cout << "Ortalama Deşifre Etme Süresi:\t" << totalSolveRunTime / testCount << "ms" << endl;
  cout << "Anahtar Üretme Süresi       :\t" << totalGenerateMatrixRunTime  << "ms" << endl;
}
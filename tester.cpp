#include "fource.hpp"
using namespace std;
#include <chrono>
#include <ctime>
using namespace std::chrono;

int main(){
  int n, totalGenerateMatrixRunTime = 0, testCount;
  auto timenow = system_clock::to_time_t(system_clock::now()); 
  cout << ctime(&timenow) << endl; 
  cout << "N         :\t";
  cin >> n; n--;
  cout << "Test Count:\t";
  cin >> testCount;
  fource deneme(n);

  auto start = high_resolution_clock::now();
    deneme.generateMatrix();
  auto stop = high_resolution_clock::now();
  totalGenerateMatrixRunTime += duration_cast<milliseconds>(stop - start).count();
  n++;
  string fileName = "matrix"; fileName += to_string(n); fileName += ".dat";
  deneme.saveMatrix(fileName.c_str());
  // deneme.loadMatrix(fileName.c_str());

  int totalEncryptRunTime = 0, totalSolveRunTime = 0, totalRightAnswers = 0;
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
  fstream fout("sonuc.txt", ios::out);
  fout << "N                           :\t" << n << endl;
  fout << "Doğru Cevap Oranı           :\t" << totalRightAnswers << "/" << testCount << endl;
  fout << "Ortalama Şifreleme Süresi   :\t" << totalEncryptRunTime / testCount << "ms" << endl;
  fout << "Ortalama Deşifre Etme Süresi:\t" << totalSolveRunTime / testCount << "ms" << endl;
  fout << "Anahtar Üretme Süresi       :\t" << totalGenerateMatrixRunTime  << "ms" << endl;
  
  // string str; cin >> str;
  // fource deneme(str);
  // deneme.encrypt();
  // if(deneme.solve() == str) cout << "lets go\n";
  // cout << "\"" << deneme.solve() << "\"" << endl;
}

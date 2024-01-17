\begin{enumerate}
    \item Şifrelenecek verilerin uzunluğundan büyük ikinin kuvveti olan bir sayı bulunur ve $N$ bu sayıya eşitlenir. \\
    Örnek: Veriler “merhaba”, “dünya!”, “tübitak” ise $N=8$ seçilebilir.
    \item $\mathbb{C}$’den $\mathbb{C}$’ye $N$ tane rastgele fonksiyonlar oluşturulur ($f_0,\ f_1,\ \cdots,\ f_{N-1}$). Bu fonksiyonların belirli bir kuralının olmasına gerek yoktur.
    \item $f_0,f_1,\cdots,f_{N-1}$ fonksiyonlarının $\DFT$’si alınır.
    \[\DFT\{f\}(k)=\sum_{n=0}^{N-1}f(n)\cdot e^{-\frac{i2\pi}{N}kn}\]
    \item $N\times N$’lik anahtar $A$ matrisi oluşturulur. $A$ matrisinin her sütunu fonksiyonların $\DFT$’si ile doldurulur.
    \[
    A=\left[\begin{matrix}
        \DFT\{f_0\}(0)&\DFT\{f_1\}(0)&\cdots&\DFT\{f_{N-1}\}(0)\\
        \DFT\{f_0\}(1)&\DFT\{f_1\}(1)&\cdots&\DFT\{f_{N-1}\}(1)\\
        \vdots&\vdots&\ddots&\vdots\\
        \DFT\{f_0\}(N-1)&\DFT\{f_1\}(N-1)&\cdots&\DFT\{f_{N-1}\}(N-1)\\
    \end{matrix}\right]
    \]
    \item $A$ matrisi bulunduktan sonra Gauss-Jordan Metodu ile $A^{-1}$ hesaplanır.
    \item $A$ ve $A^{-1}$ matrisleri bir kere hesaplandıktan sonra kayıt altına alınır ve sonraki şifrelemelerde kayıtlı matris kullanılır.
\end{enumerate}

\subsection{Verinin Şifrelenmesi} \label{encryptData}
\begin{enumerate}
    \item Aşağıdaki kurala göre bir $s(m)$ fonksiyonu tanımlanır.
    \[s(m)(n)=\left\{\begin{array}{ll}
        \len(m) & : n=0 \\
        m[(n-1)\mod\len(m)] & : n > 0 
    \end{array}\right.\]
    \item $s(m)$ fonksiyonunun $\DFT$’si ile $\vec{B}$ tanımlanır.
    \[\vec B = \left[\begin{matrix}
        \DFT\{s(m)\}(0) \\
        \DFT\{s(m)\}(1) \\
        \vdots \\
        \DFT\{s(m)\}(N-1)
    \end{matrix}\right]\]
    \item $\vec{B}$ tanımlandıktan sonra $A\vec{x}=\vec{B}$ denkleminden $\vec{x}$ bulunur. Bulunan $\vec{x}$ saklanılacak olan şifrelenmiş veridir.
    \begin{align*}
        A\vec x&=\vec B \\
        A^{-1}A\vec x&=A^{-1}\vec B \\
        \vec x&=A^{-1}\vec B
    \end{align*}
\end{enumerate}

\subsection{Verinin Deşifre Edilmesi} \label{decryptData}
\begin{enumerate}
    \item Şifrelenmiş veri kullanılarak $A\vec{x}=\vec{B}$ denkleminden $\vec{B}$ bulunur.
    \item $\vec{B}$ üzerinde Ters Ayrık Fourier Dönüşümü (İng.: Inverse Discrete Fourier Transform, IDFT) uygulanarak $\vec{S}$ elde edilir.
    \[\IDFT\{f\}(n)=\frac1N\sum_{k=0}^{N-1}f(k)\cdot e^{\frac{i2\pi}N kn}\]
    \item $\vec{S}$’in ilk satırı mesajın uzunluğu olduğundan orijinal mesaj iteratif bir şekilde elde edilebilir.
\end{enumerate}

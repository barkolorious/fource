![](logo.png)

# $\text{\textbf{\color{#00daff}FOURCE}: \textbf{\color{#00daff}Four}ier based \textbf{\color{#00daff}C}omplex \textbf{\color{#00daff}E}ncryption}$

A secure algorithm that uses both *Discrete Fourier Transformation* and *linear algebra*. Strong points of the algorithm include:

* **No data loss:** Data before encryption and after decryption is always the same (if there isn't any hardware issue).
* **The chaos in the key:** Since we use DFT to encrypt and decrypt the data any small change in the results in wildly different results in the data. Therefore, chances of brute-force and differential attacks are next to none.
* **The massive size of the key:** Lets assume that we chose $N$ to be equal to $2048$ which is $2^{11}$. Then the will have $N\times N = 2^{22}$ complex number in its entries. Therefore, $2^{23}$ real numbers in the range $(-10^9, 10^9)$. Choosing 1 true random in this range is nearly impossible and you will need $2^{23}$ correct answers to correctly guess the key (may i remind you that every false guess in the key yields a totally different decrypted data.).
* **Flexibility of $s_m$:** Every implementation can use different rules for the $s_m$ function when encrypting the data which provides adding level of complexity.

> ***Dependencies:*** This library uses ```iostream```, ```complex```, ```valarray```, ```random```, ```iomanip``` and ```fstream``` libraries to work. Your compiler needs to have these libraries installed.

To read further description for the algorithm [click here](Algorithm.md).

To read the setup guide [click here](Setup_Guide.md).

---

Made by [Barkın Özsoy](https://github.com/barkolorious), Bilge Şule Yıldız and [Ekin Önal](https://github.com/EkinONAL07) from Izmir Science High School for TUBITAK 2204-A

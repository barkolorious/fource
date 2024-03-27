# Algorithm

> **_Important:_** The math in this is not one-to-one with the code. The code is just an implementation of the algorithm which is more inclusive.

Details for the algorithm is here. Here is [math version](Math.md) if you think you can handle that.

$$
\text{DFT}[f] (k) = \sum_{n = 0}^{N - 1} f(n) \cdot e^{-\frac{i2\pi}N kn}
$$

$$
\text{IDFT}[f] (n) = \frac1N \sum_{k = 0}^{N - 1} f(n) \cdot e^{\frac{i2\pi}N kn}
$$

$$
\mathbb N^{< a} = \{n \in \mathbb N : n < a\} = \{0, \cdots, a - 1\}
$$

> ***Note:*** Every indexing in this page is done zero-indexed. This includes string indices, matrix indices and vector indices.

## Producing a Key

1. Let $N$ be a power of $2$ such that $N$ is bigger than the length of the longest data that’ll be encrypted.
2. Create $N$ functions from $\mathbb N^{< N}$ to $\mathbb C$ ($f_0, f_1, \cdots, f_{N - 1}$). There is no need for a rule for these functions (and we recommend that there isn't any).
3. Define $F_0, F_1, \cdots, F_{N - 1}$ with the Discrete Fourier Transform of $f_0, f_1, \cdots, f_{N - 1}$.

$$
F_i(k) = \mathrm{DFT}[f_i] (k) \ \ \small (i \in \mathbb N^{< N})
$$

4. Create a key matrix $\textbf A$ with the rule:

$$
\textbf A = \left[ \begin{array}{cccc}
	F_0(0)   & F_1(0)   & \cdots & F_{N - 1}(0)     \\
	F_0(1)   & F_1(1)   & \cdots & F_{N - 1}(1)     \\
	\vdots   & \vdots   & \ddots & \vdots           \\
	F_0(N-1) & F_1(N-1) & \cdots & F_{N - 1}(N - 1) \\
\end{array} \right]
$$

$$
\textbf A_{i,j} = F_j(i) = \mathrm{DFT}[f_j] (i) \ \ \small (i,j \in \mathbb N^{< N})
$$

5. Calculate $\textbf A^{-1}$ using Gauss-Jordan Method.

## Encrypting Data

1. Let $m$ be the data to encrypt.
2. Create a the function $s_m$ from $\mathbb N$ to $\mathbb C$ with the rule:

$$
s_m(n) = \left\\{ \begin{array}{ccl} 
	|m|                 & : & n = 0 \\ 
	m_{[(n-1) \\% |m|]} & : & n > 0
\end{array} \right.
$$

> ***Note 1***: $\%$ operator denotes the modulo operation. 

> ***Note 2***: $|m|$ operator denotes the length of $m$.

> ***Note 3***: $m_i$ operator is the zero-indexed $i$<sup>th</sup> element of $m$.

2. Define $\textbf b$ with the $\mathrm{DFT}$ of $s_m$.

$$
\textbf b = \left[ \begin{array}{c}
	\mathrm{DFT}[s_m] (0)   \\
	\mathrm{DFT}[s_m] (1)   \\
	\vdots                  \\
	\mathrm{DFT}[s_m] (N-1) \\
\end{array} \right]
$$

$$
\textbf b_i = \mathrm{DFT}[s_m] (i) \ \ \small (i \in \mathbb N^{< N})
$$

3. Solve the equation $\textbf A \textbf x = \textbf b$ for $\textbf x$. $\textbf x$ is the encrypted data to store.

## Decrypting Data

1. Calculate $\textbf b$ using the equation $\textbf A \textbf x = \textbf b$.
2. Define $\textbf t$ with the $\text{IDFT}$ of $\textbf b$ 

$$
\textbf t_n = \frac1N \sum_{k = 0}^{N - 1} \textbf b_k \cdot e^{\frac{i2\pi}N kn}
$$

3. Let $m$ be the data that is obtained by decryption.
4. Set $m$'s length to be equal to $\textbf t_0$.
5. Obtain $m$ using the equation below.

$$
m_i = \textbf t_{i + 1} \ \ \small (i \in \mathbb N^{< |m|})
$$

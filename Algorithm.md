# Algorithm

> **_Important:_** The math in this documant is not one-to-one with the code. The code is just an implementation of the algorithm.

Details for the algorithm is here. Here is [math version](Math.md) if you think you can handle that.

$$
\text{DFT}[f] (k) = \sum_{n = 0}^{N - 1} f(n) \cdot e^{-\frac{i2\pi}N kn}
$$

$$
\text{IDFT}[f] (n) = \frac1N \sum_{k = 0}^{N - 1} f(n) \cdot e^{\frac{i2\pi}N kn}
$$

$$
\mathcal{N}^{< a} = \\{ n \in \mathcal N : n < a\\} = \\{0, \cdots, a - 1 \\}
$$

> ***Note:*** Every indexing in this page is done zero-indexed. This includes string indices, matrix indices and vector indices.

## Producing a Key

1. Choose $N$ to be a power of $2$.
2. Create $N$ sequences ($f_0, f_1, \cdots, f_{N - 1}$) s.t.  $\forall i, j:  \in \mathbb C$. There is no need for a closed-form formula (and we recommend that there aren't any).
3. Define $F_0, F_1, \cdots, F_{N - 1}$ with the Discrete Fourier Transform of $f_0, f_1, \cdots, f_{N - 1}$.

$$
F_i = \mathrm{DFT}\\{f_i\\} \ \ \small (i \in \mathbb N^{< N})
$$

4. Create a key matrix $\textbf A$ with the rule:

$$
\textbf A = \left[ \begin{array}{cccc}
	F_{0,0}   & F_{1,0}   & \cdots & F_{N-1,0}   \\
	F_{0,1}   & F_{1,1}   & \cdots & F_{N-1,1}   \\
	\vdots    & \vdots    & \ddots & \vdots      \\
	F_{0,N-1} & F_{1,N-1} & \cdots & F_{N-1,N-1} \\
\end{array} \right]
$$

$$
\textbf A_{i,j} = F_{j,i} = (\mathrm{DFT}\\{f_j\\})_i \ \ \small (i,j \in \mathcal N^{< N})
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

> ***Note 1***: $\\%$ operator denotes the modulo operation. 

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

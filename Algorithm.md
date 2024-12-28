# Algorithm

> **_Important:_** The math in this document is not one-to-one with the code. The code is just an implementation of the algorithm.

Details for the algorithm is here. Here is [math version](Math.md) if you think you can handle that.

$$
\text{DFT}[f] (k) = \sum_{n = 0}^{N - 1} f(n) \cdot e^{-\frac{i2\pi}N kn}
$$

$$
\text{IDFT}[f] (n) = \frac1N \sum_{k = 0}^{N - 1} f(n) \cdot e^{\frac{i2\pi}N kn}
$$

> ***Note:*** Every indexing in this page is done zero-indexed. This includes string indices, matrix indices and vector indices.

## Producing a Key

1. Choose $N$ to be a power of $2$.
2. Create $N$ sequences ($f_0, f_1, \cdots, f_{N - 1}$) s.t.  $\forall i, j: f_{i,j} \in \textbf C$. There is no need for a closed-form formula (and we recommend that there aren't any).
3. Define $F_0, F_1, \cdots, F_{N - 1}$ with the Discrete Fourier Transform of $f_0, f_1, \cdots, f_{N - 1}$.

$$
F_i = \mathrm{DFT}\\{f_i\\}
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
\textbf A_{i,j} = F_{j,i} = (\mathrm{DFT}\\{f_j\\})_i
$$

5. Calculate $\textbf A^{-1}$ using Gauss-Jordan Method.

## Encrypting Data

1. Let $m$ be the data to encrypt.
2. Define a sequence $s$ with lenght $N$ s.t. $\forall i: s_i \in \textbf C$ with the rule:

$$
\begin{array}{rcl}
s_0 & = & \mathrm{len}(m) \\
s_i & = & m_{(i - 1) \bmod \mathrm{len}(m)}
\end{array}
$$

> ***Note***: $m_i$ operator is the zero-indexed $i$. element of $m$.

3. Define $S$ to be $\mathrm{DFT}\\{s\\}$.

2. Define $\textbf b$ with the elements of $S$.

$$
\textbf b = \left[ \begin{array}{c}
	S_0       \\
	S_1       \\
	\vdots    \\
	S_{N - 1} \\
\end{array} \right]
$$

$$
\textbf b_i = S_i = \mathrm{DFT}\\{s\\}_i
$$

5. Solve the equation $\textbf A \textbf x = \textbf b$ for $\textbf x$. $\textbf x$ is the encrypted data to store.

## Decrypting Data

1. Calculate $\textbf b$ using the equation $\textbf A \textbf x = \textbf b$.
2. Define a sequence $T$ s.t. $T_i = \textbf b_i$.
3. Define $t$ to be $\mathrm{IDFT}\\{T\\}$.
4. Let $m$ be the data that is obtained by decryption s.t. $\mathrm{len}(m) = t_0$ and $\forall i \in \\{0..\mathrm{len}(m) - 1\\}: m_i = t_{i + 1}$

$$
\textbf m_i = t_{i + 1} = \mathrm{IDFT}\\{T\\}_{i + 1}
$$

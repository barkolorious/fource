# $\text{\textbf{\color{#007FFF}FOURCE}: \textbf{\color{#007FFF}Four}ier based \textbf{\color{#007FFF}C}omplex \textbf{\color{#007FFF}E}ncryption}$

## Producing a Key

1. Let $N$ be a power of $2$ such that $N$ is bigger than the length of the longest word that’ll be encrypted.
2. Create $N$ functions from $\mathbb{C}$ to $\mathbb{C}$ ($f_0, f_1, \cdots, f_{N-1}$). There is no need for a rule for these functions.
3. Define $F_0, F_1, \cdots, F_{N-1}$ with the $\mathrm{DFT}$ of $f_0, f_1, \cdots, f_{N-1}$.

$$
\Large
F_i(k) = \mathrm{DFT} [f_i] (k)=\sum_{n=0}^{N-1}f_i(n)\cdot e^{-\frac{i2\pi}{N}kn}
$$

4. Create a key matrix $\textbf{A}$ such that each column of $\textbf{A}$ is the $F_{\mathrm{colum}}$.

<!--
$$
\large
\textbf{A}=\begin{bmatrix}
\mathrm{DFT}[f_0] (0) & \mathrm{DFT}[f_1] (0) & \cdots & \mathrm{DFT}[f_{N-1}] (0) \\
\mathrm{DFT}[f_0] (1) & \mathrm{DFT}[f_1] (1) & \cdots & \mathrm{DFT}[f_{N-1}] (1) \\
\vdots & \vdots & \ddots & \vdots \\
\mathrm{DFT}[f_0] (N-1) & \mathrm{DFT}[f_1] (N-1) & \cdots & \mathrm{DFT}[f_{N-1}] (N-1) \\
\end{bmatrix}
$$-->

$$
\Large
\textbf{A}=\begin{bmatrix}
F_0(0) & F_1(0) & \cdots & F_{N-1}(0) \\
F_0(1) & F_1(1) & \cdots & F_{N-1}(1) \\
\vdots & \vdots & \ddots & \vdots \\
F_0(N-1) & F_1(N-1) & \cdots & F_{N-1}(N-1) \\
\end{bmatrix}
$$

$$
\Large
\textbf{A}_{r,c}=F_c(r)=\mathrm{DFT}[f_c] (r)
$$

5. Calculate $\textbf{A}^{-1}$ using Gauss-Jordan Method.

## Encrypting Data

1. Let $m$ be the data to encrypt.
2. Create a function $s(m)$ with the rule:

$$
\Large
s_m(n) = \left\\{ \begin{array}{ccl}
\mathrm{len}(m) & : & n = 0 \\
m\left[(n - 1)\ \\%\  \mathrm{len}(m) \right] & : & n > 0
\end{array} \right.
$$

*Note 1*: $\\%$ operator denotes the modulo operation. 

*Note 2*: $\mathrm{len}(m)$ operator denotes the length of $m$.

*Note 3*: $m[i]$ operator is the zero-indexed $i$<sup>th</sup> element of $m$.

2. Define $\textbf{b}$ with the $\mathrm{DFT}$ of $s(m)$.

$$
\Large
\textbf{b} = \left[\begin{matrix}
\mathrm{DFT}[s_m] (0) \\
\mathrm{DFT}[s_m] (1) \\
\vdots \\
\mathrm{DFT}[s_m] (N-1) \\
\end{matrix}\right]
$$

3. Solve the equation $\textbf{A}\textbf{x}=\textbf{b}$ for $\textbf{x}$. $\textbf{x}$ is the encrypted data to store.

<!--
$$
\large
\begin{array}{rcl}
\textbf{A}\textbf{x} & = & \textbf{b} \\
\textbf{A}^{-1} \textbf{A} \textbf{x} & = & \textbf{A}^{-1} \textbf{b} \\
\textbf{x} & = & \textbf{A}^{-1}\textbf{b}
\end{array}
$$
-->

## Decrypting Data

1. Calculate $\textbf{b}$ using the equation $\textbf{A}\textbf{x}=\textbf{b}$.
<!--2. Let $\textbf{v}_r$ be the element in the $r$th row of $\textbf{v}$.-->
2. Define $\textbf{S}$ with the $\text{IDFT}$ of $\textbf{b}$

$$
\Large
\textbf{S}_n = \frac{1}{N} \sum _{k = 0}^{N-1} \textbf{b}_k \cdot e^{\frac{i2\pi}{N}kn}
$$

3. Let $m$ be the data that is obtained by decryption.
4. Set $m$'s length to be equal to $\textbf{S}_0$.
5. Obtain $m$ using the equation below.

$$
\Large
\forall i < \text{len}(m) : m_i = \textbf{S}_i
$$

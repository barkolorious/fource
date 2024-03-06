# Producing a Key

1. Let $N$ be a power of $2$ such that $N$ is bigger than the length of the longest word that’ll be encrypted.
2. Create $N$ functions from $\mathbb{C}$ to $\mathbb{C}$ ($f_0, f_1, \cdots, f_{N-1}$). There is no need for a rule for these functions.
3. Take the $\mathrm{DFT}$ of $f_0, f_1, \cdots, f_{N-1}$.

$$
\mathrm{DFT} [f_i] (k)=\sum_{n=0}^{N-1}f_i(n)\cdot e^{-\frac{i2\pi}{N}kn}
$$

4. Create a key matrix $\textbf{A}$ such that each column of $\textbf{A}$ is the $\mathrm{DFT}$ of $f_{\mathrm{colum}}$.

$$
\textbf{A}=\begin{bmatrix}
\mathrm{DFT}[f_0] (0) & \mathrm{DFT}[f_1] (0) & \cdots & \mathrm{DFT}[f_{N-1}] (0) \\
\mathrm{DFT}[f_0] (1) & \mathrm{DFT}[f_1] (1) & \cdots & \mathrm{DFT}[f_{N-1}] (1) \\
\vdots & \vdots & \ddots & \vdots \\
\mathrm{DFT}[f_0] (N-1) & \mathrm{DFT}[f_1] (N-1) & \cdots & \mathrm{DFT}[f_{N-1}] (N-1) \\
\end{bmatrix}
$$

$$
\textbf{A}_{ij}=\mathrm{DFT}[f_j] (i)
$$

5. Calculate $\textbf{A}^{-1}$ using Gauss-Jordan Method.

# Encrypting Data

1. Create a function $s(m)$ with the rule:

$$
s(m)(n) = \begin{cases}
\mathrm{len}(m) & : & n = 0 \\
m\left[(n - 1) \mod \mathrm{len}(m) \right] & : & n > 0
\end{cases}
$$

2. Define $\textbf{b}$ with the $\mathrm{DFT}$ of $s(m)$.

$$
\textbf{b} = \left[\begin{matrix}
\mathrm{DFT}[s(m)] (0) \\
\mathrm{DFT}[s(m)] (1) \\
\vdots \\
\mathrm{DFT}[s(m)] (N-1) \\
\end{matrix}\right]
$$

3. Solve the equation $\textbf{A}\textbf{x}=\textbf{b}$ for $\textbf{x}$. $\textbf{x}$ is the encrypted data to store.

$$
\begin{array}{rcl}
\textbf{A}\textbf{x} & = & \textbf{b} \\
\textbf{A}^{-1} \textbf{A} \textbf{x} & = & \textbf{A}^{-1} \textbf{b} \\
\textbf{b} & = & \textbf{A}^{-1}\textbf{b}
\end{array}
$$

# Decrypting Data

1. Calculate $\textbf{b}$ using the equation $\textbf{A}\textbf{x}=\textbf{b}$.
2. 

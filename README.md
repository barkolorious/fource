# Producing a Key

1. Let $N$ be a power of $2$ such that $N$ is bigger than the length of the longest word that’ll be encrypted.
2. Create $N$ functions from $\mathbb{C}$ to $\mathbb{C}$ ($f_0, f_1, \cdots, f_{N-1}$). There is no need for a rule for these functions.
3. Take the $\mathrm{DFT}$ of $f_0, f_1, \cdots, f_{N-1}$.

$$
\mathrm{DFT}[f_i](k)=\sum_{n=0}^{N-1}f_i(n)\cdot e^{-\frac{i2\pi}{N}kn}
$$

1. Create a key matrix $A$ such that each column of $A$ is the $\mathrm{DFT}$ of $f_{\mathrm{colum}}$.

$$
A=\begin{bmatrix}
\mathrm{DFT}[f_0](0) & \mathrm{DFT}[f_1](0) & \cdots & \mathrm{DFT}[f_{N-1}](0) \\
\mathrm{DFT}[f_0](1) & \mathrm{DFT}[f_1](1) & \cdots & \mathrm{DFT}[f_{N-1}](1) \\
\vdots & \vdots & \ddots & \vdots \\
\mathrm{DFT}[f_0](N-1) & \mathrm{DFT}[f_1](N-1) & \cdots & \mathrm{DFT}[f_{N-1}](N-1) \\
\end{bmatrix}
$$

$$
A_{ij}=\mathrm{DFT}[f_j](i)
$$

1. Calculate $A^{-1}$ using Gauss-Jordan Method.

# Encrypting Data

1. Create a function $s(m)$ with the rule:

$$
s(m)(n)=\left\{\begin{matrix}
\mathrm{len}(m) & : & n=0 \\
m[(n-1) \mod \mathrm{len}(m) & : & n>0
\end{matrix}\right.
$$

1. Define $\vec B$ with the $\mathrm{DFT}$ of $s(m)$.

$$
\vec B = \left[\begin{matrix}
\mathrm{DFT}[s(m)](0) \\
\mathrm{DFT}[s(m)](1) \\
\vdots \\
\mathrm{DFT}[s(m)](N-1) \\
\end{matrix}\right]
$$

1. Solve the equation $A\vec x=\vec B$ for $\vec x$. $\vec x$ is the encrypted data to store.

$$
\begin{array}{rcl}
A\vec x & = & \vec B \\
A^{-1} A \vec x & = & A^{-1} \vec B \\
\vec x & = & A^{-1} \vec B
\end{array}
$$

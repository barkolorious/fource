# Algorithm (Math Edition)

$$
D \text{ is a database} \implies D \subseteq \\{0,1\\}^*
$$

$$
\text{DFT}[f] (k) = \sum_{n = 0}^{N - 1} f(n) \cdot e^{-\frac{i2\pi}N kn}
$$

$$
\text{IDFT}[f] (n) = \frac1N \sum_{k = 0}^{N - 1} f(n) \cdot e^{\frac{i2\pi}N kn}
$$

$$
\mathbb N^{< a} = \\{n \in \mathbb N : n < a\\} = \\{0, \cdots, a - 1\\}
$$

> ***Note:*** Every indexing in this page is done zero-indexed. This includes string indices, matrix indices and vector indices.

> ***Note:*** Some variables here are named differently from [the main algorithm page](Algorithm.md). 

## Producing a Key

1. $N = 2^k \text{ s.t. } [k \in \mathbb N] \wedge [N > \max \\{|m| : m \in D\\}]$
2. $f_0, f_1, \cdots, f_{N - 1} : \mathbb N^{< N} \mapsto \mathbb C$
3. $F_i = \text{DFT}[f_i]$
4. $\mathbf A_{i,j} = F_j(i) \ \ \small (i,j \in \mathbb N^{< N})$

## Encrypting Data
 
1. $s_m: \mathbb N \mapsto \mathbb C$
2.

$$
  s_m(n) = \left\\{ \begin{array}{ccl}
    |m| & : & n = 0 \\
    m_{[(n-1) \\% |m|]} & : & n > 0 
  \end{array} \right.
$$

4. $\mathbf b_i = \text{DFT}[s_m] (i) \ \ \small (i \in \mathbb N^{< N})$
5. $\widehat m_i = \mathbf x_i \ \ \small (i \in \mathbb N^{< N}) \normalsize \text{ where } \mathbf A \mathbf x =\mathbf b$
6. $\widehat m \text{ is the encrypted } m \text{ to save.}$

## Decrypting Data

1. $\mathbf x_i = \widehat m_i \ \ \small (i \in \mathbb N^{< N})$
2. $g(i) = \mathbf b_i \ \ \small (i \in \mathbb N^{< N}) \normalsize \text{ where } \mathbf A \mathbf x = \mathbf b$
3. $\mathbf S_i = \text{IDFT}[g] (i) \ \ \small (i \in \mathbb N^{< N})$
4. $|t| = \mathbf S_0$
5. $t_i = \mathbf S_{i + 1} \ \ \small (i \in \mathbb N^{< |t|})$
6. $t \text{ is the decrypted data which is equal to } m.$

# SSE-Implementations

Implementations of `PiBas` ([Cash et al. 2014](https://eprint.iacr.org/2014/853.pdf)), `Log-SRC`, and `Log-SRC-i` ([Demertzis et al. 2016](https://idemertzis.com/Papers/sigmod16.pdf)) SSE schemes for experimental evaluation.

# Requirements

- At least C++17 is required, which corresponds to G++ version 11 and above. To check the currently installed G++ version, use `g++ --version` at the command line.
- The OpenSSL library needed to compile the program can be installed with `sudo apt install libssl-dev`.

# Compiling

```
g++ main.cpp range_sse.cpp pi_bas.cpp log_src.cpp log_srci.cpp util/*.cpp -lcrypto
```

Using GCC to compile instead of G++ will probably produce a wall of errors (different linking mechanics?).

# Credits

- <https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption#C.2B.2B_Programs> for a rough guide to encryption and decryption in OpenSSL, specifically with more C++-friendly syntax.
- <https://stackoverflow.com/a/34624592> for providing me a stupidly simple way to make said C++-friendly syntax work after hours of soul-searching and hair-pulling.
- C++ templates for teaching me pain.

# Operations

Specifically, the Salsa20 hash function is a long chain of three simple operations: 32-bit addition (breaking linearity over Z/2), 32-bit xor (breaking linearity over Z/2^32), and constant-distance 32-bit rotation (diffusing changes from high bits to low bits).

The argument for complicated operations is that they provide a remarkable amount of mixing at reasonable speed on many CPUs, and thus achieve any desired security level more quickly than simple operations on those CPUs.

The simple operations are consistently fast.

## Should there be integer multiplications?
The basic counterargument is that integer multiplication takes several cycles on typical CPUs, and many more cycles on some CPUs. For comparison, a comparably complex series of simple integer operations is always reasonably fast. Multiplication might be slightly faster on some CPUs but it is not consistently fast.

A further argument against integer multiplication is that it increases the risk of timing leaks. What really matters is not the speed of integer multiplication, but the speed of constant-time integer multiplication, which is often much slower.

## Should there be S-box lookups?
It is not obvious that a series of S-box lookups - even with rather large S-boxes, as in AES, increasing L1 cache pressure on large CPUs and forcing different implementation techniques for small CPUs - is faster than a comparably complex series of integer operations.

A further argument against S-box lookups is that, on most platforms, they are vulnerable to timing attacks.

## Should there be fewer rotations?
Replacing some of the rotations with a comparable number of additions might achieve comparable diffusion in fewer rounds.

The reader may be wondering why I used rotations rather than shifts. The basic argument for rotations is that one xor of a rotated quantity provides as much diffusion as two xors of shifted quantities. There does not appear to be a counterargument. Yes, the UltraSPARC has to simulate rotations with shifts, but it can still xor a rotated quantity just as quickly as performing two xors of shifted quantities. Meanwhile, on many other CPUs, rotation saves time.

# Encryption
Salsa20 is, at first glance, a traditional stream cipher; at second glance, a hash function in counter mode.

A continuing theme in this discussion is that, conjecturally, we gain security by reducing the amount of communication from the cryptanalyst to the internal cipher state.

## Should encryption and decryption be different?
There are several clear arguments against CBC. One disadvantage of CBC is that it requires different code for encryption and decryption, increasing costs in many contexts. Another disadvantage of CBC is that the extra communication from the cryptanalyst into the cipher state is a security threat; regaining the original level of confidence means adding rounds, taking additional time.

There is a security proof for CBC. How, then, can I claim that CBC is less secure than counter mode? One answer is that CBC’s security guarantee assumes that the block cipher outputs for attacker-controlled inputs are indistinguishable from uniform, whereas counter mode applies the block cipher to highly restricted inputs, with many input bits forced to be 0. There are many examples in the literature of block ciphers for which CBC has been broken but counter mode is unbroken.

## Should the stream be independent of the plaintext?
One counterargument is that "free" is a wild exaggeration. Incorporating the plaintext into the stream takes time for every block, and generating an authenticator takes time for every message

Another counterargument is that the incorporation of plaintext, being extra communication from the cryptanalyst into the cipher state, is a security threat. Regaining the original level of confidence means adding rounds, which takes additional time for every block.

Another counterargument is that state-of-the-art 128-bit Wegman-Carter MACs - in particular, Poly1305 - take only a few cycles per byte. Even if this exceeds the cost of "free" authentication for legitimate packets, it is much less expensive than "free" authentication for forged packets, because it skips the cost of decryption.

For me, the cost of rejecting forged packets is decisive. Consider a denialof-CPU-service attack in which an attacker floods a CPU with forged packets through a large network. In this situation, a traditional Wegman-Carter MAC is capable of handling a substantially larger flood than a "free" authenticator.

## Should there be more state?
Many stream ciphers use a larger state, reusing portions of the first-block computation as input to the second-block computation, reusing portions of the second-block computation as input to the third-block computation, etc.

The argument for a larger state is that one does not need as many cipher rounds to achieve the same conjectured security level. Copying state across blocks seems to provide just as much mixing as the first few cipher rounds. A larger state therefore saves some time after the first block.

The counterargument is that a larger state loses time in some contexts. Reuse forces serialization: one cannot take advantage of extra hardware to reduce the latency of encrypting or decrypting long messages. Furthermore, large states reduce the number of messages that can be processed simultaneously on limited hardware.

## Should blocks be larger than 64 bytes?
The basic argument for a larger block size, say 256 bytes, one does not need as many cipher rounds to achieve the same conjectured security level. Using a larger block size, like copying state across blocks, seems to provide just as much mixing as the first few cipher rounds. A larger state therefore saves time.

The basic counterargument is that a larger block size also loses time. On most CPUs, the communication cost of sweeping through a 256-byte block is a bottleneck; CPUs are designed for computations that don’t involve so much data.

Another way that a larger block size loses time is by increasing the overhead for inconvenient message sizes. Expanding a 300-byte message to 512 bytes is much more expensive than expanding it to 320 bytes.

# Hashing
## Should the hash use a block cipher?
For comparison, a "block cipher" uses the k words to modify the k words, the k words to modify the n words, and the n words to modify the n words; but it never uses the n words to modify the k words. The k words are kept separate from the n words through the entire computation. Only the n words are used as output. The omission of k prevents the cryptanalyst from inverting the computation.

The basic argument for a block cipher - for keeping the k words independent of the n words - is that, for fixed k, it is easy to make a block cipher be an invertible function of n. But this feature seems to be of purely historical interest. Invertibility is certainly not necessary for encryption.

The basic disadvantage of a block cipher is that the k words consume valuable communication resources. A 64-byte block cipher with a 32-byte key would need to repeatedly sweep through 96 bytes of memory (plus a few bytes of temporary storage) for its 64 bytes of output; in contrast, Salsa20 repeatedly sweeps through just 64 bytes of memory (plus a few bytes of temporary storage) for its 64 bytes of output.

I also see each use of a k word as a missed opportunity to spread changes through the n words. The time wasted is not very large - in AES, for example, 80% of the table lookups and most of the xor inputs are n-dependent - and can be reduced by precomputation in contexts where the cost of memory is unnoticeable; but dropping the barrier between k and n achieves the same conjectured security level at higher speed.

## Should there be more code?
The basic argument for using two different kinds of rounds is the idea that attacks will have some extra difficulty passing through the switch from one kind to another. This extra difficulty would allow the cipher to reach the same security level with fewer rounds.

The basic counterargument is that extra code is expensive in many contexts. It increases pressure on a CPU’s L1 cache, for example, and it increases the minimum size of a hardware implementation.

Even if larger code were free, I wouldn’t feel comfortable reducing the number of rounds. The cryptanalytic literature contains a huge number of examples of how extra rounds increase security; it’s much less clear how much benefit is obtained from switching round types.

## Should there be fewer rounds?
I’m comfortable with the 20 rounds of Salsa20 as being far beyond what I’m able to break. Perhaps it will turn out that, after more extensive attempts at cryptanalysis, the community is comfortable with a smaller number of rounds; I can imagine using a smaller number of rounds for the sake of speed. On the other hand, Salsa20 will still have its place as a conservative design that is fast enough for practically all applications.

Presumably 16-byte keys can get away with fewer rounds than 32-byte keys. But this type of variability creates two real-world problems: first, it complicates hardware implementations; second, it seems to tempt users to reduce key sizes even in situations where the cost savings is insignificant.

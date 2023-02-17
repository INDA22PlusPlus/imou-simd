# Using AVX2 to optimize Google Chrome base64 encoding
The idea started from this [article](https://arxiv.org/pdf/1704.00605.pdf), where it is mentioned on how standard Google Chrome base64 uses 3 lookup tables to optimize the encoding process ([see](https://github.com/client9/stringencoders/blob/master/src/modp_b64.c)). I wanted to try and optimize base64 encoding that is dependent on lookup tables with AVX2 instructions. This allows custom tables to be used for the base64 encoding process. The main idea is that only 2 bytes per every dword are unpacked with AVX2 instructions, where multiple dwords are stored in one AVX2 256bit register (8 of them). This AVX2 byte unpacking process is inspired from the [article](https://arxiv.org/pdf/1704.00605.pdf) and is 'illustrated' on page 12 with figure 3.
<br>
<br>
Nanobench was used to get the benchmarking results

## Results
| relative |               ns/op |                op/s |    err% |     total | benchmark
|---------:|--------------------:|--------------------:|--------:|----------:|:----------
|   100.0% |           15,773.88 |           63,395.94 |    0.6% |      1.90 | `google chrome base64 encode`
|   162.5% |            9,706.89 |          103,019.64 |    0.5% |      1.16 | `avx2 base64 encode`

60% faster! Compile with -O1 for same results. 

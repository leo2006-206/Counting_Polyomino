Compiler OPT flag = `-O3, -march=native, -flto`

Run with `taskset -c 8 ./main`

Core 8 is the performance core, after Core 8 are the efficiency cores

Version 1 `Poly_Int_Pair_v1`:

<!-- |Number of cells|Duration 1|Duration 2|Duration 3|Duration 4|Duration 5|Average (± Std Dev)|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|8|289 ms|290 ms|290 ms|291 ms|294 ms|290.8 ms ± 1.9 ms|
|9|4014 ms|4033 ms|4065 ms|4067 ms|4076 ms|4051.0 ms ± 26.3 ms|
|10|65338 ms|65871 ms|67335 ms|68792 ms|76984 ms|68864.0 ms ± 4734.7 ms| -->

|Number of cells|Average (± Std Dev)|
|:---:|:---:|
|8|290.8 ms ± 1.9 ms|
|9|4051.0 ms ± 26.3 ms|
|10|68864.0 ms ± 4734.7 ms|

Version 2 `Poly_Int_Pair_v2`:

<!-- |Number of cells|Duration 1|Duration 2|Duration 3|Duration 4|Duration 5|Average (± Std Dev)|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|8|3 ms|3 ms|3 ms|3 ms|3 ms|3.0 ms ± 0.0 ms|
|9|14 ms|14 ms|15 ms|16 ms|16 ms|15.0 ms ± 1.0 ms|
|10|60 ms|60 ms|61 ms|62 ms|62 ms|61.0 ms ± 1.0 ms|
|11|258 ms|259 ms|259 ms|263 ms|265 ms|260.8 ms ± 3.0 ms|
|12|1129 ms|1129 ms|1132 ms|1132 ms|1132 ms|1130.8 ms ± 1.6 ms|
|13|4894 ms|4900 ms|4973 ms|5081 ms|5176 ms|5004.8 ms ± 121.8 ms| -->

|Number of cells|Average (± Std Dev)|
|:---:|:---:|
|8|3.0 ms ± 0.0 ms|
|9|15.0 ms ± 1.0 ms|
|10|61.0 ms ± 1.0 ms|
|11|260.8 ms ± 3.0 ms|
|12|1130.8 ms ± 1.6 ms|
|13|5004.8 ms ± 121.8 ms|

|Number of cells|Speedup compare to Stage 1|
|:---:|:---:|
|8| $$\frac{290 \text{ ms}}{3 \text{ ms}} \approx 96\text{ Speedup}$$|
|9| $$\frac{4051 \text{ ms}}{15 \text{ ms}} \approx 270\text{ Speedup}$$|
|10| $$\frac{68864 \text{ ms}}{61 \text{ ms}} \approx 1128\text{ Speedup}$$|

Stage 3 `Poly_Int_Pair_v3`:

|Number of cells|Duration 1|Duration 2|Duration 3|Duration 4|Duration 5|
|:---:|:---:|:---:|:---:|:---:|:---:|
|10|48 ms|48 ms|48 ms|48 ms|49 ms|
|11|196 ms|196 ms|197 ms|198 ms|198 ms|
|12|778 ms|781 ms|781 ms|787 ms|787 ms|
|13|3278 ms|3278 ms|3285 ms|3292 ms|3307 ms|

|Number of cells|Speedup compare to Stage 2|
|:---:|:---:|
|10|$$\frac{60 \text{ ms}}{48 \text{ ms}} \approx 1.25\text{ Speedup}$$|
|11|$$\frac{258 \text{ ms}}{196 \text{ ms}} \approx 1.31\text{ Speedup}$$|
|12|$$\frac{1129 \text{ ms}}{778 \text{ ms}} \approx 1.45\text{ Speedup}$$|
|13|$$\frac{4894 \text{ ms}}{3278 \text{ ms}} \approx 1.49\text{ Speedup}$$|

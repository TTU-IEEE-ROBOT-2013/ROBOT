This is the pru code portion of the repository.
This is assymbly for pru. .hp files are for #include "file.hp"
inside of assymbly, .p files are for compileation for pru.
we can only have 2 .p files (one for each pru)
the compiler only takes a single .p file when it assymbles.
this is a 2 pass assymbler, so runs very fast. this is normal.
Init.hp does some TI suggested initializations.

Base.hp has macros and defines for GPIO and ADC communications
Base.hp is the file for any IO helper macros needed, its GPIO and ADC
because i don't think we need any pwm, uart, i2c, ect accessed from pru.
We may not even
SL(1): Cure your bad habit of mistyping
=======================================

SL (Steam Locomotive) runs across your terminal when you type "sl" as you meant to type "ls". It's just a joke command, and not useful at all.

```
% sl --help

Usage:
    sl <options>
Options:
    -a               Accident
    -F               Fly
    -d               Show logo
    -c               Show C51
    -G               Show TGV
    -t <time>
    --time <time>    Set animation time (miliseconds) 
Returns:
    1   Invalid option
    2   Invalid <time>
    3   Missing <time>
    4   Aborted because 1 sec/frame takes forever
```

[Timing](https://github.com/rchu/sl) 2022 Ronald Chu<br>
[TGV Fork](https://github.com/cosmo-ray/sl) 2019 Matthias Gatto<br>
[sl 5.03](https://github.com/mtoyoda/sl) Copyright 1993,1998,2014 Toyoda Masashi

![](demo.gif)
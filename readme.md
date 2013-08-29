![Death Clock](https://github.com/victusfate/deathclock/raw/master/Dethklok.jpg)
Death Clock - "Sewn backtogether wrong"
===

Monitoring thread that commits sepuko after a predetermined time unless it's successfully completes.

# note, need to build libuv first
    git submodule init
    git submodule update
    $ cd libuv
    $ sh autogen.sh
    $ ./configure
    $ make
    $ make check
    $ make install



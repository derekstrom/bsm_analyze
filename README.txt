INTRODUCTION
============

    Analysis of the BSM inputs



DEPENDENCIES
============

    C++ BOOST Libraries
            URL: http://www.boost.org/
        version: 1.44.X+

    ROOT Statistical Analysis Framework
            URL: http://root.cern.ch
        version: 5.22+



INSTALL
=======
    
    [-] Setup Environment

        1. export BOOST_ROOT=/path/to/boost/installation

    [-] Checkout code from github:

        2. git clone git://github.com/ksamdev/bsm_analyze.git
        3. cd bsm_analyze
        4. git submodule update --init
    

    [-] Compile

        5. make all

    [-] Setup Environment [once per shell session]

        6. source setup.sh

    [-] Run Code

        7. wget http://home.fnal.gov/~samvel/ttjets_v4_test.tar.bz
        8. mkdir input && tar -xjf ttjets_v4_test.tar.bz -C test
        9. bsm_cutflow input/*.pb

            NOTE:   multi-threaded executables (the ones that can be found at
                    bsm_analyze/bin) partially controll keyboard to track user
                    input. At the moment only two keys are supported:

                        q   clean application quit
                        i   print status of application: number of files
                            processed and left

                    User may hit key(s) several times at any time during the
                    execution, e.g. 'i', 'i', 'q'



COMPILATION
===========

    The software is split into packages and master project. Packages are
    relatively stable pieces of the code:

        bsm_core
        bsm_stat
        bsm_input

    master project is the rest and uses packages. Analysis is done in the
    master project.

    Therefore two clean/compilation modes are defined:

        1.  make cleanall
            make all

            Clean and compile everything: master project + packages

        2.  make clean
            make

            Clean and compile only master project

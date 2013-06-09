ufc.target = _ufc.so
ufc.commands = @echo "Generation of ufc library" && cmake . && make

#QMAKE_EXTRA_TARGETS += ufc

#PRE_TARGETDEPS += _ufc.so

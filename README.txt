Wir brauchen fuer die momentanen Features von C++11 (v.a. regex zeugs) die neueste Version von g++, da es erst ab Version 4.9 unterstuetzt wird. Bei mir war aber 4.8 installiert. Damit alles funktioniert, installiert die neue Version:

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-4.9

Und im Make file hab ich "g++" zu "g++-4.9" geaendert, damit er die neue Version verwendet!

----
Phil

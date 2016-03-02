Wir brauchen fuer die momentanen Features von C++11 (v.a. regex zeugs) die neueste Version von g++, da es erst ab Version 4.9 unterstuetzt wird. Bei mir war aber 4.8 installiert. Damit alles funktioniert, installiert die neue Version:

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-4.9

Und im Make file hab ich "g++" zu "g++-4.9" geaendert, damit er die neue Version verwendet!

----
Phil

Mongo C++ Client Compilieren:

--> sudo apt-get install build-essential

--> sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev

--> sudo apt-get install scons

--> cd Downloads/

--> wget http://python.org/ftp/python/2.7.2/Python-2.7.2.tgz

--> tar -xvf Python-2.7.2.tgz && cd Python-2.7.2/

--> ./configure --> make --> sudo make altinstall

--> sudo apt-get install libboost-all-dev

--> In Downloads Ordner wechseln

--> https://github.com/mongodb/mongo-cxx-driver/archive/legacy-1.1.0.tar.gz

--> tar -xvf mongo-cxx-driver-legacy-1.1.0.tar.gz && cd mongo-cxx-driver-legacy-1.1.0/

--> sudo scons install --prefix=/usr/local --c++11=on

----
Michi




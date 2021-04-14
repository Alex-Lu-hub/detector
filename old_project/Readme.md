# libdetector

## prepaer

* Install tensorflow C_API

  ```bash
  wget https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-linux-x86_64-2.4.0.tar.gz
  
  sudo tar -C /usr/local -xzf libtensorflow-gpu-linux-x86_64-2.4.0.tar.gz
  
  sudo ldconfig
  ```

* Install yaml-cpp

  ```bash
  git clone https://github.com/jbeder/yaml-cpp.git
  cd yaml-cpp/
  mkdir build
  cd build
  cmake .. -DYAML_BUILD_SHARED_LIBS=ON
  make
  sudo make install
  ```

## build & test

```bash
mkdir build
cmake ..
```

You will find an executable file ``main`` in this path. And you can run it to test.
# modelloading

## TODO
模型的加载路径以及模型的模型矩阵在widget.cpp文件中的`initializeGL()`函数里写死了，如需更改加载的模型或者更改模型的初始缩放、初始位置等请在此处更改

## Pack
```
sudo apt-get install -y devscripts debhelper qt5-qmake qtbase5-dev
debuild -us -uc
```

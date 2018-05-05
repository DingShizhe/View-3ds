# View-3ds
view .3ds files based on z-buffering but without OpenGL

## Function
Read vertex and faces information of models from a .3ds file, render them based on z-buffering-alg. then generate grey-scale map with format .ppm or .png. The grey-scale contains depth information.

```
$ g++ main.cpp
$ ./a.out ./3ds_files/foo.3ds
```

## Dependency
 - [OpenGL Mathematics Library](https://glm.g-truc.net/0.9.8/index.html)
 - [Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/)


## Render Example

<img src="./test0.png" width="300"/> <img src="./test3.png" width="300"/> 

<img src="./test4.png" width="300"/> <img src="./test6.png" width="300"/> 

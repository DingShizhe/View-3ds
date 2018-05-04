# View-3ds
view .3ds files based on z-buffering but without OpenGL

## Function
Read vertex and faces information of models from a .3ds file, render them based on z-buffering-alg. then generate grey-scale map with format .ppm or .png. The grey-scale contains depth information.

```
$ g++ main.cpp
$ ./a.out
$ cd outputs && bash conv.sh
```

## Dependency
 - [OpenGL Mathematics Library](https://glm.g-truc.net/0.9.8/index.html)
 - [Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/)


## Render Example


<img src="./test4.png" width="425"/> <img src="./test3.png" width="425"/> 

<img src="./test5.png" width="425"/> <img src="./out.png" width="425"/> 

<!-- ![Monkey](./test4.png)  ![Monkey](./test4.png) -->


<!-- <div style="float:left;border:solid 2px 000;margin:2px;"><img src="./test5.png"  width="250" alt="Dragon"></div> -->

<!-- <div style="float:left;border:solid 2px 000;margin:2px;"><img src="./out.png" width="250" alt="Complex"></div> -->


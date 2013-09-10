SimpleMouseTrackerExample
=========================

This is a demo on how to use emscripten to create javascript code from native openCV code. 
A frame from a mouse behaviour video experiment is loaded with openCV and the contour of the mouse is extracted.
The output of the program is a jpg file in SimpleMouseTrackerExample/contour.jpg 

##Requirements:
  * [Emscripten](https://github.com/kripken/emscripten/wiki/Tutorial)

###Install:

```bash 
  make install
```

###Compile js code: 

```bash 
  make cp EMCC=path/to/emscripten/emcc
```

###Run in node: 

```bash
   make run-node
```

##Recommended reading:
 * [Mustache manual](http://mustache.github.io/mustache.5.html)
 * [Emscripten/wiki/Filesystem Guide](https://github.com/kripken/emscripten/wiki/Filesystem-Guide)
  
#####See: [template.mustache](https://github.com/sergio2540/FireSim-Emscripten/blob/master/crowdprocess/pre/template/template.mustache)

 * [HOWTO: Port a C/C++ Library to JavaScript (xml.js)@blog.mozakai](http://mozakai.blogspot.pt/2012/03/howto-port-cc-library-to-javascript.html)

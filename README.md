SimpleMouseTrackerExample
=========================

##Requirements:
  * [Emscripten](https://github.com/kripken/emscripten/wiki/Tutorial)
  * [crp-reagenzglas](https://github.com/CrowdProcess/crp-reagenzglas)
  
##Recommended reading:
 * [Mustache manual](http://mustache.github.io/mustache.5.html)
 * [Emscripten/wiki/Filesystem Guide](https://github.com/kripken/emscripten/wiki/Filesystem-Guide)
  
#####See: [template.mustache](https://github.com/sergio2540/FireSim-Emscripten/blob/master/crowdprocess/pre/template/template.mustache)

 * [HOWTO: Port a C/C++ Library to JavaScript (xml.js)@blog.mozakai](http://mozakai.blogspot.pt/2012/03/howto-port-cc-library-to-javascript.html)
 * [Simple Mouse Tracker](https://github.com/joseaccruz/SimpleMouseTracker)
 
 ###Compile js code: 

```bash 
  make cp EMCC=path/to/emscripten/emcc
```

###Run js with [crp-reagenzglas](https://github.com/CrowdProcess/crp-reagenzglas): 

```bash
   make run-editor
```

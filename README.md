M3D
=========

### C OpenGL ES 1.1 framework ###

This is an old very light and fast OpenGL ES framework on which i had worked in late 2013, no internet for two month and i wanted to produce a [demo](http://en.wikipedia.org/wiki/Demoscene) for the [GP2X Wiz](https://en.wikipedia.org/wiki/GP2X_Wiz#Overview), this was rather fun to do because there was no documentations or books at hand when this was made, so this was coded 'blindly' with just memory and headers file as documentation, the GP2X Wiz OpenGL ES driver had many bugs and i got quickly bored, so after ~2 weeks i stopped working on it, some effects could not be made on the device because of the poor quality driver, especially effects which were using blending...

The framework just make the boring work easy but was not meant to replace the usage of OpenGL.

This may still be usefull for someone, the framework feature a custom Wavefront .obj/.mtl loader, simple bitmap font renderer, basic .tga loader (reverse engineered the file format because no docs at hand), quite cool resources manager powered by a custom generic storage hash table and other smaller stuff

[GLFW](http://www.glfw.org/) was used to provide cross-platform window/inputs stuff and [LodePNG](http://lodev.org/lodepng/) was added later, the code contain some GP2X Wiz specific stuff, all libraries are included...

### License ###

Do what you want.

### Screenshot ###

![Alt text](http://www.garzul.tonsite.biz/tmp_/demo.png "m3d")

![Alt text](http://garzul.tonsite.biz/m3d/hash_table_occupancy.png "hash table occupancy pattern")

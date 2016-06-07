// cs473, recurses
camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.5, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-5, 3, 0> color rgb <0.3, 0.5, 0.3>}
light_source {<5, 10, 5> color rgb <0.3, 0.4, 0.5>}

// floor
plane {<0, 1, 0>, -4
  pigment {color rgb <0.2, 0.2, 0.8>}
  finish {ambient 0.4 diffuse 0.9}
  translate <0, -1, 0>
}

// left wall
plane {<1, 0, 0>, -8
  pigment {color rgb <0.8, 0.2, 0.2>}
  finish {ambient 0.4 diffuse 0.9}
  rotate <0, 30, 0>
}

// back wall
plane {<0, 0, -1>, 30
  pigment {color rgb <0.8, 0.4, 0.2>}
  finish {ambient 0.4 diffuse 0.9}
  rotate <0, 30, 0>
}

// back wall on the right
plane {<0, 0, -1>, 30
  pigment {color rgb <0.0, 0.2, 0.2>}
  finish {ambient 0.4 diffuse 0.9}
  rotate <0, -20, 0>
}

// pyramid of spheres
sphere { <-2, -1.74305, -1.74305>, 2
  pigment { color rgbf <1.0, 0.0, 1.0, 0.4>}
  finish {ambient 0.2 diffuse 0.4 reflection 0.6}
  translate <-8, -1, -15>
}

sphere { <2, -1.74305, -1.74305>, 2
  pigment { color rgbf <1.0, 0.0, 1.0, 0.4>}
  finish {ambient 0.2 diffuse 0.4 reflection 0.6}
  translate <-8, -1, -15>
}

sphere { <0, -1.74305, 1.74305>, 2
  pigment { color rgbf <1.0, 0.0, 1.0, 0.4>}
  finish {ambient 0.2 diffuse 0.4 reflection 0.6}
  translate <-8, -1, -15>
}

sphere { <0, 1.74305, 0>, 2
  pigment { color rgbf <1.0, 0.0, 1.0, 0.4>}
  finish {ambient 0.2 diffuse 0.4 reflection 0.6}
  translate <-8, -1, -15>
}


// and a few assorted objects
sphere { <0, 0, 0>, 2
  pigment {color rgbf <1.0, 1.0, 0.0, 0.8>}
  finish {ambient 0.2 diffuse 0.8 reflection .2}
  translate <3.5, 1, 0>
}

sphere { <0, 0, 0>, 2
  pigment {color rgbf <1.0, 1.0, 0.0, 0.8>}
  finish {ambient 0.2 diffuse 0.8 reflection .2}
  translate <-3, -3, 0>
}


box { <-5, -2, -5>, <5, 2, 5>
  pigment { color rgbf <1.0, 0.2, 1.0, 0.6>}
  finish {ambient 0.2 diffuse 0.8 reflection 0.2}
  translate <3, -3, -9>
}

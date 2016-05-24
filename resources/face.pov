// Face by andrew acosta

camera { location  <-8, -.5, 14>
up        <0,  1,  0> 
right     <1.33333, 0,  0> 
look_at   <0, 0, 0>
}

light_source {<-10, 10, 8> color rgb <1.5, 1.5, 1.5>}

// mirror 1
plane {<-1, 0, 0>, -5      
  pigment {color rgb <0.2, 0.6, 0.6>}
  finish {ambient 0.3 diffuse 0.4 specular 0.7 roughness 0.001 reflection 0.8}
}

// mirror 2
plane {<0, 0, -1>, 8      
  pigment {color rgb <0.2, 0.6, 0.6>}
  finish {ambient 0.3 diffuse 0.4 specular 0.7 roughness 0.001 reflection 0.8}
}

// mirror 3
plane {<0, 0, 1>, 20      
  pigment {color rgb <0.2, 0.6, 0.6>}
  finish {ambient 0.3 diffuse 0.4 specular 0.7 roughness 0.001 reflection 0.8}
}

// floor
plane {<0, 1, 0>, -4      
  pigment {color rgb <0.6, 0.2, 1.0>}
  finish {ambient 0.3 diffuse 0.8 specular 0.8 roughness 0.001 reflection 0.5}
}

// ears
box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <1.0, 0.1, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.5 roughness 0.05}
  scale <1, 2, 1>
  rotate <0, 0, 25>
  translate <-1.2, 1 ,0>
}

box { <-1, -1, -1>, <1, 1, 1>
  pigment { color rgb <1.0, 0.1, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.5 roughness 0.05}
  scale <1, 2, 1>
  rotate <0, 0, -25>
  translate <1.2, 1 ,0>
}


// eyes
sphere { <0, 0, 0>, .5
  pigment { color rgb <0.0, 0.0, 0.0>}
  finish {ambient 0.2 diffuse 0.2 specular 0.99 roughness 0.001 }
  scale <.75, .9, .5>
  translate <-.6, -.5, 1.8>
}

sphere { <0, 0, 0>, .5
  pigment { color rgb <0.2, 0.2, 0.2>}
  finish {ambient 0.2 diffuse 0.2 specular 0.99 roughness 0.001 }
  scale <.75, .9, .5>
  translate <.6, -.5, 1.8>
}

// head
sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 0.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.01 }
  translate <0, -1, 0>
}

// nose
sphere { <0, 0, 0>, .5
  pigment { color rgb <0.7, 0.2, 0.7>}
  finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.01 }
  scale <2,1.3,1>
  translate <0, -1.4, 2>
}

// nostrils
sphere { <0, -1, 0>, .5
  pigment { color rgb <0.1, 0.1, 0.1>}
  finish {ambient 0.01 diffuse 0.45 specular 0.44 roughness 0.01 }
  scale <.55,.45,.45>
  translate <-.3, -1, 2.3>
}

sphere { <0, -1, 0>, .5
  pigment { color rgb <0.1, 0.1, 0.1>}
  finish {ambient 0.01 diffuse 0.45 specular 0.44 roughness 0.01 }
  scale <.55,.45,.45>
  translate <.3, -1, 2.3>
}

// teeth
sphere { <0, -1, 0>, .5
  pigment { color rgb <1, 1, 1>}
  finish {ambient 0.1 diffuse 0.65 specular 0.84 roughness 0.001 }
  scale <.45,.55,.15>
  translate <-.3, -2, 1.4>
}

sphere { <0, -1, 0>, .5
  pigment { color rgb <1, 1, 1>}
  finish {ambient 0.1 diffuse 0.65 specular 0.84 roughness 0.001 }
  scale <.45,.55,.15>
  translate <.3, -2, 1.4>
}

sphere { <0, -1, 0>, .5
  pigment { color rgb <1, 1, 1>}
  finish {ambient 0.1 diffuse 0.65 specular 0.84 roughness 0.001 }
  scale <.45,.55,.15>
  translate <0, -2, 1.4>
}

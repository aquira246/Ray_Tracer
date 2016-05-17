// cs174, assignment 1 sample file (RIGHT HANDED)

camera { location  <0, 0, 12>
up        <0,  1,  0> 
right     <1.33333, 0,  0> 
look_at   <0, 0, 0>
}

light_source {<-100, 100, 80> color rgb <1.5, 1.5, 1.5>}



plane {<0, 1, 0>, -4      
  pigment {color rgb <0.6, 0.2, 1.0>}
  finish {ambient 0.3 diffuse 0.8 specular 0.8 roughness 0.001 reflection 0.5}
}

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


sphere { <0, 0, 0>, .5
  pigment { color rgb <0.0, 0.0, 0.0>}
  finish {ambient 0.2 diffuse 0.2 specular 0.99 roughness 0.001 }
  translate <-.6, -.5, 3.2>
  scale <1, 1.2, .5>
}

sphere { <0, 0, 0>, .5
  pigment { color rgb <0.2, 0.2, 0.2>}
  finish {ambient 0.2 diffuse 0.2 specular 0.99 roughness 0.001 }
  translate <.6, -.5, 3.2>
  scale <1, 1.2, .5>
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <1.0, 0.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.01 }
  translate <0, -1, 0>
}

sphere { <0, 0, 0>, .5
  pigment { color rgb <0.7, 0.2, 0.7>}
  finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.01 }
  translate <0, -1, 2>
  scale <2,1.3,1>
}


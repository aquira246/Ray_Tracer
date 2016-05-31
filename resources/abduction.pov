// 

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-10, 10, 8> color rgb <1.5, 1.5, 1.5>}
light_source {<0, -1.5, 0> color rgb <1.5, 1.5, 0>}
light_source {<0, -4.5, 0> color rgb <1.5, 1.5, 0>}
light_source {<0, 0, -10> color rgb <.5, .5, 0>}

// main space ship
sphere { <0, 0, 0>, 2
  pigment { color rgbf <0.6, 0.6, 0.6>}
  finish {ambient 0.2 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
  scale <.6, .6, .6>
  translate <0, 3 ,0>   
}

sphere { <0, 0, 0>, 2
  pigment { color rgbf <0.6, 0.6, 0.6>}
  finish {ambient 0.2 diffuse 0.4 specular 0.4 roughness 0.0001 reflection 0.35}
  scale <1.1, .3, .6>
  translate <0, 3 ,0>   
}

// space ship beam
sphere { <0, 0, 0>, 2
  pigment { color rgb <0.7, 0.7, 0.1>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01 refraction 1}
  scale <1.7, 3, 1>
  rotate <0, 0, 0>
  translate <0, -4.05, 0>
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.0>}
  finish {ambient 0.9 diffuse 0.1 specular 0.2 roughness 0.01}
  scale <1.5, 1.7, .1>
  rotate <0, 0, 90>
  translate <0, -3 , -2>   
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.0>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <1.7, .1, 1>
  translate <0, -6 ,0>   
}

// cow body
sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.8>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <.65, .53, .53>
  translate <0, -3 ,0>   
}

// cow legs
sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.8>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <.2, .35, .2>
  translate <.75, -4 , .6>   
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.8>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <.2, .35, .2>
  translate <-.75, -4 , .6>   
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.8>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <.2, .35, .2>
  translate <.75, -4, -.6>   
}

sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.8>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <.2, .35, .2>
  translate <-.75, -4, -.6>   
}

// cow tail
sphere { <0, 0, 0>, 2
  pigment { color rgb <0.8, 0.8, 0.8>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <.05, .3, .05>
  rotate <0, 0, 10>
  translate <1.4, -3.2 , 0>   
}

// cow head
box { <0, 0, 0>, <1, 1, 1>
  pigment { color rgb <0.7, 0.7, 0.7>}
  finish {ambient 0.3 diffuse 0.9 specular 0.2 roughness 0.01}
  scale <.8, 1, .8>
  rotate <0, 0, 88>
  translate <-1, -2.5 ,0>
}

// cow eye
sphere { <0, 0, 0>, 1
  pigment { color rgb <0.1, 0.1, 0.1>}
  finish {ambient 0.3 diffuse 0.8 specular 0.8 roughness 0.0001}
  scale <.05, .05, .05>
  rotate <0, 0, 90>
  translate <-1.6, -1.8 , .8>  
}

// sky
plane {<0, 0, -1>, 25      
  pigment {color rgb <0.1, 0.05, 0.2>}
  finish {ambient 0.3 diffuse 0.4 specular 0.2 roughness 0.001 reflection 0.1}
}

// stars
sphere {<18, 24, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<29, 9, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<21, 17, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-16, 33, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-13, 20, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-25, 12.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<19.35, 5.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<14, 13.54, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<25, 11.2, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-20, 5.4, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-15, 21, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-25, 0, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-23, 24, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-26, 9, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-26, 17, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<21, 33, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<18, 20, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<30, 12.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-24.35, 5.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-19, 13.54, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-30, 11.2, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<25, 5.4, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<20, 21, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<30, 0, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<36, 24, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<58, 9, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<42, 17, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-32, 33, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-38, 20, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-50, 12.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<57.23, 5.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<42, 13.54, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<41, 11.2, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-32, 5.4, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-33.56, 21, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-38, 0, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-44, 24, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-41.5, 9, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-47, 17, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<21, 33, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<18, 20, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<37, 12.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-25.35, 5.3, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-29, 13.54, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<-33, 11.2, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<52, 5.4, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<43, 21, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}

sphere {<48, 0, 10>, 1
  pigment { color rgb <1, 1, 0>}
  finish {ambient 1 diffuse 0.4 specular 0.8 roughness 0.0001 reflection 0.35}
}



// grass
plane {<0, 1, 0>, -6      
  pigment {color rgb <0.05, 0.2, 0.05>}
  finish {ambient 0.3 diffuse 0.4 specular 0.0 roughness 0.001}
}
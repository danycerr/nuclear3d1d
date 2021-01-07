Point(1) = {0, 0.0, 0., 0. };
Point(2) = {0.022717, 0.0, 0., 0. };
//+
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/3} {
  Duplicata { Point{2}; }
}
//+
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/3} {
  Duplicata { Point{3}; }
}
//+
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/3} {
  Duplicata { Point{4}; }
}
//+
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/3} {
  Duplicata { Point{5}; }
}
//+
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/3} {
  Duplicata { Point{6}; }
}
Line(1) = {3, 6};
Line(2) = {6, 7};
Line(3) = {7, 2};
Line(4) = {2, 3};
Line(5) = {3, 4};
Line(6) = {4, 5};
Line(7) = {5, 6};
Line Loop(8) = {2, 3, 4, 1};
Plane Surface(9) = {8};
Line Loop(10) = {7, -1, 5, 6};
Plane Surface(11) = {10};
Transfinite Line {2, 3, 4, 1, 7, 6, 5} = 30 Using Bump 1;
Transfinite Surface {9} = {6, 3, 2, 7};
Recombine Surface {9};
Extrude {0,0,2} {Surface{9}; Layers{120}; Recombine;}
Transfinite Surface {11} = {5, 6, 3, 4};
Recombine Surface {11};
Extrude {0,0,2} {Surface{11}; Layers{120}; Recombine;}
Physical Volume(57) = {1, 2};

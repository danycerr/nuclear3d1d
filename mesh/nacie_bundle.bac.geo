

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

// Point(8) = {0, 0.0, 2., 0. };
//+
Line(1) = {2, 3};
Line(2) = {3, 4};
Line(3) = {4, 5};
Line(4) = {5, 6};
Line(5) = {6, 7};
Line(6) = {7, 2};
//+
Line Loop(1) = {2, 3, 4, 5, 6, 1};
//+
Plane Surface(1) = {1};
//+
//+
// Extrude {0, 0, 1} {
//  Point{0}; Point{8}; 
// }
//+
// Extrude {0, 0, 2} {
//   Surface{1}; 
// }
//+
Extrude {0, 0, 2} {
  Surface{1}; 
}

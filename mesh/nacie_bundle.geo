

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
Field[1] = Max;
Field[1].FieldsList = {0, 1};
Background Field = 1;
Field[2] = Param;
Delete Field [1];
Field[2].FX = "0.1";
Field[2].FY = "0.1";
Field[2].FZ = "0.1";
Background Field = 2;
Field[2].FX = "0.01";
Field[2].FY = "0.01";
Field[2].FZ = "0.01";
Field[2].FX = "0.001";
Field[2].FY = "0.001";
Field[2].FZ = "0.001";
Field[1] = MathEval;
Field[1].F = "0.1";
Background Field = 1;
Delete Field [2];
Field[1].F = "0.001";
Field[1].F = "0.1";
Field[1].F = "0.01";
Field[1].F = "0.05";
Field[1].F = "0.04";
Field[1].F = "0.03";
Field[1].F = "0.02";
Field[1].F = "0.01";
Field[1].F = "0.02";
Field[1].F = "0.021";
Field[1].F = "0.015";
Field[1].F = "0.005";
Field[1].F = "0.007";
Field[1].F = "0.008";
Physical Volume(39) = {1};
Physical Volume(40) = {1};
Field[1].F = "0.004";

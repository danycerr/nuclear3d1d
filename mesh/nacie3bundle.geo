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

Rotate {{0, 0, 1}, {0.0340755,0.0196735,0}, Pi/3} {
  Duplicata { Point{2}; }
}
Rotate {{0, 0, 1}, {0.0340755,0.0196735,0}, Pi/3} {
  Duplicata { Point{8}; }
}

Rotate {{0, 0, 1}, {0.0340755,0.0196735,0}, Pi/3} {
  Duplicata { Point{9}; }
}

Rotate {{0, 0, 1}, {0.0340755,0.0196735,0}, Pi/3} {
  Duplicata { Point{10}; }
}

Rotate {{0, 0, 1}, {0.0340755,0.0196735,0}, Pi/3} {
  Duplicata { Point{11}; }
}

Rotate {{0, 0, 1}, {0.0340755,-0.0196735,0}, Pi/3} {
  Duplicata { Point{7}; }
}

Rotate {{0, 0, 1}, {0.0340755,-0.0196735,0}, Pi/3} {
  Duplicata { Point{12}; }
}

Rotate {{0, 0, 1}, {0.0340755,-0.0196735,0}, Pi/3} {
  Duplicata { Point{13}; }
}Line(1) = {6, 5};
Line(2) = {5, 4};
Line(3) = {5, 2};
Line(4) = {7, 6};
Line(5) = {4, 3};
Line(6) = {3, 2};
Line(7) = {2, 7};
Line(8) = {7, 12};
Line(9) = {12, 13};
Line(10) = {13, 14};
Line(11) = {14, 8};
Line(12) = {8, 2};
Line(13) = {3, 11};
Line(14) = {11, 10};
Line(15) = {10, 9};
Line(16) = {9, 8};
Line(17) = {3, 9};
Line(18) = {7, 14};
Line Loop(19) = {5, 6, -3, 2};
Plane Surface(20) = {19};
Line Loop(21) = {3, 7, 4, 1};
Plane Surface(22) = {21};
Line Loop(23) = {14, 15, -17, 13};
Plane Surface(24) = {23};
Line Loop(25) = {16, 12, -6, 17};
Plane Surface(26) = {25};
Line Loop(27) = {11, 12, 7, 18};
Plane Surface(28) = {27};
Line Loop(29) = {18, -10, -9, -8};
Plane Surface(30) = {29};
Transfinite Line {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18} = 10 Using Bump 1;
Transfinite Surface {20} = {2, 3, 4, 5};
Transfinite Surface {22} = {2, 5, 6, 7};
Transfinite Surface {24} = {9, 10, 11, 3};
Transfinite Surface {26} = {2, 8, 9, 3};
Transfinite Surface {28} = {14, 8, 2, 7};
Transfinite Surface {30} = {13, 14, 7, 12};
Recombine Surface {20,22,24,26,28,30};
ndiv=30;
Extrude {0,0,2} {Surface{20}; Layers{ndiv}; Recombine;}
Extrude {0,0,2} {Surface{22}; Layers{ndiv}; Recombine;}
Extrude {0,0,2} {Surface{24}; Layers{ndiv}; Recombine;}
Extrude {0,0,2} {Surface{26}; Layers{ndiv}; Recombine;}
Extrude {0,0,2} {Surface{28}; Layers{ndiv}; Recombine;}
Extrude {0,0,2} {Surface{30}; Layers{ndiv}; Recombine;}

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
c[] = Point{4}; // c[] contains the coordinates 1,2,3
d[] = Point{5}; // c[] contains the coordinates 1,2,3
Point(8) ={0.5*c[0]+0.5*d[0], 0.5*c[1]+0.5*d[1], 0.0};
c[] = Point{2}; // c[] contains the coordinates 1,2,3
d[] = Point{7}; // c[] contains the coordinates 1,2,3
Point(9) ={0.5*c[0]+0.5*d[0], 0.5*c[1]+0.5*d[1], 0.0};



Dilate {{0, 0, 0.}, 0.5} {
  Duplicata { Point{3}; }
}
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/2} {
  Duplicata { Point{10}; }
}
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/2} {
  Duplicata { Point{11}; }
}
Rotate {{0, 0, 1}, {0, 0, 0}, Pi/2} {
  Duplicata { Point{12}; }
}

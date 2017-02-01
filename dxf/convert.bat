@del %1.sg3
rb_saturndxf dxf\%1.dxf -o %1_C.dxf -b
dxf2sg3 %1_C.dxf -scl 50 50 50 -back
@rename %1_C.sg3 %1.sg3
@del %1_C.dxf
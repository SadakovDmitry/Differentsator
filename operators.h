//OP(name, str_op, num_op, def, len, priority, math)
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//OP(name, str_op, num_op, def                                                              , len                , prior,  math            , mask      )
OP(ADD , "+"  , 0 , ADD(D(L), D(R))                                                         , 3 * (L + R) + 1    , 0   ,   left+right      ,  "%s + %s")
OP(SUB , "-"  , 1 , SUB(D(L), D(R))                                                         , 3 * (L + R) + 1    , 0   ,   left-right      ,  "%s - %s")
OP(MUL , "*"  , 2 , ADD(MUL(C(L), Der(R)) , MUL(D(L), C(L)))                                , 3 * (L + R) + 1    , 1   ,   left*right      ,  "%s * %s")
OP(DIV , "/"  , 3 , DIV(SUB(MUL(C(L), D(R)) , MUL(Der(L), C(L))), POW(C(R), CONST(2)))      , 2 * MAX(L , R) + 1 , 1   ,   left/right      ,  "\\frac{%s}{%s}")
OP(SQRT, "√"  , 4 , MUL(D(R), DIV(CONST(1), MUL(CONST(2), C(node))))                        , 8 * L + 2 * R + 1  , 20  ,   sqrt(right)     ,  "sqrt{%s}")
OP(SIN , "sin", 5 , MUL(D(R), COS(C(R)))                                                    , 2*R + 3            , 0   ,   sin(right)      ,  "sin(%s)")
OP(COS , "cos", 6 , MUL(D(R), MUL(CONST(-1), SIN(C(R))))                                    , 2*R + 3            , 0   ,   cos(right)      ,  "cos(%s)")
OP(TAN , "tg" , 7 , MUL(D(R), DIV(CONST(1), POW(COS(C(R)), CONST(2))))                      , 2*R + 3            , 0   ,   tan(right)      ,  "tg(%s)")
OP(COT , "ctg", 8 , MUL(D(R), MUL(CONST(-1), SIN(DIV(CONST(1), POW(SIN(C(R)), CONST(2)))))) , 2*R + 3            , 0   ,   1/tan(right)    ,  "ctg(%s)")
OP(POW , "^"  , 9 , MUL(C(node), ADD(MUL(D(R), LOG(E, C(L))), MUL(DIV(Der(L), C(L)), C(R)))), 8 * L + 2 * R + 1  , -100,   pow(left, right),  "%s^{%s}")
OP(LOG , "log", 10, MUL(D(R), DIV(CONST(1), C(R)))                                          , 2*R + 3            , 1   ,   log(right)      ,  "lof_{%s}{%s}")
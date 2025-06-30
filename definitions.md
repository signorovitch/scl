Lambda
    size_t parmc
    AST** parmv
    AST* body

Call
    Lambda to
        size_t parmc
        AST** parmv
    size_t argc
    AST** argv

VDef
    char* name
    AST* exp

(\(x) x * 2)(4)

Call
to:
    Lambda
    parc: 1
    parv: [ x ]
    body: 
        Call
        to:
            BIF(mul)
         argc: 2
         argv: [
             VRef
             name: x

             2
         ]
argc: 1
argv: [ 4 ]

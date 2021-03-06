/* Mode: -*- C++ -*- */
/* @BC
 *		                Copyright (c) 1993
 *	    by Microelectronics and Computer Technology Corporation (MCC)
 *				All Rights Reserved
 *
 *	Permission to use, copy, modify, and distribute this software and its
 *	documentation for any purpose and without fee is hereby granted,
 *	provided that this notice be retained unaltered, and that the name of
 *	MCC and its shareholders and participants shall not be used in
 *	advertising or publicity pertaining to distribution of the software
 *	without specific written prior permission.
 *
 *	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 *	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * $Header$
 *
 * $Log$
 */

#if !defined(_RBL_Prim_h)
#define _RBL_Prim_h

#ifdef __GNUG__
#pragma interface
#endif

#include "BinaryOb.h"
#include "Ctxt.h"


/*
 * The encoding for opApplyPrim assumes that the prim offset fits in one
 * byte.  If MaxPrims is made larger than 256, changes will be required
 * Code.h, Code.cc, and Vm.cc to change the encoding of opApplyPrim.
 */


static const int MaxPrims = 1024;



class Prim : public BinaryOb
{
    STD_DECLS(Prim);

  protected:

    static int		primcount;
    static Prim*	inlineTbl [MaxPrims];

    Prim (char*, PRIMFN*, int, int);

    virtual int		traversePtrs (PSOb__PSOb);
    virtual int		traversePtrs (SI__PSOb);
    virtual void	traversePtrs (V__PSOb);

    friend class	BuiltinPrim;

  public:

    Ob*			id;
    PRIMFN*		fn;
    ArgNum		minargs;
    ArgNum		maxargs;
    unsigned short	primnum;

    static Prim*	create (char*, PRIMFN*, int, int);
    static Prim*	nthPrim (int);

    int			primNumber ();
    virtual Prim*	InlineablePrimP ();
    Ob*			dispatchHelper (Ctxt*);
    virtual Ob*		dispatch (Ctxt*);
    virtual Ob*		invoke (Ctxt*);
};

inline Prim* Prim::nthPrim (int n) { return Prim::inlineTbl[n]; }


struct BuiltinPrimRecord
{
    char*	name;
    PRIMFN*	fn;
    Prim**	clientPrim;
    ArgNum	min;
    ArgNum	max;
    unsigned short filler_up_please; 
};


class BuiltinPrim
{
  private:

    static BuiltinPrim*			root;
    const BuiltinPrimRecord* const	record;
    const BuiltinPrim* const		link;

    void init () const;

  public:

    BuiltinPrim (const BuiltinPrimRecord*);

    static void		initBuiltinPrims ();
};

int debug_builtinprim(char *);
inline
BuiltinPrim::BuiltinPrim (const BuiltinPrimRecord* bpr)
    : record(bpr), link(BuiltinPrim::root)
{
    debug_builtinprim(this->record->name );
    BuiltinPrim::root = this;
}





#define INTERNAL_PRIM_NAME(pname) name2(_i_,pname)
#define INTERNAL_PRIM_REC(pname) name2(_pr_,pname)

#if 0
#define DEF(ext_name,int_name,min,max)					      \
Prim* int_name;								      \
BUILTIN_PRIM(int_name);							      \
static BuiltinPrimRecord INTERNAL_PRIM_REC(int_name)			      \
    = {ext_name, PRIM_NAME(int_name), (min), (max), &int_name};		      \
static BuiltinPrim INTERNAL_PRIM_NAME(int_name) (&INTERNAL_PRIM_REC(int_name));      \
BUILTIN_PRIM(int_name)
#else
#define DEF(ext_name,int_name,min,max)					      \
Prim* int_name;								      \
BUILTIN_PRIM(int_name);							      \
static BuiltinPrimRecord INTERNAL_PRIM_REC(int_name)			      \
    = {ext_name, PRIM_NAME(int_name), &int_name,(min), (max),0 };		      \
static BuiltinPrim INTERNAL_PRIM_NAME(int_name) (&INTERNAL_PRIM_REC(int_name));      \
BUILTIN_PRIM(int_name)
#endif

#define CHECK(n,type,var)						      \
if (!IS_A(ARG(n), type) ) return PRIM_MISMATCH((n), _STRING(type));	      \
type* var = (type*) ARG(n);

#define CHECK_FIXNUM(n,var)						      \
if (!IS_FIXNUM(ARG(n))) return PRIM_MISMATCH((n), "Fixnum");	      \
int var = FIXVAL(ARG(n));

#define CHECK_SYM(n,var)						      \
if (!IS_SYM(ARG(n))) return PRIM_MISMATCH((n), "Symbol");		      \
Ob* var = ARG(n);

#define CHECK_TYPE(n,typ,var) \
if (!(typeGreaterEq(CLASS_SBO(typ),ARG(n)))) \
return PRIM_MISMATCH((n), _STRING(typ)); \
typ* var = (typ*)(ARG(n));

#define CHECK_TYPE_BASE(n,typ,var) \
if (!(typeGreaterEq(CLASS_SBO(typ),ARG(n)))) \
return PRIM_MISMATCH((n), _STRING(typ)); \
typ* var = (typ*)(BASE(ARG(n)));

#define PRIM_MISMATCH(n,s) __PRIM__->mismatch(__CTXT__,n,s)
#define PRIM_ERROR(msg) __PRIM__->runtimeError(__CTXT__,msg)

#define NARGS __CTXT__->nargs
#define ARGS __CTXT__->argvec
#define ARG ARGS->elem

#endif

#pragma once
#include <Urho3D/Urho3DAll.h>
#include "MyTools.h"
#include <vector>
#include <stdarg.h>
#include "Viking.h"
#include <initializer_list>

//1. implicitly make printable from any var
//2. feed any vars into function
//3. convert the printables into string
//4. print

using namespace Urho3D;
class Printable
{
	friend Printable operator+(const Printable p1, const Printable p2) { return Printable(p1.str + " " + p2.str); }
public:
	Printable();
template <typename T>
	Printable(T t) { str = String(t); }
	~Printable();
	operator String() { return str; }
	String str = "";
};

extern void MyPrint(std::initializer_list<Printable> list);
//spaghetti version with no brackets required
extern void MyPrint(Printable p1="",Printable p2="", Printable p3="", Printable p4="", Printable p5="", Printable p6="", Printable p7="", Printable p8="", Printable p9="", Printable p10="", Printable p11="", Printable p12="");
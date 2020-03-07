#include "Printable.h"
Printable::Printable()
{

}
Printable::~Printable()
{

}
void MyPrint(Printable p1, Printable p2, Printable p3, Printable p4, Printable p5, Printable p6, Printable p7, Printable p8, Printable p9, Printable p10, Printable p11, Printable p12) {
	String text = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12;
	URHO3D_LOGINFO(text);
}
void MyPrint(std::initializer_list<Printable> list) {
	String text = "";
	for (auto p :list) {
		text += p.str;
		text += " ";
	}
	URHO3D_LOGINFO(text);
}